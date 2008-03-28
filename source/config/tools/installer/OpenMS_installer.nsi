# Auto-generated by EclipseNSIS Script Wizard
# 02-Mar-2008 23:59:47

Name "OpenMS"


# edit this to suit your system
 #path to QT libs
!define QTLIBDIR "C:\Qt\4.3.4\bin"
!define CONTRIBDIR "D:\uni\OpenMS_Win\my\contrib"
!define OPENMSDIR "..\..\..\.."


# Defines
!define REGKEY "SOFTWARE\$(^Name)"
!define VERSION 1.1
#!define COMPANY "Free University of Berlin"
!define URL http://www.open-ms.de

# MUI defines
!define MUI_ICON OpenMS.ico
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY ${REGKEY}
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER OpenMS
!define MUI_LICENSEPAGE_RADIOBUTTONS
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_LANGDLL_REGISTRY_ROOT HKLM
!define MUI_LANGDLL_REGISTRY_KEY ${REGKEY}
!define MUI_LANGDLL_REGISTRY_VALUENAME InstallerLanguage

# Included files
!include Sections.nsh
!include MUI.nsh
!include Library.nsh
!define ALL_USERS
!include IncludeScript_WriteEnvStr.nsh

# Reserved Files
!insertmacro MUI_RESERVEFILE_LANGDLL
ReserveFile "${NSISDIR}\Plugins\AdvSplash.dll"

# Variables
Var StartMenuGroup

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${OPENMSDIR}\License.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE German
!insertmacro MUI_LANGUAGE English
!insertmacro MUI_LANGUAGE French

# Installer attributes
OutFile OpenMS_setup.exe
InstallDir $PROGRAMFILES\OpenMS
CRCCheck on
XPStyle on
ShowInstDetails hide
VIProductVersion 1.1.0.0
VIAddVersionKey /LANG=${LANG_GERMAN} ProductName "OpenMS"
VIAddVersionKey /LANG=${LANG_GERMAN} ProductVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_GERMAN} CompanyName "${COMPANY}"
VIAddVersionKey /LANG=${LANG_GERMAN} CompanyWebsite "${URL}"
VIAddVersionKey /LANG=${LANG_GERMAN} FileVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_GERMAN} FileDescription ""
VIAddVersionKey /LANG=${LANG_GERMAN} LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails hide


Section TESTING
System::Alloc 32
Pop $1
System::Call "Kernel32::GlobalMemoryStatus(i) v (r1)"
System::Call "*$1(&i4 .r2, &i4 .r3, &i4 .r4, &i4 .r5, \
                  &i4 .r6, &i4.r7, &i4 .r8, &i4 .r9)"
System::Free $1
DetailPrint "Structure size (useless): $2 Bytes"
DetailPrint "Memory load: $3%"
DetailPrint "Total physical memory: $4 Bytes"
## do something with that!!

DetailPrint "Free physical memory: $5 Bytes"
DetailPrint "Total page file: $6 Bytes"
DetailPrint "Free page file: $7 Bytes"
DetailPrint "Total virtual: $8 Bytes"
DetailPrint "Free virtual: $9 Bytes"

SectionEnd

!macro FOLDER_LIST_RECURSIVE INCLUDE_DIR EXCLUDE_FILES 
  !system "FolderList.exe /r $\"${INCLUDE_DIR} $\" $\"${EXCLUDE_FILES}$\"" 
  !include "FolderList.exe.txt" 
!macroend

# Installer sections
Section -lib SEC0000
    SetOutPath $INSTDIR\lib
    SetOverwrite on
    File "${CONTRIBDIR}\lib\libxerces-c2_7_0.dll"
    
    # Installing library ..\OpenMS\lib\libOpenMS.dll
    # old: # File D:\uni\OpenMS_Win\my\OpenMS\lib\libOpenMS.dll
    !insertmacro InstallLib REGDLL 1 REBOOT_PROTECTED ${OPENMSDIR}\lib\libOpenMS.dll $INSTDIR\lib\libOpenMS.dll $INSTDIR\lib
    
    File "${QTLIBDIR}\QtCore4.dll"
    File "${QTLIBDIR}\QtGui4.dll"
    File "${QTLIBDIR}\QtNetwork4.dll"
    File "${QTLIBDIR}\QtOpenGL4.dll"
    File "${QTLIBDIR}\QtSql4.dll"
    File "${QTLIBDIR}\mingwm10.dll"
    WriteRegStr HKLM "${REGKEY}\Components" lib 1

    
    SetOutPath $INSTDIR\share
    SetOverwrite on
    !insertmacro FOLDER_LIST_RECURSIVE "${OPENMSDIR}\share\*.*" ".svn\"
    
