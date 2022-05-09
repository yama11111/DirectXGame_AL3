#include "CalcVector.h"
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
	  vec1.y * vec2.z - vec1.z * vec2.y,
	  vec1.z * vec2.x - vec1.x * vec2.z,
	  vec1.x * vec2.y - vec1.y * vec2.x
	};
}

int SizeVector3(const Vector3& vec) { 
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)); 
}

Vector3 NormalizeVector3(const Vector3& vec) { 
	return DivVector3(vec, SizeVector3(vec));
}
