// SANNMakeDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <exception>
#include <wchar.h>
#include <string>
#include "SANNMakeDll.h"

extern std::wstring dll_code_template_top;
extern std::wstring dll_code_template_bottom;
extern std::wstring dll_code_template_main_regression;
extern std::wstring dll_code_template_main_classification;

std::wstring SANNMakeDll::_extractVarpart(const std::wstring& contents) const
{
	static const std::wstring end_anchor = L"input_hidden_weights[";
	static const std::wstring begin_anchor = L"double ";

	const std::wstring::size_type endpos = contents.find(end_anchor);
	if (endpos == std::wstring::npos)
	{
		throw std::exception("Can't find \"input_hidden_weights[\"");
	}
	const std::wstring::size_type pos = contents.rfind(begin_anchor, endpos);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"double ...input_hidden_weights[\"");
	}

	return contents.substr(pos + begin_anchor.size(), endpos - pos - begin_anchor.size());
}

void SANNMakeDll::_dropMain(std::wstring& contents) const
{
	static const std::wstring anchor = L"int main()";
	const std::wstring::size_type mainpos = contents.find(anchor);
	if (mainpos == std::wstring::npos)
	{
		throw std::exception("Can't find \"int main()\"");
	}
	std::wstring::size_type endpos = mainpos + anchor.size();
	while (endpos < contents.size() && contents[endpos] != L'{')
	{
		++endpos;
	}
	if (endpos >= contents.size())
	{
		throw std::exception("Can't find main() body");
	}
	// Warning! Brackets in strings or comments will break the convertion.
	int bracket_depth = 1;
	while (endpos < contents.size() && bracket_depth > 0)
	{
		if (contents[endpos] == L'{')
		{
			++bracket_depth;
		}
		else if (contents[endpos] == L'}')
		{
			--bracket_depth;
		}
		++endpos;
	}
	contents.erase(mainpos, endpos - mainpos);
}

void SANNMakeDll::_eraseVarPart(std::wstring& contents) const
{
	const std::wstring varpart = _extractVarpart(contents);
	if (varpart.empty())
	{
		return;
	}
	std::wstring::size_type pos = contents.find(varpart);
	while (pos != std::wstring::npos)
	{
		contents.erase(pos, varpart.size());
		pos = contents.find(varpart, pos);
	}
}

void SANNMakeDll::_findInOutSizes(std::wstring& contents)
{
	in_size_ = out_size_ = 0;
	static const std::wstring in_anchor = L"double input[";
	static const std::wstring out_anchor = L"double output[";
	std::wstring::size_type pos = contents.find(in_anchor);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"double input[\"");
	}
	in_size_ = _wtoi(contents.c_str() + pos + in_anchor.size());

	pos = contents.find(out_anchor);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"double output[\"");
	}
	out_size_ = _wtoi(contents.c_str() + pos + out_anchor.size());
}

void SANNMakeDll::_findCallType(std::wstring& contents)
{
	const size_t rm_pos = contents.find(L"RunNeuralNet_Regression");
	const size_t cl_pos = contents.find(L"RunNeuralNet_Classification");

	if (rm_pos == std::wstring::npos && cl_pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"RunNeuralNet_*\"");
	}
	if (rm_pos != std::wstring::npos && cl_pos != std::wstring::npos)
	{
		throw std::exception("Panic: both \"RunNeuralNet_Regression\" and \"RunNeuralNet_Classification\" found.");
	}
	if (rm_pos != std::wstring::npos)
	{
		call_type_ = CallType::Regression;
	}
	else if (cl_pos != std::wstring::npos)
	{
		call_type_ = CallType::Classification;
	}
}

void SANNMakeDll::_addDllCode(std::wstring& contents) const
{
	static const std::wstring in_anchor = L"#in_size#";
	static const std::wstring out_anchor = L"#out_size#";
	static const std::wstring main_anchor = L"#main#";

	contents = dll_code_template_top + contents;

	// dd_code
	std::wstring dd_code = dll_code_template_bottom;
	const std::wstring in_size_str = std::to_wstring(in_size_);
	const std::wstring out_size_str = std::to_wstring(out_size_);

	std::wstring::size_type pos = dd_code.find(in_anchor);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"#in_size#\"");
	}
	dd_code.replace(pos, in_anchor.size(), in_size_str);

	pos = dd_code.find(out_anchor);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"#out_size#\"");
	}
	dd_code.replace(pos, out_anchor.size(), out_size_str);

	pos = dd_code.find(main_anchor);
	if (pos == std::wstring::npos)
	{
		throw std::exception("Can't find \"#main#\"");
	}
	dd_code.replace(pos, main_anchor.size(),
		call_type_ == CallType::Regression ? dll_code_template_main_regression : dll_code_template_main_classification );
	//~dd_code

	contents.append(dd_code);
}

/*
	1. Delete main.
	2. Delete variable part. Example "brentD_6_MLP_20_24_1_"
	3. Find in and out sizes.
	4. Find call type.
	5. Add dll code.
*/
void SANNMakeDll::convert(std::wstring& contents)
{
	// 1.
	_dropMain(contents);
	// 2.
	_eraseVarPart(contents);
	// 3.
	_findInOutSizes(contents);
	// 4.
	_findCallType(contents);
	// 5.
	_addDllCode(contents);
}

