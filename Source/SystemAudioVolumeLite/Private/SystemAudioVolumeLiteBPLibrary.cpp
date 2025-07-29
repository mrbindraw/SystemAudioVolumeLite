// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#include "SystemAudioVolumeLiteBPLibrary.h"
#include "SystemAudioVolumeLite.h"

USystemAudioVolumeLiteBPLibrary::USystemAudioVolumeLiteBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USystemAudioVolumeLiteBPLibrary::PostInitProperties()
{
	Super::PostInitProperties();

	FSystemAudioLiteManager::Get()->Init();
}

FString USystemAudioVolumeLiteBPLibrary::GetDefaultDeviceName()
{
	return FSystemAudioLiteManager::Get()->GetDefaultDeviceName();
}

void USystemAudioVolumeLiteBPLibrary::GetDeviceIdFromName(const FString& InDeviceName, FString& OutDeviceId)
{
	OutDeviceId = FSystemAudioLiteManager::Get()->GetDeviceIdFromName(InDeviceName);
}

void USystemAudioVolumeLiteBPLibrary::GetDeviceNameFromId(const FString& InDeviceId, FString& OutDeviceName)
{
	OutDeviceName = FSystemAudioLiteManager::Get()->GetDeviceNameFromId(InDeviceId);
}

void USystemAudioVolumeLiteBPLibrary::SetVolume(float Value)
{
	FSystemAudioLiteManager::Get()->SetVolume(Value);
}

void USystemAudioVolumeLiteBPLibrary::SetVolumeForDeviceId(const FString& DeviceId, float Volume)
{
	FSystemAudioLiteManager::Get()->SetVolume(Volume, DeviceId);
}

void USystemAudioVolumeLiteBPLibrary::SetVolumeForDeviceName(const FString& DeviceName, float Volume)
{
	const FString DeviceId = FSystemAudioLiteManager::Get()->GetDeviceIdFromName(DeviceName);
	FSystemAudioLiteManager::Get()->SetVolume(Volume, DeviceId);
}

float USystemAudioVolumeLiteBPLibrary::GetVolume()
{
	return FSystemAudioLiteManager::Get()->GetVolume();
}

void USystemAudioVolumeLiteBPLibrary::GetVolumeForDeviceId(const FString& InDeviceId, float& OutVolume)
{
	OutVolume = FSystemAudioLiteManager::Get()->GetVolume(InDeviceId);
}

void USystemAudioVolumeLiteBPLibrary::GetVolumeForDeviceName(const FString& InDeviceName, float& OutVolume)
{
	const FString DeviceId = FSystemAudioLiteManager::Get()->GetDeviceIdFromName(InDeviceName);
	OutVolume = FSystemAudioLiteManager::Get()->GetVolume(DeviceId);
}

TMap<FString, FString> USystemAudioVolumeLiteBPLibrary::GetActiveDevices()
{
	return FSystemAudioLiteManager::Get()->GetActiveDevices();
}