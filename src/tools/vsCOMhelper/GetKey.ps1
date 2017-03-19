# Copyright 2017 - Refael Ackermann
# Distributed under MIT style license
# See accompanying file LICENSE at https://github.com/node4good/windows-autoconf
# version: 1.11.1

param (
    [Parameter(Mandatory=$false)]
    [string[]]$filters = "IsVcCompatible",
    [Parameter(Mandatory=$false)]
    [string[]]$keys = "VisualCppToolsCL"
)
if (-NOT (Test-Path 'Registry::HKEY_CLASSES_ROOT\CLSID\{177F0C4A-1CD3-4DE7-A32C-71DBBB9FA36D}')) { Exit 1 }
Add-Type -Path GetVS2017Configuration.cs;
$insts = [VisualStudioConfiguration.ComSurrogate]::QueryEx()
foreach ($filter in $filters) {
    if ($filter -ne "*") {
        if ($filter -like "*=*") {
            $parts = $filter -Split "=";
            $filter = $parts[0];
            $criteria = $parts[1];
            $insts = $insts | where { $_.Get($filter) -eq $criteria }
        } else {
            $insts = $insts | where { $_.Get($filter) }
        }
    }
}
foreach ($key in $keys) {
    if ($key -eq "*") { $insts | echo } else { $insts | % { $_.Get($key) } }
}
