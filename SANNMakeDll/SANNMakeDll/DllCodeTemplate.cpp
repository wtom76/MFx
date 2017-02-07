#include "stdafx.h"
#include <string>

// added includes
std::wstring dll_code_template_top = LR"(
#include "stdafx.h"
#include <vector>
#include <iostream>
)";

// added dll implementation. contains "#main#", that should be replaced by one of dll_code_template_main
std::wstring dll_code_template_bottom = LR"(
template <int input_size_, int output_size_>
class NSNetworkDll
{
private:
	int current_in_index_;
	void responseClear();

public:
	NSNetworkDll();

	int responseBegin();
	int responseAddInput(double value);
	int responseCall();
	double responseOutput(int index);
};

//---------------------------------------------------------------------------------------------------------------------

template <int input_size_, int output_size_>
NSNetworkDll<input_size_, output_size_>::NSNetworkDll() : current_in_index_(0)
{}

template <int input_size_, int output_size_>
void NSNetworkDll<input_size_, output_size_>::responseClear()
{
	current_in_index_ = 0;
	::memset(input, 0, sizeof(input));
	::memset(output, 0, sizeof(output));
}

template <int input_size_, int output_size_>
int NSNetworkDll<input_size_, output_size_>::responseBegin()
{
	responseClear();
	return 0;
}

template <int input_size_, int output_size_>
int NSNetworkDll<input_size_, output_size_>::responseAddInput(double value)
{
	if (current_in_index_ >= input_size_)
	{
		std::cerr << "NSAdapter Error: input is full" << std::endl;
		return -1;
	}
	input[current_in_index_++] = value;
	return 0;
}

template <int input_size_, int output_size_>
int NSNetworkDll<input_size_, output_size_>::responseCall()
{	#main#
	return 0;
}

template <int input_size_, int output_size_>
double NSNetworkDll<input_size_, output_size_>::responseOutput(int index)
{
	if (index < 0 || index >= output_size_)
	{
		return 0.;
	}
	return output[index];
}

//---------------------------------------------------------------------------------------------------------------------
// The one and only application object

NSNetworkDll<#in_size#, #out_size#> g_network;

//---------------------------------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) int NSAdapterResponseBegin()
{
	return g_network.responseBegin();
}

extern "C" __declspec(dllexport) int NSAdapterResponseAddInput(double value)
{
	return g_network.responseAddInput(value);
}

extern "C" __declspec(dllexport) int NSAdapterResponseCall()
{
	return g_network.responseCall();
}

extern "C" __declspec(dllexport) double NSAdapterResponseGetOutput(int index)
{
	return g_network.responseOutput(index);
}
)";

// replaces "#main#" in dll_code_template_bottom
std::wstring dll_code_template_main_regression = LR"(
	for (int i = 0; i < input_size_; ++i)
	{
		//Substitution of missing continuous variables
		if (input[i] == -9999)
		{
			input[i] = MeanInputs[i];
		}
	}
	ScaleInputs(input, 0, 1, input_size_);
	RunNeuralNet_Regression();
	UnscaleTargets(output, 0, 1, output_size_);
)";

// replaces "#main#" in dll_code_template_bottom
std::wstring dll_code_template_main_classification = LR"(
	for (int i = 0; i < input_size_; ++i)
	{
		//Substitution of missing continuous variables
		if (input[i] == -9999)
		{
			input[i] = MeanInputs[i];
		}
	}
	ScaleInputs(input, 0, 1, input_size_);
	RunNeuralNet_Classification();
	//Normalise output if output activation is not Softmax;
	Normalise(output, output_size_);
)";
