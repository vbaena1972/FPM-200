$ErrorActionPreference = 'Stop'
$testRoot = $PSScriptRoot
$repoFirmware = Split-Path (Split-Path $testRoot)
$testBuild = Join-Path $repoFirmware 'build/host-tests'
New-Item -ItemType Directory -Force -Path $testBuild | Out-Null
$vswhere = 'C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe'
$vsRoot = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$dev = Join-Path $vsRoot 'Common7/Tools/Launch-VsDevShell.ps1'
& $dev -Arch amd64 -HostArch amd64 -SkipAutomaticLocation | Out-Null
& cl.exe /nologo /std:c11 /W3 "/I$testRoot/stubs" "/I$repoFirmware/components/drivers/include" "$testRoot/test_eeprom.c" "$repoFirmware/components/drivers/at24c256.c" "/Fo$testBuild/" "/Fe$testBuild/test_eeprom.exe"
if ($LASTEXITCODE -ne 0) { throw 'Host compilation failed' }
& "$testBuild/test_eeprom.exe"
if ($LASTEXITCODE -ne 0) { throw 'Host test failed' }

& cl.exe /nologo /std:c11 /W3 "/I$testRoot/stubs" "/I$repoFirmware/components/drivers/include" "$testRoot/test_alarm.c" "$repoFirmware/components/drivers/alarm_mgr.c" "/Fo$testBuild/" "/Fe$testBuild/test_alarm.exe"
if ($LASTEXITCODE -ne 0) { throw 'Alarm test compilation failed' }
& "$testBuild/test_alarm.exe"
if ($LASTEXITCODE -ne 0) { throw 'Alarm test failed' }

& cl.exe /nologo /std:c11 /W3 "/I$repoFirmware/components/sensors_runtime/include" "$testRoot/test_flow_integrator.c" "/Fo$testBuild/" "/Fe$testBuild/test_flow_integrator.exe"
if ($LASTEXITCODE -ne 0) { throw 'Flow integrator test compilation failed' }
& "$testBuild/test_flow_integrator.exe"
if ($LASTEXITCODE -ne 0) { throw 'Flow integrator test failed' }
