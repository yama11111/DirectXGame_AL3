#pragma once
#include "Vector3.h"

Vector3 AddVector3 (const Vector3& vec1, const Vector3& vec2); // ‘«‚µZ
Vector3 SubVector3 (const Vector3& vec1, const Vector3& vec2); // ˆø‚«Z
Vector3 MultVector3(const Vector3& vec, const int scalar); // Š|‚¯Z
Vector3 DivVector3 (const Vector3& vec, const int scalar); // Š„‚èZ
int DotVector3(const Vector3& vec1, const Vector3& vec2);		// “àÏ
Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2); // ŠOÏ
int SizeVector3(const Vector3& vec); // ‘å‚«‚³
Vector3 NormalizeVector3(const Vector3& vec); // ³‹K‰»