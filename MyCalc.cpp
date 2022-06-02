#include "MyCalc.h"
#include <math.h>

Vector3 AddVector3(const Vector3& vec1, const Vector3& vec2) {
	return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
}
Vector3 SubVector3(const Vector3& vec1, const Vector3& vec2) {
	return {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
}
Vector3 MultVector3(const Vector3& vec, const int scalar) {
	return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}
Vector3 DivVector3(const Vector3& vec, const int scalar) {
	return {vec.x / scalar, vec.y / scalar, vec.z / scalar};
}

int DotVector3(const Vector3& vec1, const Vector3& vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2) {
	return {
	  vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z,
	  vec1.x * vec2.y - vec1.y * vec2.x};
}

int SizeVector3(const Vector3& vec) { return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)); }

Vector3 NormalizeVector3(const Vector3& vec) { return DivVector3(vec, SizeVector3(vec)); }


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
