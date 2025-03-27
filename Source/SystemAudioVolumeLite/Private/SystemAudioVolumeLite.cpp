// Copyright 2025 Andrew Bindraw. All Rights Reserved.

#include "SystemAudioVolumeLite.h"
#include "SystemAudioLiteManager.h"

#define LOCTEXT_NAMESPACE "FSystemAudioVolumeLiteModule"

void FSystemAudioVolumeLiteModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FSystemAudioLiteManager::Get();
}

void FSystemAudioVolumeLiteModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FSystemAudioLiteManager::DestroyInstance();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSystemAudioVolumeLiteModule, SystemAudioVolumeLite)