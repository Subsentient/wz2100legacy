;Warzone 2100 Legacy Project Installer Script

;--------------------------------
;Defines

  !define APPLICATION_NAME "Warzone 2100 Legacy"
  !define APPLICATION_VERSION "microwave_popping"
  !define APPLICATION_PUBLISHER "Warzone 2100 Legacy Project"
  !define APPLICATION_WEB_SITE "http://universe2.us/wzlegacy"
  !define INSTALL_LICENSE "${TOP_BUILDDIR}\COPYING"
  !define INSTALL_PATH "$PROGRAMFILES\${APPLICATION_NAME}"
  !define INSTALL_GRAPHICS "nsis"

  !define APPLICATION_VERSION_MAJOR microwave_popping
  !define APPLICATION_VERSION_MINOR 0
  !define APPLICATION_VERSION_REVISION 0
;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;Configuration

  ;General
  Name "${APPLICATION_NAME} ${APPLICATION_VERSION}"
  OutFile "wz2100legacy-${APPLICATION_VERSION}-installer.exe"

  ;Folder selection page
  InstallDir "${INSTALL_PATH}"

  ;Get install folder from registry if available
  InstallDirRegKey HKCU "Software\${APPLICATION_PUBLISHER}\${APPLICATION_NAME}" ""

  VIProductVersion "0.0.0.0"
  VIAddVersionKey "FileDescription" "${APPLICATION_NAME} Installer"
  VIAddVersionKey "CompanyName" "${APPLICATION_PUBLISHER}"
  VIAddVersionKey "ProductName" "${APPLICATION_NAME} Installer"
  VIAddVersionKey "Comments" "${APPLICATION_NAME} Installer"
  ;VIAddVersionKey "LegalTrademarks" "${APPLICATION_PUBLISHER}"
  VIAddVersionKey "LegalCopyright" "${APPLICATION_PUBLISHER}"
  VIAddVersionKey "FileVersion" "${APPLICATION_VERSION}"

;--------------------------------
;Variables

  ;Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  ;Generic settings
  !define MUI_ABORTWARNING

  ;Welcome page
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${TOP_BUILDDIR}\icons\wz2100l_welcome.bmp"
  ;!define MUI_WELCOMEPAGE_TITLE_3LINES

  ;Install pages
  !define MUI_HEADERIMAGE_BITMAP "${TOP_BUILDDIR}\icons\wz2100l_header.bmp"
  !define MUI_HEADERIMAGE
  !define MUI_COMPONENTSPAGE_NODES

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${APPLICATION_PUBLISHER}\${APPLICATION_NAME}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${APPLICATION_NAME}"

  ;Finish page
  !define MUI_FINISHPAGE_LINK "Go to the ${APPLICATION_PUBLISHER}'s homepage"
  !define MUI_FINISHPAGE_LINK_LOCATION "${APPLICATION_WEB_SITE}"
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  ;!define MUI_FINISHPAGE_TITLE_3LINES

;--------------------------------
;Installer Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${INSTALL_LICENSE}"
  ;!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
; Uninstaller Pages
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  ;!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
  Icon "${TOP_BUILDDIR}\icons\wz2100legacy.ico"
  UninstallIcon "${TOP_BUILDDIR}\icons\wz2100legacy.ico"

;--------------------------------
;Installler Secions

Section "Install"

  ;Store install folder
  WriteRegStr HKCU "Software\${APPLICATION_PUBLISHER}\${APPLICATION_NAME}" "" $INSTDIR

  ;Files
  SetOutPath "$INSTDIR"
  File "${TOP_BUILDDIR}\src\wz2100legacy.exe"
  File "${TOP_BUILDDIR}\data\mp.wzl"
  File "${TOP_BUILDDIR}\data\base.wzl"
  File "${TOP_BUILDDIR}\AUTHORS"
  File "${TOP_BUILDDIR}\ChangeLog"
  File "${TOP_BUILDDIR}\COPYING"
  File "${TOP_BUILDDIR}\COPYING.NONGPL"
  File "${TOP_BUILDDIR}\COPYING.README"
  CreateDirectory "$INSTDIR\music"
  SetOutPath "$INSTDIR\music"
  File "${TOP_BUILDDIR}\data\music\menu.ogg"
  File "${TOP_BUILDDIR}\data\music\track1.ogg"
  File "${TOP_BUILDDIR}\data\music\track2.ogg"
  File "${TOP_BUILDDIR}\data\music\track3.ogg"
  File "${TOP_BUILDDIR}\data\music\music.wpl"
  SetOutPath "$INSTDIR\fonts"
  NSISdl::download "http://universe2.us/wzlegacy/winfonts/fonts.conf" "fonts.conf"
  NSISdl::download "http://universe2.us/wzlegacy/winfonts/DejaVuSans.ttf" "DejaVuSans.ttf"
  NSISdl::download "http://universe2.us/wzlegacy/winfonts/DejaVuSans-Bold.ttf" "DejaVuSans-Bold.ttf"
  NSISdl::download "http://universe2.us/wzlegacy/winfonts/README" "README"
  NSISdl::download "http://universe2.us/wzlegacy/winfonts/LICENSE" "LICENSE"
  
  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLICATION_NAME}.lnk" "$INSTDIR\wz2100legacy.exe"

  ;Create uninstaller
  WriteUninstaller "Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${APPLICATION_NAME}.lnk" "$INSTDIR\Uninstall.exe"
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayName" "${APPLICATION_NAME} ${APPLICATION_VERSION}"
  ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayIcon" "$INSTDIR\Screening.exe,0"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayVersion" "${APPLICATION_VERSION}"
  ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "HelpLink" "${APP_URL}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "VersionMajor" "${APPLICATION_VERSION_MAJOR}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "VersionMinor" "${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_REVISION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "Publisher" "${APPLICATION_PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "URLInfoAbout" "${APPLICATION_WEB_SITE}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "NoRepair" "1"

SectionEnd

;--------------------------------
;Uninstall Files

Section "Uninstall"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
  Delete "$INSTDIR\wz2100legacy.exe"
  Delete "$INSTDIR\mp.wzl"
  Delete "$INSTDIR\AUTHORS"
  Delete "$INSTDIR\base.wzl"
  Delete "$INSTDIR\ChangeLog"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\COPYING.NONGPL"
  Delete "$INSTDIR\COPYING.README"
  Delete "$INSTDIR\music\*.*"
  RMDir	 "$INSTDIR\music"
  Delete "$INSTDIR\fonts\*.*"
  RMDir	 "$INSTDIR\fonts"
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLICATION_NAME}.lnk"
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${APPLICATION_NAME}.lnk"
  RMDir "$SMPROGRAMS\$STARTMENU_FOLDER\"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR\"

  DeleteRegKey /ifempty HKCU "Software\${APPLICATION_PUBLISHER}\${APPLICATION_NAME}"
  DeleteRegKey /ifempty HKCU "Software\${APPLICATION_PUBLISHER}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}"

SectionEnd

