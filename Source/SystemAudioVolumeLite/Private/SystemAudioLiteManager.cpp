// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#include "SystemAudioLiteManager.h"

DEFINE_LOG_CATEGORY(LogSystemAudioLiteManager);

FSystemAudioLiteManager* FSystemAudioLiteManager::Instance = nullptr;

FSystemAudioLiteManager::FSystemAudioLiteManager()
#if PLATFORM_WINDOWS
: PolicyConfigVista(nullptr),
	PolicyConfig(nullptr),
	DeviceEnumerator(nullptr)
#endif
{

}

void FSystemAudioLiteManager::Init()
{
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::CoInitialize();

	CoCreateInstance(__uuidof(CPolicyConfigVistaClient), nullptr, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID*)&PolicyConfigVista);

	// For Win10
	HRESULT Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig2, (LPVOID*)&PolicyConfig);
	if (Result != S_OK)
	{
		Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig1, (LPVOID*)&PolicyConfig);
	}

	// For Win Vista, 7, 8, 8.1
	if (Result != S_OK)
	{
		Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig0, (LPVOID*)&PolicyConfig);
	}

	CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&DeviceEnumerator);
#endif
}

FSystemAudioLiteManager* FSystemAudioLiteManager::Get()
{
	if(Instance == nullptr)
	{
		Instance = new FSystemAudioLiteManager;
	}
	
	return Instance;
}

FSystemAudioLiteManager::~FSystemAudioLiteManager()
{
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::CoUninitialize();
#endif
}

void FSystemAudioLiteManager::DestroyInstance()
{
	if(Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

FString FSystemAudioLiteManager::GetDefaultDeviceName() const
{
#if PLATFORM_WINDOWS
	return GetDeviceName(GetDevice());
#else
	return FString(TEXT(""));
#endif
}

FString FSystemAudioLiteManager::GetDefaultDeviceId() const
{
#if PLATFORM_WINDOWS
	return GetDeviceId(GetDevice());
#else
	return FString(TEXT(""));
#endif
}

FString FSystemAudioLiteManager::GetDeviceNameFromId(const FString& DeviceId) const
{
	FString DeviceName;

	for (const TPair<FString, FString>& Device : GetActiveDevices())
	{
		if (Device.Key == DeviceId)
		{
			DeviceName = Device.Value;
			break;
		}
	}

	return DeviceName;
}

FString FSystemAudioLiteManager::GetDeviceIdFromName(const FString& DeviceName) const
{
	FString DeviceId;

	for (const TPair<FString, FString>& Device : GetActiveDevices())
	{
		if (Device.Value == DeviceName)
		{
			DeviceId = Device.Key;
			break;
		}
	}

	return DeviceId;
}

TMap<FString, FString> FSystemAudioLiteManager::GetActiveDevices() const
{
	TMap<FString, FString> ActiveDevices;

#if PLATFORM_WINDOWS
	TComPtr<IMMDeviceCollection> DevicesCollection;
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd371400(v=vs.85).aspx, see Return value!
	HRESULT Result = DeviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &DevicesCollection);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, DeviceEnumerator->EnumAudioEndpoints, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return TMap<FString, FString>();
	}

	UINT CountActiveDevices = 0;
	Result = DevicesCollection->GetCount(&CountActiveDevices);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, DevicesCollection->GetCount, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return TMap<FString, FString>();
	}

	for (UINT i = 0; i < CountActiveDevices; i++)
	{
		TComPtr<IMMDevice> Device;
		DevicesCollection->Item(i, &Device);

		const FString DeviceId = GetDeviceId(Device);
		const FString DeviceName = GetDeviceName(Device);

		ActiveDevices.Add(DeviceId, DeviceName);
	}
#endif

	return ActiveDevices;
}

void FSystemAudioLiteManager::SetVolume(float Value, const FString& DeviceId)
{
	if (FMath::IsNearlyEqual(Value, GetVolume(DeviceId), 0.001f))
	{
		return;
	}

	float Volume = GetScalarFromValue(Value);

#if PLATFORM_WINDOWS
	TComPtr<IAudioEndpointVolume> AudioEndpointVolume = GetAudioEndpointVolume(DeviceId);
	if (!AudioEndpointVolume.IsValid())
	{
		return;
	}

	HRESULT Result = AudioEndpointVolume->SetMasterVolumeLevelScalar(Volume, nullptr);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, AudioEndpointVolume->SetMasterVolumeLevelScalar, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return;
	}
