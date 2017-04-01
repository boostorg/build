# Copyright 2017 - Refael Ackermann
# Copyright 2017 - Arkady Shapkin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

Param(
    [Parameter(Mandatory=$True,Position=1)]
    [string]$toolsetVersion,
    [switch]$pathToCompiler = $false
)

# Is Microsoft.VisualStudio.Setup.Configuration.Native.dll installed
if (-NOT (Test-Path 'Registry::HKEY_CLASSES_ROOT\CLSID\{177F0C4A-1CD3-4DE7-A32C-71DBBB9FA36D}')) { Exit 1 }

try
{
    Add-Type -Path (Join-Path $PSScriptRoot GetVS2017Configuration.cs) | Out-Null
    $instPath = [VisualStudioConfiguration.Main]::Query($toolsetVersion)
    if ($pathToCompiler)
    {
        if ($env:PROCESSOR_ARCHITEW6432 -ne $null) {$filt = '*64\x64*'} else {$filt = '*86\x86*'}
        $cls = get-childitem $instPath -Include cl.exe -Recurse | ? { $_.Directory -like '*Host*' }
        $cl = $cls | ? { $_.Directory -like $filt }
        if ($cl -ne $null)
        {
            Write-Output $cl.FullName
        }
        else
        {
            Write-Output $cls[0].FullName
        }
    }
    else
    {
        Write-Output $instPath
    }
}
catch
{
    # This scipt shouldn't return any ouput on error
    Exit 2
}
