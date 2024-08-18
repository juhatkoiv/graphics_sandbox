#pragma once

template<typename T>
struct [[nodiscard]] Result
{
	T value;
	bool success = false;

	T get()
	{
		return value;
	}

	bool operator!()
	{
		return !success;
	}
	
	bool operator()()
	{
		return success;
	}

	static Result<T> ok( T value )
	{
		return Result<T>{ value, true };
	}

	static Result<T> failed()
	{
		return Result<T>{ T{}, false };
	}
};

