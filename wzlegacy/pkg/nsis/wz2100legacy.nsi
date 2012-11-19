;Warzone 2100 Legacy Project Installer Script

;--------------------------------
;Defines

  !define APPLICATION_NAME "Warzone 2100 Legacy"
  !define APPLICATION_VERSION "microwave_popping"
  !define APPLICATION_PUBLISHER "Warzone 2100 Legacy Project"
  !define APPLICATION_WEB_SITE "http://wzlegacy.universe2.us/"
  !define INSTALL_LICENSE "${TOP_BUILDDIR}\COPYING"
  !define INSTALL_PATH "$PROGRAMFILES\${APPLICATION_NAME} ${APPLICATION_VERSION}"
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
  Name "${APPLICATION_NAME}"
  OutFile "${TOP_BUILDDIR}\wz2100legacy-${APPLICATION_VERSION}-installer.exe"
  RequestExecutionLevel admin ;We need to be an admin
  ;Folder selection page
  InstallDir "${INSTALL_PATH}"

  ;Get install folder from registry if available
  InstallDirRegKey HKCU "Software\${APPLICATION_NAME}" ""

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
  !define MUI_WELCOMEPAGE_TEXT \
"Welcome to the installer for ${APPLICATION_NAME} ${APPLICATION_VERSION}. \
IMPORTANT: A functional internet connection is required for installation, even if you choose not to install the videos. \
The fonts required for the game are downloaded over the internet. Click Next to proceed."

  ;Install pages
  !define MUI_HEADERIMAGE_BITMAP "${TOP_BUILDDIR}\icons\wz2100l_header.bmp"
  !define MUI_HEADERIMAGE
  !define MUI_COMPONENTSPAGE_NODES

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${APPLICATION_NAME}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${APPLICATION_NAME}"

  ;Finish page
  !define MUI_FINISHPAGE_LINK "Go to the ${APPLICATION_PUBLISHER}'s homepage"
  !define MUI_FINISHPAGE_LINK_LOCATION "${APPLICATION_WEB_SITE}"
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  ;!define MUI_FINISHPAGE_TITLE_3LINES
  !define MUI_FINISHPAGE_RUN_TEXT "Run ${APPLICATION_NAME} when I click Finish"
  !define MUI_FINISHPAGE_RUN "$INSTDIR\wz2100legacy.exe"

;--------------------------------
;Installer Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${INSTALL_LICENSE}"
  !insertmacro MUI_PAGE_COMPONENTS
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
;Installer Sections

