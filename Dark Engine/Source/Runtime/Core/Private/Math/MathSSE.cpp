#include "Math/MathSSE.h"
#include "Math/MathFwd.h"
#include "DirectXMath.h"



template<>
void MatrixMultiply(FMatrix4x4f* Result, const FMatrix4x4f* Matrix1, const FMatrix4x4f* Matrix2)
{
	DirectX::XMMATRIX* R = (DirectX::XMMATRIX*)Result;
	DirectX::XMMATRIX* A = (DirectX::XMMATRIX*)Matrix1;
	DirectX::XMMATRIX* B = (DirectX::XMMATRIX*)Matrix2;
	*R = DirectX::XMMatrixMultiply(*A, *B);
}

template<>
void MatrixMultiply(FMatrix4x4d* Result, const FMatrix4x4d* Matrix1, const FMatrix4x4d* Matrix2)
{
	typedef double Double4x4[4][4];
	const Double4x4& A = *(const Double4x4*)(Matrix1->M);
	const Double4x4& B = *(const Double4x4*)(Matrix2->M);

	VectorRegister4Double R0, R1, R2, R3;
	VectorRegister4Double R[4];

	R[0] = VectorLoad(B[0]);
	R[1] = VectorLoad(B[1]);
	R[2] = VectorLoad(B[2]);
	R[3] = VectorLoad(B[3]);

	{
		const VectorRegister4Double ARow = VectorLoad(A[0]);
		R0 = VectorMultiply(VectorReplicateImpl4<0>(ARow), R[0]);
		R0 = VectorMultiplyAdd(VectorReplicateImpl4<1>(ARow), R[1], R0);
		R0 = VectorMultiplyAdd(VectorReplicateImpl4<2>(ARow), R[2], R0);
		R0 = VectorMultiplyAdd(VectorReplicateImpl4<3>(ARow), R[3], R0);
	}
	{
		const VectorRegister4Double ARow = VectorLoad(A[1]);
		R1 = VectorMultiply(VectorReplicateImpl4<0>(ARow), R[0]);
		R1 = VectorMultiplyAdd(VectorReplicateImpl4<1>(ARow), R[1], R1);
		R1 = VectorMultiplyAdd(VectorReplicateImpl4<2>(ARow), R[2], R1);
		R1 = VectorMultiplyAdd(VectorReplicateImpl4<3>(ARow), R[3], R1);
	}
	{
		const VectorRegister4Double ARow = VectorLoad(A[2]);
		R2 = VectorMultiply(VectorReplicateImpl4<0>(ARow), R[0]);
		R2 = VectorMultiplyAdd(VectorReplicateImpl4<1>(ARow), R[1], R2);
		R2 = VectorMultiplyAdd(VectorReplicateImpl4<2>(ARow), R[2], R2);
		R2 = VectorMultiplyAdd(VectorReplicateImpl4<3>(ARow), R[3], R2);
	}
	{
		const VectorRegister4Double ARow = VectorLoad(A[3]);
		R3 = VectorMultiply(VectorReplicateImpl4<0>(ARow), R[0]);
		R3 = VectorMultiplyAdd(VectorReplicateImpl4<1>(ARow), R[1], R3);
		R3 = VectorMultiplyAdd(VectorReplicateImpl4<2>(ARow), R[2], R3);
		R3 = VectorMultiplyAdd(VectorReplicateImpl4<3>(ARow), R[3], R3);
	}

	Double4x4& ResultDst = *(Double4x4*)Result->M;
	VectorStore(R0, ResultDst[0]);
	VectorStore(R1, ResultDst[1]);
	VectorStore(R2, ResultDst[2]);
	VectorStore(R3, ResultDst[3]);



}
