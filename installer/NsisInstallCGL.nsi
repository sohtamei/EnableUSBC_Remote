# http://kwikwi.cocolog-nifty.com/blog/2010/04/chapter4.html

# 日本語UI
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Japanese.nlf"
Unicode True

Name "Enable USBC Remote"

OutFile "EnableUSBC_Remote_Installer.exe"

# インストール先
InstallDir "$DESKTOP\EnableUSBC_Remote"

# ページの定義
Page directory
Page instfiles

# デフォルト セクション
Section
  # 出力先
  SetOutPath "$INSTDIR"
  # インストール対象
  File /r "EnableUSBC_Remote\*"
  #WriteUninstaller "$INSTDIR\Uninstall.exe"

  ExecWait "$INSTDIR/Driver/dpinst-amd64.exe"

  Exec "$INSTDIR/VC_redist.x64.exe /install /quiet /norestart"
SectionEnd

# アンインストーラ
#Section "Uninstall"
  # アンインストーラを削除
  #Delete "$INSTDIR/Uninstall.exe"
  # ディレクトリを削除
  #RMDir /r "$INSTDIR"
#SectionEnd