Section "Base Game"
  SectionIn RO
  ;Store install folder
  WriteRegStr HKCU "Software\${APPLICATION_NAME}" "" $INSTDIR

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
  NSISdl::download "http://wzlegacy.universe2.us/winfonts/fonts.conf" "fonts.conf"
  NSISdl::download "http://wzlegacy.universe2.us/winfonts/DejaVuSans.ttf" "DejaVuSans.ttf"
  NSISdl::download "http://wzlegacy.universe2.us/winfonts/DejaVuSans-Bold.ttf" "DejaVuSans-Bold.ttf"
  NSISdl::download "http://wzlegacy.universe2.us/winfonts/README" "README"
  NSISdl::download "http://wzlegacy.universe2.us/winfonts/LICENSE" "LICENSE"
  
  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLICATION_NAME}.lnk" "$INSTDIR\wz2100legacy.exe"

  ;Create uninstaller
  WriteUninstaller "Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${APPLICATION_NAME}.lnk" "$INSTDIR\Uninstall.exe"
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayName" "${APPLICATION_NAME} ${APPLICATION_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayIcon" "$INSTDIR\${PACKAGE},0"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "DisplayVersion" "${APPLICATION_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "HelpLink" "${APPLICATION_WEB_SITE}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "VersionMajor" "${APPLICATION_VERSION_MAJOR}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "VersionMinor" "${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_REVISION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "Publisher" "${APPLICATION_PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "URLInfoAbout" "${APPLICATION_WEB_SITE}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}" "NoRepair" "1"

SectionEnd

Section "Non-English Language Support"
SetOutPath "$INSTDIR\locale\ca\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\ca_ES.gmo"
SetOutPath "$INSTDIR\locale\cs\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\cs.gmo"
SetOutPath "$INSTDIR\locale\da\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\da.gmo"
SetOutPath "$INSTDIR\locale\de\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\de.gmo"
SetOutPath "$INSTDIR\locale\en_GB\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\en_GB.gmo"
SetOutPath "$INSTDIR\locale\es\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\es.gmo"
SetOutPath "$INSTDIR\locale\et\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\et_EE.gmo"
SetOutPath "$INSTDIR\locale\fi\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\fi.gmo"
SetOutPath "$INSTDIR\locale\fr\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\fr.gmo"
SetOutPath "$INSTDIR\locale\fy\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\fy.gmo"
SetOutPath "$INSTDIR\locale\ga\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\ga.gmo"
SetOutPath "$INSTDIR\locale\hr\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\hr.gmo"
SetOutPath "$INSTDIR\locale\hu\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\hu.gmo"
SetOutPath "$INSTDIR\locale\it\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\it.gmo"
SetOutPath "$INSTDIR\locale\ko\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\ko.gmo"
SetOutPath "$INSTDIR\locale\la\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\la.gmo"
SetOutPath "$INSTDIR\locale\lt\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\lt.gmo"
SetOutPath "$INSTDIR\locale\nb\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\nb.gmo"
SetOutPath "$INSTDIR\locale\nl\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\nl.gmo"
SetOutPath "$INSTDIR\locale\pl\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\pl.gmo"
SetOutPath "$INSTDIR\locale\pt_BR\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\pt_BR.gmo"
SetOutPath "$INSTDIR\locale\pt\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\pt.gmo"
SetOutPath "$INSTDIR\locale\ro\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\ro.gmo"
SetOutPath "$INSTDIR\locale\ru\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\ru.gmo"
SetOutPath "$INSTDIR\locale\sk\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\sk.gmo"
SetOutPath "$INSTDIR\locale\sl\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\sl.gmo"
SetOutPath "$INSTDIR\locale\tr\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\tr.gmo"
SetOutPath "$INSTDIR\locale\uk\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\uk_UA.gmo"
SetOutPath "$INSTDIR\locale\zh_TW\LC_MESSAGES"
File "/oname=${PACKAGE}.mo" "${TOP_SRCDIR}\po\zh_TW.gmo"
SetOutPath "$INSTDIR\locale\zh_CN\LC_MESSAGES"
File "${TOP_SRCDIR}\po\zh_CN.gmo"
SectionEnd

Section /o "Download Videos"
  SectionIn 1
   AddSize 173670
   NSISdl::download "http://cloud.github.com/downloads/Subsentient/wz2100legacy/sequences.wzl" "$INSTDIR\sequences.wzl"
SectionEnd

;Make it easy to migrate our stuff.
Section /o "Migrate maps and ranks from Warzone 2100 2.3"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\maps\"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\players\"
 CopyFiles "$DOCUMENTS\Warzone 2100 2.3\maps\*.*" "$DOCUMENTS\Warzone 2100 Legacy Microwave\maps\"
 CopyFiles "$DOCUMENTS\Warzone 2100 2.3\multiplay\players\*.*" "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\players\"
SectionEnd

Section /o "Migrate maps and ranks from Warzone 2100 3.1"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\maps\"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\"
 CreateDirectory "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\players\"
 CopyFiles "$DOCUMENTS\Warzone 2100 3.1\maps\*.*" "$DOCUMENTS\Warzone 2100 Legacy Microwave\maps\"
 CopyFiles "$DOCUMENTS\Warzone 2100 3.1\multiplay\players\*.*" "$DOCUMENTS\Warzone 2100 Legacy Microwave\multiplay\players\"
SectionEnd

Function .onInit ;Splash display.
SetOutPath $TEMP
  File /oname=wzlsplash.bmp "${TOP_BUILDDIR}\icons\wz2100l_instalload.bmp"
  splash::show 2000 $TEMP\wzlsplash
  Delete $TEMP\wzlsplash.bmp
FunctionEnd

;--------------------------------
;Uninstall Files

Section "Uninstall"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
  Delete "$INSTDIR\wz2100legacy.exe"
  Delete "$INSTDIR\mp.wzl"
  Delete "$INSTDIR\base.wzl"
  Delete "$INSTDIR\sequences.wzl"
  Delete "$INSTDIR\AUTHORS"
  Delete "$INSTDIR\ChangeLog"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\COPYING.NONGPL"
  Delete "$INSTDIR\COPYING.README"
  Delete "$INSTDIR\music\*.*"
  RMDir	 "$INSTDIR\music"
  Delete "$INSTDIR\fonts\*.*"
  RMDir	 "$INSTDIR\fonts"
  RMDir /r "$INSTDIR\locale"
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLICATION_NAME}.lnk"
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${APPLICATION_NAME}.lnk"
  RMDir "$SMPROGRAMS\$STARTMENU_FOLDER\"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR\"

  DeleteRegKey /ifempty HKCU "Software\${APPLICATION_NAME}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}"

SectionEnd

