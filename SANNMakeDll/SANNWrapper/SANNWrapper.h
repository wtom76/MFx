#pragma once
#include <map>
#include <memory>
#include <string>

class WrappedLib;

class SANNWrapper
{
	using LibPoolT = std::map<std::string, std::shared_ptr<WrappedLib>>;

	LibPoolT pool_;
	LibPoolT::const_iterator current_lib_;

public:
	SANNWrapper();
	~SANNWrapper();

	int responseBegin(const char* dll_name);
	int responseAddInput(double value);
	int responseCall();
	double responseOutput(int index);
};

extern std::shared_ptr<SANNWrapper> g_wrapper;
