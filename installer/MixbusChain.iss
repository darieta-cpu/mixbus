; Installer Windows per MixbusChain (VST3, 64 bit) - Inno Setup 6
; Si compila da riga di comando:  ISCC.exe installer\MixbusChain.iss
; Prima serve la cartella dist\MixbusChain.vst3 (la prepara la build su GitHub).

#define MyAppName      "MixbusChain"
#define MyAppVersion   "0.1.0"
#define MyAppPublisher "DaniArieta"

[Setup]
AppId={{31E0782B-3A99-45C4-9F79-5B97D6C4327F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
; Nessuna cartella dell'applicazione: il plugin va nella cartella VST3 di sistema
CreateAppDir=no
DisableProgramGroupPage=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputDir=..\dist
OutputBaseFilename={#MyAppName}-Setup-{#MyAppVersion}
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
UninstallDisplayName={#MyAppName} (VST3)

[Files]
Source: "..\dist\{#MyAppName}.vst3\*"; DestDir: "{commoncf64}\VST3\{#MyAppName}.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
