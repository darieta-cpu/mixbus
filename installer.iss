[Setup]
AppName=MixbusChain
AppVersion=0.1.0
AppPublisher=DaniArieta
DefaultDirName={autopf}\MixbusChain
DisableDirPage=yes
DisableProgramGroupPage=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputDir=output
OutputBaseFilename=MixbusChain-Setup
Compression=lzma
SolidCompression=yes

[Files]
Source: "dist\MixbusChain.vst3\*"; DestDir: "{commonpf64}\Common Files\VST3\MixbusChain.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
