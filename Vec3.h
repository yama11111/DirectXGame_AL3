#pragma once
class Vec3 {
  public:
	float x;
	float y;
	float z;

  public:
	Vec3(); // ��x�N�g��
	Vec3(float x, float y, float z);

	float Length() const;                  // �m����(����)
	Vec3 Nomalize();                    // ���K��
	float Dot(const Vec3& v) const;     // ����
	Vec3 Closs(const Vec3& v) const; // �O��

	// �P�����Z�q
	Vec3 operator+() const;
	Vec3 operator-() const;

	// ������Z�q
	Vec3 operator+=(const Vec3& v);
	Vec3 operator-=(const Vec3& v);
	Vec3 operator*=(float s);
	Vec3 operator/=(float s);
};

// �񍀉��Z�q
const Vec3 operator+(const Vec3& v1, const Vec3& v2);
const Vec3 operator-(const Vec3& v1, const Vec3& v2);
const Vec3 operator*(const Vec3& v, float s);
const Vec3 operator*(float s, const Vec3& v);
const Vec3 operator/(const Vec3& v, float s);