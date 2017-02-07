#pragma once
#include <windows.h>

typedef int(*NSAdapterResponseBeginT)();
typedef int(*NSAdapterResponseAddInputT)(double);
typedef int(*NSAdapterResponseCallT)();
typedef double(*NSAdapterResponseGetOutputT)(int);

class WrappedLib
{
	HMODULE lib_ = nullptr;

public:
	WrappedLib() = delete;
	WrappedLib(const WrappedLib&) = delete;
	WrappedLib& operator =(const WrappedLib&) = delete;
	WrappedLib(WrappedLib&&) = delete;
	WrappedLib& operator =(WrappedLib&&) = delete;

	NSAdapterResponseBeginT NSAdapterResponseBegin = nullptr;
	NSAdapterResponseAddInputT NSAdapterResponseAddInput = nullptr;
	NSAdapterResponseCallT NSAdapterResponseCall = nullptr;
	NSAdapterResponseGetOutputT NSAdapterResponseGetOutput = nullptr;

public:
	WrappedLib(const char* filename);
	~WrappedLib();
};
