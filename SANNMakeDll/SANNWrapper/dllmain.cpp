// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "SANNWrapper.h"
#include <memory>

std::shared_ptr<SANNWrapper> g_wrapper;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_wrapper = std::make_shared<SANNWrapper>();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		g_wrapper.reset();
		break;
	}
	return TRUE;
}