// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmeapi.h>
#include "Microsoft/COMPointer.h"
#include "PolicyConfig.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogSystemAudioLiteManager, Log, All);

class FSystemAudioLiteManager
{
	private:
		FSystemAudioLiteManager();
		~FSystemAudioLiteManager();
		
		static FSystemAudioLiteManager* Instance;

#if PLATFORM_WINDOWS
		TComPtr<IPolicyConfigVista>     PolicyConfigVista;
		TComPtr<IPolicyConfig>			PolicyConfig;
		TComPtr<IMMDeviceEnumerator>	DeviceEnumerator;
#endif

	public:
		static FSystemAudioLiteManager* Get();
		
		static void DestroyInstance();

		void Init();
		
		void SetVolume(float Value, const FString& DeviceId = FString(TEXT("")));
		float GetVolume(const FString& DeviceId = FString(TEXT("")));

		TMap<FString, FString> GetActiveDevices();

		FString GetDefaultDeviceId();
		FString GetDefaultDeviceName();

		FString GetDeviceNameFromId(const FString& DeviceId);

		FString GetDeviceIdFromName(const FString& DeviceName);

	private:
		FORCEINLINE float GetScalarFromValue(int32 Value);

		FORCEINLINE float GetValueFromScalar(float Value);

#if PLATFORM_WINDOWS
		TComPtr<IMMDevice> GetDevice(const FString& DeviceId = FString(TEXT("")));
		FString GetDeviceName(const TComPtr<IMMDevice>& Device) const;
		FString GetDeviceId(const TComPtr<IMMDevice>& Device) const;
		TComPtr<IAudioEndpointVolume> GetAudioEndpointVolume(const TComPtr<IMMDevice>& Device);
		TComPtr<IAudioEndpointVolume> GetAudioEndpointVolume(const FString& DeviceId = FString(TEXT("")));
#endif
};