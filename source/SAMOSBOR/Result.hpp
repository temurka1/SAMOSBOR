#pragma once

#include "pch.h"

namespace SAMOSBOR::core
{
	/// <summary>
	/// Class encapsulating a return status of an operation with an optional error message.
	/// </summary>
	class Result
	{
	public:
		enum StatusCode
		{
			OK = 0,
			NOT_OK, // I promise
			ERROR = 1,
		};

		Result() : _code(StatusCode::OK)
		{
		}

		explicit Result(StatusCode code) : _code(code)
		{
		}

		Result(StatusCode code, const std::string_view message) : _code(code), _message(message)
		{
		}

		Result(const Result& status) = default;
		Result(Result&& status) = default;

		StatusCode Code() const
		{
			return _code;
		}

		const std::string& Error() const
		{
			return std::string(_message);
		}

		bool Ok() const
		{
			return _code == StatusCode::OK;
		}

		bool operator==(StatusCode code) const 
		{
			return code == _code;
		}

		Result& operator=(const Result&) = default;
	private:
		StatusCode _code;
		std::string_view _message;
	};
}