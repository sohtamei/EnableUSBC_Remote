xcopy .\RemoteCli\external\crsdk\* .\RemoteCli\Debug\ /E /Y
xcopy .\RemoteCli\external\crsdk\* .\RemoteCli\Release\ /E /Y
xcopy .\RemoteCli\Debug\* .\appRemote\bin\Debug\net8.0-windows\ /E /Y
xcopy .\RemoteCli\Release\* .\appRemote\bin\Release\net8.0-windows\ /E /Y
timeout 3 > nul
