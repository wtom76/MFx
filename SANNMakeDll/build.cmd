SANNMakeDll.exe < %1 > .\SANNDll\sann.cpp
"C:\Program Files (x86)\MSBuild\14.0\bin\amd64\msbuild.exe" .\SANNDll\SANNDll.vcxproj /property:Configuration=Release /property:Platform=x64