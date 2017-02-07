#include "stdafx.h"
#include "WrappedLib.h"
#include <string>

WrappedLib::WrappedLib(const char* filename) : lib_(nullptr)
{
	lib_ = ::LoadLibraryA(filename);
	if (!lib_)
	{
		std::string msg = std::string("Can't load ") + filename;
		throw std::exception(msg.c_str());
	}

	NSAdapterResponseBegin = (NSAdapterResponseBeginT)(::GetProcAddress(lib_, "NSAdapterResponseBegin"));
	NSAdapterResponseAddInput = (NSAdapterResponseAddInputT)::GetProcAddress(lib_, "NSAdapterResponseAddInput");
	NSAdapterResponseCall = (NSAdapterResponseCallT)::GetProcAddress(lib_, "NSAdapterResponseCall");
	NSAdapterResponseGetOutput = (NSAdapterResponseGetOutputT)::GetProcAddress(lib_, "NSAdapterResponseGetOutput");

	if (!NSAdapterResponseBegin || !NSAdapterResponseAddInput || !NSAdapterResponseCall || !NSAdapterResponseGetOutput)
	{
		throw std::exception("Can't load proc addresses");
	}
}

WrappedLib::~WrappedLib()
{
	if (lib_)
	{
		::FreeLibrary(lib_);
		lib_ = nullptr;
	}
}
