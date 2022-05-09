#pragma once
#include "Vector3.h"

Vector3 AddVector3 (const Vector3& vec1, const Vector3& vec2); // �����Z
Vector3 SubVector3 (const Vector3& vec1, const Vector3& vec2); // �����Z
Vector3 MultVector3(const Vector3& vec, const int scalar); // �|���Z
Vector3 DivVector3 (const Vector3& vec, const int scalar); // ����Z
int DotVector3(const Vector3& vec1, const Vector3& vec2);		// ����
Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2); // �O��
int SizeVector3(const Vector3& vec); // �傫��
Vector3 NormalizeVector3(const Vector3& vec); // ���K��