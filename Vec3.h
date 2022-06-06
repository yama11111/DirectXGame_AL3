#pragma once
class Vec3 {
  public:
	float x;
	float y;
	float z;

  public:
	Vec3(); // —ëƒxƒNƒgƒ‹
	Vec3(float x, float y, float z);

	float Length() const;                  // ƒmƒ‹ƒ€(’·‚³)
	Vec3 Nomalize();                    // ³‹K‰»
	float Dot(const Vec3& v) const;     // “àÏ
	Vec3 Closs(const Vec3& v) const; // ŠOÏ

	// ’P€‰‰Zq
	Vec3 operator+() const;
	Vec3 operator-() const;

	// ‘ã“ü‰‰Zq
	Vec3 operator+=(const Vec3& v);
	Vec3 operator-=(const Vec3& v);
	Vec3 operator*=(float s);
	Vec3 operator/=(float s);
};

// “ñ€‰‰Zq
const Vec3 operator+(const Vec3& v1, const Vec3& v2);
const Vec3 operator-(const Vec3& v1, const Vec3& v2);
const Vec3 operator*(const Vec3& v, float s);
const Vec3 operator*(float s, const Vec3& v);
const Vec3 operator/(const Vec3& v, float s);