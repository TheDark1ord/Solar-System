#include "pch.h"

long double operator""s(long double val)
{
	return val;
}

long double operator""s(unsigned long long val)
{
	return val;
}

long double operator""min(long double val)
{
	return val * 60;
}

long double operator""min(unsigned long long val)
{
	return val * 60;
}

long double operator""h(long double val)
{
	return val * 60 * 60;
}

long double operator""h(unsigned long long val)
{
	return val * 60 * 60;
}

long double operator""d(long double val)
{
	return val * 60 * 60 * 24;
}

long double operator""d(unsigned long long val)
{
	return val * 60 * 60 * 24;
}

long double operator""y(long double val)
{
	return val * 60 * 60 * 24 * 365;
}

long double operator""y(unsigned long long val)
{
	return val * 60 * 60 * 24 * 365;
}

long double operator""m(long double val)
{
	return val;
}

long double operator""m(unsigned long long val)
{
	return val;
}

long double operator""km(long double val)
{
	return val * 1000;
}

long double operator""km(unsigned long long val)
{
	return val * 1000;
}

long double operator""ls(long double val)
{
	return val * 2.99792e8;
}

long double operator""ls(unsigned long long val)
{
	return val * 2.99792e8;
}

long double operator""lm(long double val)
{
	return val * 1.79875e10;
}

long double operator""lm(unsigned long long val)
{
	return val * 1.79875e10;
}

long double operator""ly(long double val)
{
	return val * 9.4605e15;
}

long double operator""ly(unsigned long long val)
{
	return val * 9.4605e15;
}

long double operator""au(long double val)
{
	return val * 1.496e11;
}

long double operator""au(unsigned long long val)
{
	return val * 1.496e11;
}

long double operator""pc(long double val)
{
	return val * 3.0857e16;
}

long double operator""pc(unsigned long long val)
{
	return val * 3.0857e16;
}
long double operator""er(long double val)
{
	return val * 6.371e6;
}
long double operator""er(unsigned long long val)
{
	return val * 6.371e6;
}