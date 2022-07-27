#include "MyCalc.h"
#include <math.h>
#include <cmath>

Vector3 AddVector3(const Vector3& vec1, const Vector3& vec2) {
	return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
}
Vector3 SubVector3(const Vector3& vec1, const Vector3& vec2) {
	return {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
}
Vector3 MultVector3(const Vector3& vec, const float scalar) {
	return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}
Vector3 DivVector3(const Vector3& vec, const float scalar) {
	return {vec.x / scalar, vec.y / scalar, vec.z / scalar};
}

float DotVector3(const Vector3& vec1, const Vector3& vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2) {
	return {
	  vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z,
	  vec1.x * vec2.y - vec1.y * vec2.x};
}

float SizeVector3(const Vector3& vec) { 
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)); 
}

Vector3 NormalizeVector3(const Vector3& vec) { 
	return DivVector3(vec, SizeVector3(vec)); 
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { 
	Vector3 v = SubVector3(v2, v1);
	return AddVector3(v1, MultVector3(v, t));
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

	float dot = DotVector3(v1, v2);
	if (dot >= 1.0 || dot <= -1.0) return v1;

	float theta = acosf(dot);
	float sTheta = sinf(theta);
	float sf = sinf((1 - t) * theta);
	float st = sinf(t * theta);

	float aS = SizeVector3(v1);
	float bS = SizeVector3(v2);
	float s = aS + t * (bS - aS);

	Vector3 from = MultVector3(v1, sf);
	Vector3 to = MultVector3(v2, st);
	Vector3 e = DivVector3(AddVector3(from, to), sTheta);

	return MultVector3(e, s);
}


// Matrix4 NomalizeMatrix(const Matrix4& max) {
//
//	return {};
// }

Matrix4 TransposeMatrix4(Matrix4& mat) {

	return {
	  mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0], mat.m[0][1], mat.m[1][1],
	  mat.m[2][1], mat.m[3][1], mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
	  mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3],
	};
}

Vector3 MultVec3Mat4(const Vector3& vec, const Matrix4& mat) { 
	return {
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2]
	}; 
}

Matrix4 MultMatrix4(const Matrix4& mat1, const Matrix4& mat2) {
	
	Matrix4 m;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				m.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
			}
		}
	}

	return m;
}

Matrix4 Scaling(const Matrix4& mat, const Vector3& scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0;

	return MultMatrix4(mat, matScale);
}

Matrix4 RotationX(const Matrix4& mat, const float rota) {
	Matrix4 matRota = {
		1, 0, 0, 0, 
		0, (float)cos(rota), (float)sin(rota), 0, 
		0, (float)-sin(rota), (float)cos(rota), 0,
		0, 0, 0, 1
	};

	return MultMatrix4(mat, matRota);
}
Matrix4 RotationY(const Matrix4& mat, const float rota) {
	Matrix4 matRota = {
		(float)cos(rota), 0, (float)-sin(rota), 0, 
		0, 1, 0, 0, 
		(float)sin(rota), 0, (float)cos(rota),  0,
		0, 0, 0, 1
	};

	return MultMatrix4(mat, matRota);
}
Matrix4 RotationZ(const Matrix4& mat, const float rota) { 
	Matrix4 matRota = {
		(float)cos(rota), (float)sin(rota), 0, 0, 
		(float)-sin(rota), (float)cos(rota), 0, 0, 
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return MultMatrix4(mat, matRota);
}

Matrix4 Rotation(const Matrix4& mat, const Vector3& rota) {
	Matrix4 m = mat;
	m = RotationZ(m, rota.z);
	m = RotationX(m, rota.x);
	m = RotationY(m, rota.y);
	return m;
}

Matrix4 Moving(const Matrix4& mat, const Vector3& move) { 
	Matrix4 matMove = {
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0,
		move.x, move.y, move.z, 1
	};

	return MultMatrix4(mat, matMove);
}

void Affine(WorldTransform& wt) {
	wt.matWorld_ = {
	  1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	};
	wt.matWorld_ = Scaling(wt.matWorld_, wt.scale_);
	wt.matWorld_ = Rotation(wt.matWorld_, wt.rotation_);
	wt.matWorld_ = Moving(wt.matWorld_, wt.translation_);
	wt.TransferMatrix();
}
