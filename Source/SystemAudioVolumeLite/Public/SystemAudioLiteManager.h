// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmeapi.h>
#include "PolicyConfig.h"
#include "Windows/HideWindowsPlatformTypes.h"

class FSystemAudioLiteManager
{
	private:
		FSystemAudioLiteManager();
		~FSystemAudioLiteManager();
		
		static FSystemAudioLiteManager *Instance;

		IAudioEndpointVolume    *AudioEndpointVolume;
		IMMDevice               *DefaultDevice;
		IMMDeviceEnumerator     *DeviceEnumerator;
		IMMDeviceCollection     *DevicesCollection;
		IPropertyStore          *PropertyStore;

		IPolicyConfigVista      *PolicyConfigVista;
		IPolicyConfig           *PolicyConfig;

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