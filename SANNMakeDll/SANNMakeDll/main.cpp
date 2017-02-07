#include "stdafx.h"
#include <iostream>
#include <algorithm> // copy
#include <iterator> // istreambuf_iterator, ostreambuf_iterator, back_inserter
#include <fstream> // wofstream
#include "SANNMakeDll.h"

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		std::wstring contents;
		std::copy(std::istreambuf_iterator<wchar_t>(std::wcin), std::istreambuf_iterator<wchar_t>(),
			std::back_inserter(contents));

		SANNMakeDll().convert(contents);

		std::copy(std::begin(contents), std::end(contents), std::ostreambuf_iterator<wchar_t>(std::wcout));

		//std::wofstream target_file(L".\\SANNDll\\sann.cpp");
		//target_file << contents;
		//target_file.close();
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	return 0;
}
