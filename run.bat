::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
: run.bat
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: server Command: port
start "" "Debug\ServerMockChannel.exe" 8081

start "" "Debug\ServerMockChannel.exe" 8082

:: client1
start "" "Debug\WPFClient.exe"

:: client2
start "" "Debug\WPFClient.exe"