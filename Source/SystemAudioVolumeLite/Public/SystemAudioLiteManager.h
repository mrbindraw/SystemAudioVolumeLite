// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmeapi.h>
#include "PolicyConfig.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

class FSystemAudioLiteManager
{
	private:
		FSystemAudioLiteManager();
		~FSystemAudioLiteManager();
		
		static FSystemAudioLiteManager *Instance;

#if PLATFORM_WINDOWS
		IAudioEndpointVolume    *AudioEndpointVolume;
		IMMDevice               *DefaultDevice;
		IMMDeviceEnumerator     *DeviceEnumerator;
		IMMDeviceCollection     *DevicesCollection;
		IPropertyStore          *PropertyStore;

		IPolicyConfigVista      *PolicyConfigVista;
		IPolicyConfig           *PolicyConfig;
#endif

	public:
		static FSystemAudioLiteManager *Get();
		
		static void DestroyInstance();
		
		void SetMasterVolume(float Value);
		float GetMasterVolume();

		TMap<FString, FString> GetActiveDevices();

		FString GetDefaultDeviceId();
		FString GetDefaultDeviceName();

		FString GetDeviceNameFromId(const FString &DeviceId);

	private:
		FORCEINLINE float GetScalarFromValue(int32 Value);

		FORCEINLINE float GetValueFromScalar(float Value);
};