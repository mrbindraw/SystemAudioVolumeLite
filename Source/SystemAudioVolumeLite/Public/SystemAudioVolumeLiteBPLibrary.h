// Copyright 2024 Andrew Bindraw. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SystemAudioLiteManager.h"
#include "SystemAudioVolumeLiteBPLibrary.generated.h"

UCLASS()
class USystemAudioVolumeLiteBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Default Device Name")
	static FString GetDefaultDeviceName();

	UFUNCTION(BlueprintCallable, Category = "SystemAudioVolumeLite", DisplayName = "Set System Audio Master Volume")
	static void SetMasterVolume(float Value);

	UFUNCTION(BlueprintPure, Category = "SystemAudioVolumeLite", DisplayName = "Get System Audio Master Volume")
	static float GetMasterVolume();

};
