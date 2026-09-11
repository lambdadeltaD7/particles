#include "vec.h"

Vec operator + (const Vec& v1, const Vec& v2)
{
	Vec res = v1;
	return res += v2;
}


Vec operator - (const Vec& v1, const Vec& v2)
{
	Vec res = v1;
	return res -= v2;
}


Vec& Vec::operator += (const Vec& v)
{
	x += v.x;
	y += v.y;
	return *this;
}


Vec& Vec::operator -= (const Vec& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}


Vec& Vec::operator *= (float a)
{
	x *= a;
	y *= a;
	return *this;
}


Vec operator * (const Vec& v1, float a)
{
	Vec res = v1;
	return res *= a;
}


Vec operator * (float a, const Vec& v1)
{
	Vec res = v1;
	return res *= a;
}

