// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SystemAudioLiteManager.h"
#include "SystemAudioVolumeLiteBPLibrary.generated.h"

UCLASS()
class SYSTEMAUDIOVOLUMELITE_API USystemAudioVolumeLiteBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Default Device Name")
	static FString GetDefaultDeviceName();

	UFUNCTION(BlueprintCallable, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Device Id From Name")
	static void GetDeviceIdFromName(const FString& InDeviceName, FString& OutDeviceId);

	UFUNCTION(BlueprintCallable, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Device Name From Id")
	static void GetDeviceNameFromId(const FString& InDeviceId, FString& OutDeviceName);

	UFUNCTION(BlueprintCallable, Category = "SystemAudioVolumeLite", DisplayName = "Set System Audio Volume")
	static void SetVolume(float Value);

	/** If DeviceId is empty then it will set Volume value to the default device audio in the system */
	UFUNCTION(BlueprintCallable, Category = "SystemAudioVolumeLite", DisplayName = "Set System Audio Volume For DeviceId")
	static void SetVolumeForDeviceId(const FString& DeviceId, float Volume);

	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Volume")
	static float GetVolume();

	/** If DeviceId is empty then it will return volume value from the default device audio in the system */
	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Volume For DeviceId")
	static float GetVolumeForDeviceId(const FString& DeviceId);
	
	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Active Devices")
	static TMap<FString, FString> GetActiveDevices();

};
