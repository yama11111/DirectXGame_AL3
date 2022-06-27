#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#define PI 3.141592f

Vector3 AddVector3(const Vector3& vec1, const Vector3& vec2);   // ‘«‚µZ
Vector3 SubVector3(const Vector3& vec1, const Vector3& vec2);   // ˆø‚«Z
Vector3 MultVector3(const Vector3& vec, const int scalar);      // Š|‚¯Z
Vector3 DivVector3(const Vector3& vec, const int scalar);       // Š„‚èZ
int DotVector3(const Vector3& vec1, const Vector3& vec2);       // “àÏ
Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2); // ŠOÏ
int SizeVector3(const Vector3& vec);                            // ‘å‚«‚³
Vector3 NormalizeVector3(const Vector3& vec);                   // ³‹K‰»

// Matrix4 NomalizeMatrix(const Matrix4& mat); // ³‹K‰»
Matrix4 TransposeMatrix4(const Matrix4& mat); // “]’u
Vector3 MultVec3Mat4(const Vector3& vec, const Matrix4& mat);

Matrix4 MultMatrix4(const Matrix4& mat1, const Matrix4& mat2);
Matrix4 Scaling(const Matrix4& mat, const Vector3& scale);
Matrix4 RotationX(const Matrix4& mat, const float rota);
Matrix4 RotationY(const Matrix4& mat, const float rota);
Matrix4 RotationZ(const Matrix4& mat, const float rota);
Matrix4 Rotation(const Matrix4& mat, const Vector3& rota);
Matrix4 Moving(const Matrix4& mat, const Vector3& move);
