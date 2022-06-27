#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#define PI 3.141592f

Vector3 AddVector3(const Vector3& vec1, const Vector3& vec2);   // �����Z
Vector3 SubVector3(const Vector3& vec1, const Vector3& vec2);   // �����Z
Vector3 MultVector3(const Vector3& vec, const int scalar);      // �|���Z
Vector3 DivVector3(const Vector3& vec, const int scalar);       // ����Z
int DotVector3(const Vector3& vec1, const Vector3& vec2);       // ����
Vector3 CrossVector3(const Vector3& vec1, const Vector3& vec2); // �O��
int SizeVector3(const Vector3& vec);                            // �傫��
Vector3 NormalizeVector3(const Vector3& vec);                   // ���K��

// Matrix4 NomalizeMatrix(const Matrix4& mat); // ���K��
Matrix4 TransposeMatrix4(const Matrix4& mat); // �]�u
Vector3 MultVec3Mat4(const Vector3& vec, const Matrix4& mat);

Matrix4 MultMatrix4(const Matrix4& mat1, const Matrix4& mat2);
Matrix4 Scaling(const Matrix4& mat, const Vector3& scale);
Matrix4 RotationX(const Matrix4& mat, const float rota);
Matrix4 RotationY(const Matrix4& mat, const float rota);
Matrix4 RotationZ(const Matrix4& mat, const float rota);
Matrix4 Rotation(const Matrix4& mat, const Vector3& rota);
Matrix4 Moving(const Matrix4& mat, const Vector3& move);
