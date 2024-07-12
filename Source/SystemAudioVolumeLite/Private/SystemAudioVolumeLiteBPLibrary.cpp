// Copyright 2024 Andrew Bindraw. All Rights Reserved.

#include "SystemAudioVolumeLiteBPLibrary.h"
#include "SystemAudioVolumeLite.h"

USystemAudioVolumeLiteBPLibrary::USystemAudioVolumeLiteBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	FSystemAudioLiteManager::Get();
}

FString USystemAudioVolumeLiteBPLibrary::GetDefaultDeviceName()
{
	return FSystemAudioLiteManager::Get()->GetDefaultDeviceName();
}

void USystemAudioVolumeLiteBPLibrary::SetMasterVolume(float Value)
{
	FSystemAudioLiteManager::Get()->SetMasterVolume(Value);
}

float USystemAudioVolumeLiteBPLibrary::GetMasterVolume()
{
	return FSystemAudioLiteManager::Get()->GetMasterVolume();
}

TMap<FString, FString> USystemAudioVolumeLiteBPLibrary::GetActiveDevices()
{
	return FSystemAudioLiteManager::Get()->GetActiveDevices();
}