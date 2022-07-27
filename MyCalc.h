#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "WorldTransform.h"
#define PI 3.141592f

Vector3 AddVector3(const Vector3& vec1, const Vector3& vec2);   // ë´ÇµéZ
Vector3 SubVector3(const Vector3& vec1, const Vector3& vec2);   // à¯Ç´éZ
Vector3 MultVector3(const Vector3& vec, const float scalar);      // ä|ÇØéZ
Vector3 DivVector3(const Vector3& vec, const float scalar);       // äÑÇËéZ
float DotVector3(const Vector3& vec1, const Vector3& vec2);       // ì‡êœ
Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2); // äOêœ
float SizeVector3(const Vector3& vec);                            // ëÂÇ´Ç≥
Vector3 NormalizeVector3(const Vector3& vec);                   // ê≥ãKâª

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

// Matrix4 NomalizeMatrix(const Matrix4& mat); // ê≥ãKâª
Matrix4 TransposeMatrix4(const Matrix4& mat); // ì]íu
Vector3 MultVec3Mat4(const Vector3& vec, const Matrix4& mat);

Matrix4 MultMatrix4(const Matrix4& mat1, const Matrix4& mat2);
Matrix4 Scaling(const Matrix4& mat, const Vector3& scale);
Matrix4 RotationX(const Matrix4& mat, const float rota);
Matrix4 RotationY(const Matrix4& mat, const float rota);
Matrix4 RotationZ(const Matrix4& mat, const float rota);
Matrix4 Rotation(const Matrix4& mat, const Vector3& rota);
Matrix4 Moving(const Matrix4& mat, const Vector3& move);

void Affine(WorldTransform& wt);