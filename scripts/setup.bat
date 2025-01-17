@echo off

pushd ..
Walnut\vendor\bin\premake\Windows\premake5.exe --file=build-client.lua vs2022
Walnut\vendor\bin\premake\Windows\premake5.exe --file=build-server.lua vs2022
popd

if %errorlevel% neq 0 (
	pause
)