namespace safeArithmetic
{
	int add(const int lhs, const int rhs)
	{
		int res;
		if(__builtin_add_overflow(lhs, rhs, &res))
			throw std::runtime_error("Integer addition overflow");
		return res;
	}

	int sub(const int lhs, const int rhs)
	{
		int res;
		if(__builtin_sub_overflow(lhs, rhs, &res))
			throw std::runtime_error("Integer substraction overflow");
		return res;
	}

	int mult(const int lhs, const int rhs)
	{
		int res;
		if(__builtin_mul_overflow(lhs, rhs, &res))
			throw std::runtime_error("Integer multiplication overflow");
		return res;
	}

	int div(const int lhs, const int rhs)
	{
		if(rhs == 0)
			throw std::runtime_error("Division by zero");
		return lhs/rhs;
	}
}
