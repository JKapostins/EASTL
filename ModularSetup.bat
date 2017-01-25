@echo off
set "LocalPath=%~dp0\Local"
set "PackagePath=%LocalPath%\Packages"
set "EngineCMakeScriptsPath=%PackagePath%\Engine.CMake.Scripts"
set "EntryPointPath=%PackagePath%\Engine.Main"

rem Create local directory and pull down the windows build scripts
if not exist %LocalPath% (
mkdir %LocalPath% 
cd %LocalPath%
call git init
if not "%1" == "ssh" (
call git remote add origin https://github.com/JKapostins/Engine.WindowsBuildScripts.git
) else (
call git remote add origin git@github.com:JKapostins/Engine.WindowsBuildScripts.git
)
call git pull origin master
)

if not exist %PackagePath% (
mkdir %PackagePath% 
)

rem Create a folder for the common cmake scripts and pull them down.
if not exist %EngineCMakeScriptsPath% (
mkdir %EngineCMakeScriptsPath%
cd %EngineCMakeScriptsPath%
call git init
if not "%1" == "ssh" (
call git remote add origin https://github.com/JKapostins/Engine.CMake.Scripts.git
) else (
call git remote add origin git@github.com:JKapostins/Engine.CMake.Scripts.git
)
call git pull origin master
)

rem Add the cross platform entry point code. This should be used for all executables.
if not exist %EntryPointPath% (
mkdir %EntryPointPath%
cd %EntryPointPath%
call git init
if not "%1" == "ssh" (
call git remote add origin https://github.com/JKapostins/Engine.Main.git
) else (
call git remote add origin git@github.com:JKapostins/Engine.Main.git
)
call git pull origin master
)