// Copyright 2024 Andrew Bindraw. All Rights Reserved.

#include "SystemAudioLiteManager.h"

FSystemAudioLiteManager *FSystemAudioLiteManager::Instance = nullptr;

FSystemAudioLiteManager::FSystemAudioLiteManager() : 
	AudioEndpointVolume(nullptr),
	DefaultDevice(nullptr),
	DeviceEnumerator(nullptr),
	DevicesCollection(nullptr),
	PropertyStore(nullptr),
	PolicyConfigVista(nullptr),
	PolicyConfig(nullptr)
{
	FWindowsPlatformMisc::CoInitialize();

	CoCreateInstance(__uuidof(CPolicyConfigVistaClient), nullptr, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&PolicyConfigVista);

	// For Win10
	HRESULT Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig2, (LPVOID *)&PolicyConfig);
	if (Result != S_OK)
	{
		Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig1, (LPVOID *)&PolicyConfig);
	}

	// For Win Vista, 7, 8, 8.1
	if (Result != S_OK)
	{
		Result = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_INPROC, IID_IPolicyConfig0, (LPVOID *)&PolicyConfig);
	}

	CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&DeviceEnumerator);
}

FSystemAudioLiteManager *FSystemAudioLiteManager::Get()
{
	if(Instance == nullptr)
	{
		Instance = new FSystemAudioLiteManager;
	}
	
	return Instance;
}

FSystemAudioLiteManager::~FSystemAudioLiteManager()
{
	FWindowsPlatformMisc::CoUninitialize();
}

void FSystemAudioLiteManager::DestroyInstance()
{
	if(Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

FString FSystemAudioLiteManager::GetDefaultDeviceName()
{
	return GetDeviceNameFromId(GetDefaultDeviceId());
}

FString FSystemAudioLiteManager::GetDefaultDeviceId()
{
	HRESULT Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &DefaultDevice);
	if (Result != S_OK)
	{
		return FString(TEXT(""));
	}

	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd371405(v=vs.85).aspx, see Return value!
	Result = DefaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (PVOID *)&AudioEndpointVolume);
	if (Result != S_OK)
	{
		return FString(TEXT(""));
	}

	WCHAR *swId = TCHAR_TO_WCHAR(TEXT(""));
	Result = DefaultDevice->GetId(&swId);
	if (Result != S_OK)
	{
		return FString(TEXT(""));
	}

	return FString(WCHAR_TO_TCHAR(swId));
}

FString FSystemAudioLiteManager::GetDeviceNameFromId(const FString &DeviceId)
{
	FString DeviceName;

	for (TPair<FString, FString> &Device : GetActiveDevices())
	{
		if (Device.Key == DeviceId)
		{
			DeviceName = Device.Value;
		}
	}

	return DeviceName;
}

TMap<FString, FString> FSystemAudioLiteManager::GetActiveDevices()
{
	TMap<FString, FString> ActiveDevices;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd371400(v=vs.85).aspx, see Return value!
	HRESULT Result = DeviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &DevicesCollection);
	if (Result != S_OK)
	{
		return TMap<FString, FString>();
	}

	UINT count = 0;
	DevicesCollection->GetCount(&count);
	LPWSTR pwszID = nullptr;

	for (UINT i = 0; i < count; i++)
	{
		DevicesCollection->Item(i, &DefaultDevice);
		DefaultDevice->GetId(&pwszID);
		DefaultDevice->OpenPropertyStore(STGM_READ, &PropertyStore);

		PROPVARIANT nameDevice;
		PropVariantInit(&nameDevice);
		PropertyStore->GetValue(PKEY_Device_FriendlyName, &nameDevice);

		ActiveDevices.Add(FString(WCHAR_TO_TCHAR(pwszID)));
		ActiveDevices[FString(WCHAR_TO_TCHAR(pwszID))] = FString(WCHAR_TO_TCHAR(nameDevice.pwszVal));

		CoTaskMemFree(pwszID);
		pwszID = nullptr;
	}

	if (PropertyStore)
	{
		PropertyStore->Release();
		PropertyStore = nullptr;
	}

	if (DefaultDevice)
	{
		DefaultDevice->Release();
		DefaultDevice = nullptr;
	}

	if (DevicesCollection)
	{
		DevicesCollection->Release();
		DevicesCollection = nullptr;
	}

	return ActiveDevices;
}

void FSystemAudioLiteManager::SetMasterVolume(float Value)
{
	float MasterVolume = this->GetScalarFromValue(Value);

	HRESULT Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &DefaultDevice);
	if (Result != S_OK)
	{
		return;
	}

	Result = DefaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (PVOID *)&AudioEndpointVolume);
	if (Result != S_OK)
	{
		return;
	}

	Result = AudioEndpointVolume->SetMasterVolumeLevelScalar(MasterVolume, nullptr);
	if (Result != S_OK)
	{
		return;
	}

	if (AudioEndpointVolume)
	{
		AudioEndpointVolume->Release();
		AudioEndpointVolume = nullptr;
	}

	if (DefaultDevice)
	{
		DefaultDevice->Release();
		DefaultDevice = nullptr;
	}
}

float FSystemAudioLiteManager::GetMasterVolume()
{
	HRESULT Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &DefaultDevice);
	if (Result != S_OK)
	{
		return 0.0f;
	}

	Result = DefaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (PVOID *)&AudioEndpointVolume);
	if (Result != S_OK)
	{
		return 0.0f;
	}

	float MasterVolume = 0.0f;
	Result = AudioEndpointVolume->GetMasterVolumeLevelScalar(&MasterVolume);
	if (Result != S_OK)
	{
		return 0.0f;
	}

	if (AudioEndpointVolume)
	{
		AudioEndpointVolume->Release();
		AudioEndpointVolume = nullptr;
	}

	if (DefaultDevice)
	{
		DefaultDevice->Release();
		DefaultDevice = nullptr;
	}

	return GetValueFromScalar(MasterVolume);
}


float FSystemAudioLiteManager::GetScalarFromValue(int32 Value)
{
	return FMath::Abs(Value) >= 100.0f ? 1.0f : Value / 100.0f;
}

float FSystemAudioLiteManager::GetValueFromScalar(float Value)
{
	return FMath::RoundToFloat(FMath::Abs(Value) > 0.0f ? Value * 100.0f : 0.0f);
}