#endif
}

float FSystemAudioLiteManager::GetVolume(const FString& DeviceId) const
{
	float Volume = 0.0f;

#if PLATFORM_WINDOWS
	TComPtr<IAudioEndpointVolume> AudioEndpointVolume = GetAudioEndpointVolume(DeviceId);
	if (!AudioEndpointVolume.IsValid())
	{
		return 0.0f;
	}

	HRESULT Result = AudioEndpointVolume->GetMasterVolumeLevelScalar(&Volume);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, AudioEndpointVolume->GetMasterVolumeLevelScalar, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return 0.0f;
	}
#endif

	return GetValueFromScalar(Volume);
}


float FSystemAudioLiteManager::GetScalarFromValue(int32 Value) const
{
	return FMath::Abs(Value) >= 100.0f ? 1.0f : Value / 100.0f;
}

float FSystemAudioLiteManager::GetValueFromScalar(float Value) const
{
	return FMath::RoundToFloat(FMath::Abs(Value) > 0.0f ? Value * 100.0f : 0.0f);
}

#if PLATFORM_WINDOWS
TComPtr<IMMDevice> FSystemAudioLiteManager::GetDevice(const FString& DeviceId) const
{
	TComPtr<IMMDevice> Device;
	HRESULT Result = S_OK;

	if (DeviceId.IsEmpty())
	{
		Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &Device);
		if (Result != S_OK)
		{
			UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, DeviceEnumerator->GetDefaultAudioEndpoint, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
			return nullptr;
		}
	}
	else
	{
		Result = DeviceEnumerator->GetDevice(TCHAR_TO_WCHAR(*DeviceId), &Device);
		if (Result != S_OK)
		{
			UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, DeviceEnumerator->GetDevice, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
			return nullptr;
		}
	}

	return Device;
}

FString FSystemAudioLiteManager::GetDeviceName(const TComPtr<IMMDevice>& Device) const
{
	if (!Device.IsValid())
	{
		return FString(TEXT(""));
	}

	TComPtr<IPropertyStore> PropertyStore;
	Device->OpenPropertyStore(STGM_READ, &PropertyStore);

	PROPVARIANT nameDevice;
	PropVariantInit(&nameDevice);
	PropertyStore->GetValue(PKEY_Device_FriendlyName, &nameDevice);

	const FString DeviceName = FString(WCHAR_TO_TCHAR(nameDevice.pwszVal));

	PropVariantClear(&nameDevice);

	return DeviceName;
}

FString FSystemAudioLiteManager::GetDeviceId(const TComPtr<IMMDevice>& Device) const
{
	if (!Device.IsValid())
	{
		return FString(TEXT(""));
	}

	WCHAR* swDeviceId = TCHAR_TO_WCHAR(TEXT(""));
	HRESULT Result = Device->GetId(&swDeviceId);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, Device->GetId, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return FString(TEXT(""));
	}

	const FString DeviceId = FString(WCHAR_TO_TCHAR(swDeviceId));

	CoTaskMemFree(swDeviceId);
	swDeviceId = nullptr;

	return DeviceId;
}

TComPtr<IAudioEndpointVolume> FSystemAudioLiteManager::GetAudioEndpointVolume(const TComPtr<IMMDevice>& Device) const
{
	if (!Device.IsValid())
	{
		return nullptr;
	}

	TComPtr<IAudioEndpointVolume> AudioEndpointVolume;
	HRESULT Result = Device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (PVOID*)&AudioEndpointVolume);
	if (Result != S_OK)
	{
		UE_LOG(LogSystemAudioLiteManager, Warning, TEXT("Result != S_OK, Device->Activate, [%s], line: %d"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__);
		return nullptr;
	}

	return AudioEndpointVolume;
}

TComPtr<IAudioEndpointVolume> FSystemAudioLiteManager::GetAudioEndpointVolume(const FString& DeviceId) const
{
	TComPtr<IMMDevice> Device = GetDevice(DeviceId);
	if (!Device.IsValid())
	{
		return nullptr;
	}

	TComPtr<IAudioEndpointVolume> AudioEndpointVolume = GetAudioEndpointVolume(Device);
	if (!AudioEndpointVolume.IsValid())
	{
		return nullptr;
	}

	return AudioEndpointVolume;
}
#endif // PLATFORM_WINDOWS