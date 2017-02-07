#pragma once
#include <string>

class SANNMakeDll
{
	enum class CallType
	{
		Regression, Classification
	};

	int			in_size_ = 0;
	int			out_size_ = 0;
	CallType	call_type_ = CallType::Regression;

	std::wstring _extractVarpart(const std::wstring& contents) const;
	void _dropMain(std::wstring& contents) const;
	void _eraseVarPart(std::wstring& contents) const;
	void _findInOutSizes(std::wstring& contents);
	void _findCallType(std::wstring& contents);
	void _addDllCode(std::wstring& contents) const;
public:
	void convert(std::wstring& contents);
};