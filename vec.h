


class Vec{
public:
	float x = 0;
	float y = 0;

	Vec(float x, float y):
		x(x), y(y)
	{};
	Vec(): Vec(0,0) {};
	
	friend Vec operator + (const Vec& v1, const Vec& v2);
	friend Vec operator - (const Vec& v1, const Vec& v2);
	Vec& operator += (const Vec& v);
	Vec& operator -= (const Vec& v);
	Vec& operator *= (float a);
	friend Vec operator * (const Vec& v1, float a);
	friend Vec operator * (float a, const Vec& v1);
	// float norm() const;
};


float dot(const Vec& v1, const Vec& v2);
