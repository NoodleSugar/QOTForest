#ifndef UTILITY_H
#define UTILITY_H

typedef unsigned int uint;

template<int N>
struct PowerOfTwo
{
	enum
	{
		value = 2 * PowerOfTwo<N - 1>::value
	};
};

template<>
struct PowerOfTwo<0>
{
	enum
	{
		value = 1
	};
};

#endif