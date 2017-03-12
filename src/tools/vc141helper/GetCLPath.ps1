# Copyright 2017 - Refael Ackermann
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
if (-NOT (Test-Path 'Registry::HKEY_CLASSES_ROOT\CLSID\{177F0C4A-1CD3-4DE7-A32C-71DBBB9FA36D}')) { Exit 1 }
$jsonFile = '.tmp.json'
Invoke-Expression 'powershell -NoProfile -ExecutionPolicy Unrestricted -Command "&{ Add-Type -Path GetVS2017Configuration.cs; [VisualStudioConfiguration.Main]::Query()}"' > $jsonFile
$instPath = (Get-Content $jsonFile | ? {$_ -like "*InstallationPath*"}) -split '"' -replace '\\\\', '\' | Select-Object -skip 3 -first 1
if ($args[0] -eq 'InstallationPath') { echo $instPath; exit }
if ($env:PROCESSOR_ARCHITEW6432 -ne $null) {$filt = '*64\x64*'} else {$filt = '*86\x86*'}
$cls = get-childitem $instPath -Include cl.exe -Recurse | ? { $_.Directory -like '*Host*' }
$cl = $cls | ? { $_.Directory -like $filt }
if ($cl -ne $null) {echo $cl.FullName} else {echo $cls[0].FullName}
