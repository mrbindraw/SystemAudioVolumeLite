# SystemAudioVolumeLite
This is a free version of the plugin [SystemAudioVolume](https://www.fab.com/listings/344d268f-7e3a-4c5c-9183-2d7850bd37ed) with the lite functionality.  
This plugin can get the default audio device name and change audio volume in the system. 

**C++ / Blueprints functions:**
- FString GetDefaultDeviceName / GetSystemAudioDefaultDeviceName ( );
- GetDeviceIdFromName / GetSystemAudioDeviceIdFromName (const FString& InDeviceName, FString& OutDeviceId);
- GetDeviceNameFromId / GetSystemAudioDeviceNameFromId (const FString& InDeviceId, FString& OutDeviceName);
- SetVolume / SetSystemAudioVolume(float Value);
- SetVolumeForDeviceId / SetSystemAudioVolumeForDeviceId (const FString& DeviceId, float Volume);
- float GetVolume / GetSystemAudioVolume ( );
- float GetVolumeForDeviceId / GetSystemAudioVolumeForDeviceId (const FString& DeviceId);
- TMap<FString, FString> GetActiveDevices / GetSystemAudioActiveDevices( );

# Install into Project
You can install manually by extracting archive `SystemAudioVolumeLite-X.X.X-UE-X.X.zip` from [Releases](https://github.com/mrbindraw/SystemAudioVolumeLite/releases)  
to your project plugins folder or build example project `ExamplePrj-UE-X.X-SystemAudioVolumeLite-X.X.X.zip`  
Documentation: [README_SystemAudioVolumeLite](https://docs.google.com/document/d/13qQpKiYOstN0GyQf7LARekuegFQJSqb79T_gIKM_G9o/edit?usp=drive_link)  
Example project: [DemoSysAudioVolumeLite](https://drive.google.com/drive/folders/1ZMCjGoutt0LQ7CcDy8sjNeaLxmW3vNlu?usp=drive_link)

# Install into Unreal Engine
You can install it from the marketplace [SystemAudioVolumeLite](https://www.fab.com/listings/9614c75e-f88e-4f39-9310-8f1a86d7a53b)  

Manual:   
1. Download and extracting archive `SystemAudioVolumeLite-X.X.X-UE-X.X.zip` from [Releases](https://github.com/mrbindraw/SystemAudioVolumeLite/releases) to any disk path, for example: `D:\Plugins`  
2. Than open any terminal (cmd, powershell) in `D:\Plugins` folder  
3. Launch `RunUAT` in the terminal with arguments, for example:

Windows:
```  
D:\EpicGames\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat BuildPlugin -Plugin=D:\Plugins\SystemAudioVolumeLite\SystemAudioVolumeLite.uplugin -Package=D:\Plugins\UE_5.4\SystemAudioVolumeLite -Rocket
```
Mac:
```
sh "/Users/Shared/Epic Games/UE_5.4/Engine/Build/BatchFiles/RunUAT.sh" BuildPlugin -Plugin="$PWD/SystemAudioVolumeLite/SystemAudioVolumeLite.uplugin" -Package="$PWD/5.4/SystemAudioVolumeLite" -Rocket
```
4. If you see the message `BUILD SUCCESSFUL` in the terminal after the build is complete,  
copy the `SystemAudioVolumeLite` folder from `D:\Plugins\UE_5.4` to `D:\EpicGames\UE_5.4\Engine\Plugins\Marketplace`  
> [!IMPORTANT]
> **The engine path and folder names may differ on your system.**
