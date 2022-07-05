#pragma once

#include "Result.hpp"

namespace SAMOSBOR::core
{
	/// <summary>
	/// Used to wrap a Status along with a value of a specified type T. 
	/// ResultOr<T> is intended to be returned from functions in situations where it is desirable 
	/// to carry over more information about the potential errors encountered during the function execution.
	/// </summary>
	template<class T>
	class ResultOr
	{
	public:
		ResultOr() 
		{
		}

		ResultOr(const ResultOr&) = default;
		ResultOr(ResultOr&&) = default;
		
		ResultOr(const Result& status) : _result(status) 
		{
		}

		ResultOr(const T& value) : _result(Result(Result::StatusCode::OK)), _value(value) 
		{
		}

		ResultOr(T&& value) : _result(Result(Result::StatusCode::OK)), _value(std::move(value))
		{
		}

		ResultOr(const Result::StatusCode status, const T& value): _result(Result(status)), _value(value)
		{
		}

		ResultOr(const Result& status, const T& value): _result(status), _value(value) 
		{
		}

		const Result& Res() const 
		{ 
			return _result; 
		}

		const T& Value() const& 
		{ 
			return _value; 
		}

		const T&& Value() const&& 
		{ 
			return std::move(_value); 
		}

		T&& Value()&& 
		{ 
			return std::move(_value); 
		}

		bool Ok() const 
		{ 
			return _result.Ok(); 
		}

	private:
		Result _result;
		T _value;
	};

#define ASSIGN_OR_RETURN(lhs, expression) \
	auto resultor = expression;           \
	if (!resultor.Ok())                   \
	{                                     \
		return resultor.Res();            \
	}                                     \
	lhs = resultor.Value();

#define ASSIGN_OR_RETURN_T(lhs, expression, T)               \
	auto resultor = expression;                              \
	if (!resultor.Ok())                                      \
	{                                                        \
		return SAMOSBOR::core::ResultOr<T>(resultor.Res());  \
	}                                                        \
	lhs = resultor.Value();
}