SectionEnd

Section -license SEC0008
    SetOutPath $INSTDIR
    SetOverwrite on
    File "${OPENMSDIR}\License.gpl-2.0.txt"
    File "${OPENMSDIR}\License.lgpl-2.1.txt"
    File "${OPENMSDIR}\License.libSVM.txt"
    File "${OPENMSDIR}\License.NetCDF.txt"
    
SectionEnd


Section "-TOPP Tools" SEC0001
    SetOutPath $INSTDIR\TOPP
    SetOverwrite on
    File /r /x TOPPView.exe /x INIFileEditor.exe ${OPENMSDIR}\bin\*.exe
    WriteRegStr HKLM "${REGKEY}\Components" "TOPP Tools" 1
SectionEnd

!macro CREATE_SMGROUP_SHORTCUT NAME PATH
    Push "${NAME}"
    Push "${PATH}"
    Call CreateSMGroupShortcut
!macroend


Section create_command_bat
    StrCpy $R0 $INSTDIR 2
    fileOpen $0 "$INSTDIR\TOPP\command.bat" w
    fileWrite $0 "$SYSDIR\cmd.exe /k $\"$R0 && cd $INSTDIR\TOPP && cls && echo on && echo Welcome to the OpenMS TOPP tools command line && echo type 'dir' to see available commands && echo on $\""
    fileClose $0
SectionEnd

Section -GUI SEC0002
    SetOutPath $INSTDIR\GUI
    SetOverwrite on
    File ${OPENMSDIR}\bin\TOPPView.exe
    File ${OPENMSDIR}\bin\INIFileEditor.exe
    !insertmacro CREATE_SMGROUP_SHORTCUT TOPPView $INSTDIR\GUI\TOPPView.exe
    !insertmacro CREATE_SMGROUP_SHORTCUT INIFileEditor $INSTDIR\GUI\INIFileEditor.exe
    !insertmacro CREATE_SMGROUP_SHORTCUT "OpenMS Homepage" http://www.open-ms.de/
    !insertmacro CREATE_SMGROUP_SHORTCUT "TOPP command line" "$INSTDIR\TOPP\command.bat"
    
    WriteRegStr HKLM "${REGKEY}\Components" GUI 1
SectionEnd

Section PathInst
  Push "$INSTDIR\lib"
  Call AddToPath
  Push "$INSTDIR\TOPP"
  Call AddToPath
  #add OpenMS data path (for shared xml files etc)
  Push "OPENMS_DATA_PATH"
  Push "$INSTDIR\share\OpenMS"
  Call AddToEnvVar
SectionEnd

Section -post SEC0003
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\$(^UninstallLink).lnk" $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    #WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\uninstall.exe
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKLM "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
!macro DELETE_SMGROUP_SHORTCUT NAME
    Push "${NAME}"
    Call un.DeleteSMGroupShortcut
!macroend

Section /o -un.GUI UNSEC0002
    !insertmacro DELETE_SMGROUP_SHORTCUT "TOPP command line"
    !insertmacro DELETE_SMGROUP_SHORTCUT "OpenMS Homepage"
    !insertmacro DELETE_SMGROUP_SHORTCUT INIFileEditor
    !insertmacro DELETE_SMGROUP_SHORTCUT TOPPView
    Delete /REBOOTOK $INSTDIR\GUI\INIFileEditor.exe
    Delete /REBOOTOK $INSTDIR\GUI\TOPPView.exe
    
    Delete /REBOOTOK  $INSTDIR\TOPP\command.bat
    
    DeleteRegValue HKLM "${REGKEY}\Components" GUI
SectionEnd

