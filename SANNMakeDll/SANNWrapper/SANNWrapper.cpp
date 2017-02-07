// SANNWrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "SANNWrapper.h"
#include "WrappedLib.h"

SANNWrapper::SANNWrapper() : current_lib_(std::end(pool_))
{}

SANNWrapper::~SANNWrapper()
{
	pool_.clear();
}

int SANNWrapper::responseBegin(const char* dll_name)
{
	if (current_lib_ == std::end(pool_) || current_lib_->first != dll_name)
	{
		current_lib_ = pool_.find(dll_name);
		if (current_lib_ == std::end(pool_))
		{
			try
			{
				auto pr = pool_.emplace(dll_name, std::make_shared<WrappedLib>(dll_name));
				current_lib_ = pr.first;
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
				return -1;
			}
		}
	}
	return current_lib_->second->NSAdapterResponseBegin();
}

int SANNWrapper::responseAddInput(double value)
{
	if (current_lib_ == std::end(pool_))
	{
		std::cerr << "NSAdapterResponseBegin wasn't called or unsuccessfull" << std::endl;
		return -1;
	}
	return current_lib_->second->NSAdapterResponseAddInput(value);
}

int SANNWrapper::responseCall()
{
	if (current_lib_ == std::end(pool_))
	{
		std::cerr << "NSAdapterResponseBegin wasn't called or unsuccessfull" << std::endl;
		return -1;
	}
	return current_lib_->second->NSAdapterResponseCall();
}

double SANNWrapper::responseOutput(int index)
{
	if (current_lib_ == std::end(pool_))
	{
		std::cerr << "NSAdapterResponseBegin wasn't called or unsuccessfull" << std::endl;
		return -1;
	}
	return current_lib_->second->NSAdapterResponseGetOutput(index);
}

//---------------------------------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) int NSAdapterResponseBegin(const char* dll_name)
{
	return g_wrapper->responseBegin(dll_name);
}

extern "C" __declspec(dllexport) int NSAdapterResponseAddInput(double value)
{
	return g_wrapper->responseAddInput(value);
}

extern "C" __declspec(dllexport) int NSAdapterResponseCall()
{
	return g_wrapper->responseCall();
}

extern "C" __declspec(dllexport) double NSAdapterResponseGetOutput(int index)
{
	return g_wrapper->responseOutput(index);
}
