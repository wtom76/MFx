// DllTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

constexpr int g_output_size = 1;

typedef int (*NSAdapterResponseBeginT)(const char*);
typedef int (*NSAdapterResponseAddInputT)(double);
typedef int (*NSAdapterResponseCallT)();
typedef double (*NSAdapterResponseGetOutputT)(int);

class Lib
{
	HMODULE lib_ = nullptr;

public:
	NSAdapterResponseBeginT NSAdapterResponseBegin = nullptr;
	NSAdapterResponseAddInputT NSAdapterResponseAddInput = nullptr;
	NSAdapterResponseCallT NSAdapterResponseCall = nullptr;
	NSAdapterResponseGetOutputT NSAdapterResponseGetOutput = nullptr;

public:
	Lib(const char* filename);
	~Lib();
};

Lib::Lib(const char* filename)
{
	HMODULE lib = ::LoadLibraryA(filename);
	if (!lib)
	{
		std::string msg = std::string("Can't load ") + filename;
		throw std::exception(msg.c_str());
	}

	NSAdapterResponseBegin = (NSAdapterResponseBeginT)(::GetProcAddress(lib, "NSAdapterResponseBegin"));
	NSAdapterResponseAddInput = (NSAdapterResponseAddInputT)::GetProcAddress(lib, "NSAdapterResponseAddInput");
	NSAdapterResponseCall = (NSAdapterResponseCallT)::GetProcAddress(lib, "NSAdapterResponseCall");
	NSAdapterResponseGetOutput = (NSAdapterResponseGetOutputT)::GetProcAddress(lib, "NSAdapterResponseGetOutput");

	if (!NSAdapterResponseBegin || !NSAdapterResponseAddInput || !NSAdapterResponseCall || !NSAdapterResponseGetOutput)
	{
		throw std::exception("Can't load proc addresses");
	}
}

Lib::~Lib()
{
	if (lib_)
	{
		::CloseHandle(lib_);
	}
}

int main()
{
	Lib lib("SANNWrapper.dll");

	std::vector<double> values;

	std::wstring str;
	std::getline(std::wcin, str);
	const size_t value_count = std::count(std::begin(str), std::end(str), L',');

	values.resize(value_count, 0.);

	while (std::getline(std::wcin, str))
	{
		std::replace(std::begin(str), std::end(str), L',', L' ');
		std::wistringstream strm(str);
		std::copy_n(std::istream_iterator<double, wchar_t>(strm), values.size(), std::begin(values));

		//----

		(*lib.NSAdapterResponseBegin)("..\\..\\sources\\dlls\\SANN_C_Code_brentD-1.dll");

		for (int i = 0; i < values.size() - 4; ++i)
		{
			(*lib.NSAdapterResponseAddInput)(values[i]);
		}

		(*lib.NSAdapterResponseCall)();

		for (int i = 0; i < values.size(); ++i)
		{
			std::wcout << values[i] << L'\t';
		}
		for (int i = 0; i < g_output_size; ++i)
		{
			std::wcout << (*lib.NSAdapterResponseGetOutput)(i) << L'\t';
		}
		std::wcout << std::endl;

		//----

		(*lib.NSAdapterResponseBegin)("..\\..\\sources\\dlls\\SANN_C_Code_brentD-6");

		for (int i = 0; i < values.size() - 4; ++i)
		{
			(*lib.NSAdapterResponseAddInput)(values[i]);
		}

		(*lib.NSAdapterResponseCall)();

		for (int i = 0; i < values.size(); ++i)
		{
			std::wcout << values[i] << L'\t';
		}
		for (int i = 0; i < g_output_size; ++i)
		{
			std::wcout << (*lib.NSAdapterResponseGetOutput)(i) << L'\t';
		}
		std::wcout << std::endl;
	}

    return 0;
}
