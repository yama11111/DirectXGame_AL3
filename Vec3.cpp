#include "Vec3.h"
#include <math.h>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::Length() const { return sqrt(x * x + y * y + z * z); }

Vec3 Vec3::Nomalize() {
	if (Length() != 0)
		return *this / Length();
	return *this;
}

float Vec3::Dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

Vec3 Vec3::Closs(const Vec3& v) const {
	return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

Vec3 Vec3::operator+() const { return *this; }

Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }

Vec3 Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3 Vec3::operator-=(const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3 Vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vec3 Vec3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	Vec3 temp(v1);
	return temp += v2;
}

const Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	Vec3 temp(v1);
	return temp -= v2;
}

const Vec3 operator*(const Vec3& v, float s) {
	Vec3 temp(v);
	return temp *= s;
}

const Vec3 operator*(float s, const Vec3& v) { return v * s; }

const Vec3 operator/(const Vec3& v, float s) {
	Vec3 temp(v);
	return temp /= s;
}
