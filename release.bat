xcopy .\appRemote\bin\Release\net8.0-windows\* installer\EnableUSBC_Remote\ /E /Y
xcopy installer\VC_redist.x64.exe installer\EnableUSBC_Remote\ /Y
xcopy installer\Driver\* installer\EnableUSBC_Remote\Driver\ /E /Y
timeout 3 > nul
