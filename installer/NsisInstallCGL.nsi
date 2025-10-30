# http://kwikwi.cocolog-nifty.com/blog/2010/04/chapter4.html

# ���{��UI
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Japanese.nlf"
Unicode True

Name "Enable USBC Remote"

OutFile "EnableUSBC_Remote_Installer.exe"

# �C���X�g�[����
InstallDir "$DESKTOP\EnableUSBC_Remote"

# �y�[�W�̒�`
Page directory
Page instfiles

# �f�t�H���g �Z�N�V����
Section
  # �o�͐�
  SetOutPath "$INSTDIR"
  # �C���X�g�[���Ώ�
  File /r "EnableUSBC_Remote\*"
  #WriteUninstaller "$INSTDIR\Uninstall.exe"

  ExecWait "$INSTDIR/Driver/dpinst-amd64.exe"

  Exec "$INSTDIR/VC_redist.x64.exe /install /quiet /norestart"
SectionEnd

# �A���C���X�g�[��
#Section "Uninstall"
  # �A���C���X�g�[�����폜
  #Delete "$INSTDIR/Uninstall.exe"
  # �f�B���N�g�����폜
  #RMDir /r "$INSTDIR"
#SectionEnd