Section /o "-un.TOPP Tools" UNSEC0001
    RmDir /r /REBOOTOK $INSTDIR\TOPP
    DeleteRegValue HKLM "${REGKEY}\Components" "TOPP Tools"
SectionEnd


Section /o -un.PathInst UNSEC0010
  Push "$INSTDIR\lib"
  Call un.RemoveFromPath
  Push "$INSTDIR\TOPP"
  Call un.RemoveFromPath
  Push "OPENMS_DATA_PATH"
  Push "$INSTDIR\share\OpenMS"
  Call un.RemoveFromEnvVar
SectionEnd

Section /o -un.license UNSEC0008
    Delete /REBOOTOK "$INSTDIR\License.gpl-2.0.txt"
    Delete /REBOOTOK "$INSTDIR\License.lgpl-2.1.txt"
    Delete /REBOOTOK "$INSTDIR\License.libSVM.txt"
    Delete /REBOOTOK "$INSTDIR\License.NetCDF.txt"
    
SectionEnd
  
Section /o -un.lib UNSEC0000
    Delete /REBOOTOK $INSTDIR\lib\mingwm10.dll
    Delete /REBOOTOK $INSTDIR\lib\QtSql4.dll
    Delete /REBOOTOK $INSTDIR\lib\QtOpenGL4.dll
    Delete /REBOOTOK $INSTDIR\lib\QtNetwork4.dll
    Delete /REBOOTOK $INSTDIR\lib\QtGui4.dll
    Delete /REBOOTOK $INSTDIR\lib\QtCore4.dll
        
    # old: # Delete /REBOOTOK $INSTDIR\lib\libOpenMS.dll
    !insertmacro UnInstallLib REGDLL SHARED REBOOT_PROTECTED $INSTDIR\lib\libOpenMS.dll
    
    Delete /REBOOTOK $INSTDIR\lib\libxerces-c2_7_0.dll
    RmDir /r /REBOOTOK $INSTDIR\lib
    
    RmDir /r /REBOOTOK ${OPENMSDIR}\share
    
    DeleteRegValue HKLM "${REGKEY}\Components" lib
SectionEnd

Section -un.post UNSEC0003
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$(^UninstallLink).lnk"
    Delete /REBOOTOK $INSTDIR\uninstall.exe
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
    Push $R0
    StrCpy $R0 $StartMenuGroup 1
    StrCmp $R0 ">" no_smgroup
no_smgroup:
    Pop $R0
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
    Push $R1
    File /oname=$PLUGINSDIR\spltmp.bmp OpenMS_splash.bmp
    advsplash::show 1000 600 400 -1 $PLUGINSDIR\spltmp
    Pop $R1
    Pop $R1
    !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function CreateSMGroupShortcut
    Exch $R0 ;PATH
    Exch
    Exch $R1 ;NAME
    Push $R2
    StrCpy $R2 $StartMenuGroup 1
    StrCmp $R2 ">" no_smgroup
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    StrCpy $OUTDIR "$INSTDIR"       # execute link target in $OUTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\$R1.lnk" $R0
no_smgroup:
    Pop $R2
    Pop $R1
    Pop $R0
FunctionEnd

# Uninstaller functions
Function un.onInit
    ReadRegStr $INSTDIR HKLM "${REGKEY}" Path
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
    !insertmacro MUI_UNGETLANGUAGE
    !insertmacro SELECT_UNSECTION lib ${UNSEC0000}
    !insertmacro SELECT_UNSECTION "TOPP Tools" ${UNSEC0001}
    !insertmacro SELECT_UNSECTION GUI ${UNSEC0002}
FunctionEnd

Function un.DeleteSMGroupShortcut
    Exch $R1 ;NAME
    Push $R2
    StrCpy $R2 $StartMenuGroup 1
    StrCmp $R2 ">" no_smgroup
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$R1.lnk"
no_smgroup:
    Pop $R2
    Pop $R1
FunctionEnd

# Installer Language Strings
# TODO Update the Language Strings with the appropriate translations.

LangString ^UninstallLink ${LANG_GERMAN} "Uninstall $(^Name)"
LangString ^UninstallLink ${LANG_ENGLISH} "Uninstall $(^Name)"
LangString ^UninstallLink ${LANG_FRENCH} "Uninstall $(^Name)"


