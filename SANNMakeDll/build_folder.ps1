param([Parameter(Mandatory=$true)][string]$folder)

if (!(Test-Path "$folder\dlls"))
{
    New-Item -ItemType directory -Path "$folder\dlls"
}

$files = Get-ChildItem "$folder\*.c" -File
foreach ($file in $files)
{
    write-host "Processing $file"
    write-host "    Creating sann.cpp ..." -NoNewline
    start-process -FilePath ".\\SANNMakeDll.exe" -RedirectStandardInput $file.FullName -RedirectStandardOutput ".\\SANNDll\\sann.cpp" -Wait -NoNewWindow
    write-host "done"

    write-host "    Building SANNDll.dll ..." -NoNewline
    start-process -FilePath "C:\Program Files (x86)\MSBuild\14.0\bin\amd64\msbuild.exe" -ArgumentList ".\SANNDll\SANNDll.vcxproj /property:Configuration=Release /property:Platform=x64" -Wait
    write-host "done"

    $dll_name = "$folder\dlls\" + $file.BaseName + ".dll"
    write-host "    Copying SANNDll.dll to $dll_name ..." -NoNewline
    Move-Item -Path "SANNDll.dll" -Destination $dll_name -Force
    write-host "done"
}
