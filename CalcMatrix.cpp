#include "CalcMatrix.h"

//Matrix4 NomalizeMatrix(const Matrix4& max) { 
//	
//	return {};
//}

Matrix4 TransposeMatrix(Matrix4& mat) { 
	
	return {
	  mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
	  mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
	  mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
	  mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3],
	};
}
