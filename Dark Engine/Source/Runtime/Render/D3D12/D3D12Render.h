#pragma once


#include "../Renderer.h"

#include "D3D12.h"

#include <Model.h>
#include <Keyboard.h>

#include <SimpleMath.h>
#include <VertexTypes.h>
#include <GraphicsMemory.h>
#include <Core/CommandLine/CommandLine.h>

using namespace std;
using namespace DirectX;


//struct Vector3;



struct VertexPosColor
{
	VertexPosColor(XMFLOAT3 iposition, XMFLOAT3 icolor) : Position(iposition), Color(icolor) {}

	XMFLOAT3 Position;
	XMFLOAT3 Color;
};

typedef VertexPositionNormalColorTexture Vertex;



//VertexPositionNormalColorTexture

class D3D12Mesh;
class D3D12Mesh1;
class D3D12Model;
class D3D12Material;


//
//static VertexPosColor Vertices[8] = {
//	VertexPosColor(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 0
//	VertexPosColor(XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 1
//	VertexPosColor(XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 2
//	VertexPosColor(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 3
//	VertexPosColor(XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 4
//	VertexPosColor(XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 5
//	VertexPosColor(XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)), // 6
//	VertexPosColor(XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f))  // 7
//};

//static Vertex Vertices[8] =
//{
//	VertexPositionColor(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)),
//	VertexPositionColor(XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 1
//	VertexPositionColor(XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 2
//	VertexPositionColor(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 3
//	VertexPositionColor(XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 4
//	VertexPositionColor(XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 5
//	VertexPositionColor(XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f)), // 6
//	VertexPositionColor(XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f))  // 7
//
//
//};


static Vertex ConusVertices[] =
{		// Vertex							Nolmal					Color								UV					 // нижняя сторона
	Vertex(XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, -1.f, 0.f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.0f, 0.0f)),  // левый дальний
	Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, -1.f, 0.f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.0f, 1.0f)), // левый ближний
	Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, -1.f, 0.f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.0f, 1.0f)),	// правый ближний
	Vertex(XMFLOAT3(0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, -1.f, 0.f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.0f, 0.0f)),	// правый дальний
																															// левая сторона
	Vertex(XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT3(-0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.f, 1.f)),	// левый нижний
	Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(-0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.f, 1.f)),	// правый нижний
	Vertex(XMFLOAT3(0.0f, 0.8f,  0.0f), XMFLOAT3(-0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.5f, 0.f)),  // вернхий
																															// передняя сторона
	Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.5f, -0.8f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.f, 1.f)),	// правый нижний
	Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.5f, -0.8f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.0f, 1.f)),	// левый нижний
	Vertex(XMFLOAT3(0.0f, 0.8f,  0.0f), XMFLOAT3(0.0f, 0.5f, -0.8f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.5f, 0.f)),	// верхий
																															// правая сторона
	Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.f, 1.f)),	// левый нижний
	Vertex(XMFLOAT3(0.5f, 0.0f,  0.5f), XMFLOAT3(0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.f, 1.f)),	// правый нижний
	Vertex(XMFLOAT3(0.0f, 0.8f,  0.0f), XMFLOAT3(0.8f, 0.5f,  0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.5f, 0.f)),	// верхний
																															// задняя сторона
	Vertex(XMFLOAT3(0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, 0.5f,  0.8f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.f, 1.f)),	// левый нижний
	Vertex(XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, 0.5f,  0.8f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(1.f, 1.f)),	// правый нижний
	Vertex(XMFLOAT3(0.0f, 0.8f,  0.0f), XMFLOAT3(0.0f, 0.5f,  0.8f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f), XMFLOAT2(0.5f, 0.f))	// верхний
};


static Vertex Vertices[]
{	//FORWARD
	Vertex(XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT3(0.f, 0.f, -1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 0 // левый нижний
	Vertex(XMFLOAT3(-1.f,  1.f, -1.f), XMFLOAT3(0.f, 0.f, -1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 1 // левый верхний
	Vertex(XMFLOAT3(1.f,   1.f, -1.f), XMFLOAT3(0.f, 0.f, -1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 2 // правый верхний
	Vertex(XMFLOAT3(1.f,  -1.f, -1.f), XMFLOAT3(0.f, 0.f, -1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 3 //правый нижний
	//LEFT
	Vertex(XMFLOAT3(-1.f, -1.f, 1.f), XMFLOAT3(-1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 4 // левый нижний
	Vertex(XMFLOAT3(-1.f,  1.f, 1.f), XMFLOAT3(-1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 5/ левый верхний
	Vertex(XMFLOAT3(-1.f,  1.f,-1.f), XMFLOAT3(-1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 6 // правый верхний
	Vertex(XMFLOAT3(-1.f, -1.f,-1.f), XMFLOAT3(-1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 7//правый нижний
	//BACK
	Vertex(XMFLOAT3(1.f, -1.f, 1.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 8 // левый нижний
	Vertex(XMFLOAT3(1.f,  1.f, 1.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 9/ левый верхний
	Vertex(XMFLOAT3(-1.f, 1.f, 1.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 10 // правый верхний
	Vertex(XMFLOAT3(-1.f, -1.f,1.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 11//правый нижний
	//RIGHT
	Vertex(XMFLOAT3(1.f, -1.f, -1.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 12 // левый нижний
	Vertex(XMFLOAT3(1.f, 1.f, -1.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 13/ левый верхний
	Vertex(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 14 // правый верхний
	Vertex(XMFLOAT3(1.f, -1.f, 1.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 15//правый нижний
	//UP
	Vertex(XMFLOAT3(-1.f, 1.f, -1.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 16 // левый нижний
	Vertex(XMFLOAT3(-1.f, 1.f, 1.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 17/ левый верхний
	Vertex(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 18 // правый верхний
	Vertex(XMFLOAT3(1.f, 1.f, -1.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 19//правый нижний
	//DOWN
	Vertex(XMFLOAT3(-1.f, -1.f, 1.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 1.f)), // 20 // левый нижний
	Vertex(XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0.f, 0.f)), // 21/ левый верхний
	Vertex(XMFLOAT3(1.f, -1.f, -1.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 0.f)), // 22 // правый верхний
	Vertex(XMFLOAT3(1.f, -1.f, 1.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(1.f, 1.f)), // 23//правый нижний

};



//static VertexPosColor Vertices[3] = {
//	VertexPosColor(XMFLOAT4(0.f, 0.25f, 0, 1), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)), // 0
//	VertexPosColor(XMFLOAT4(0.25f, -0.25f, 0.0f, 1), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)), // 1
//	VertexPosColor(XMFLOAT4(-0.25f, -0.25f, 0.0f, 1), XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f)), // 2
//};

static WORD Indices[]
{
	20, 21, 22,
	20, 22, 23,

	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	12, 13, 14,
	12, 14, 15,

	16, 17, 18,
	16, 18, 19,

	0, 1, 2,
	0, 2, 3



};





static WORD ConusIndices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

//static WORD Indices[3] =
//{
//	0, 1, 2
//};

class D3D12Renderer : public Renderer
{
private:
	static const uint32_t BACK_BUFFER_COUNT = 2;
	void Test() { PrintLine("Test"); }

public:
	D3D12Renderer();

public:
	void Init() override;
	void Shutdown() override;

	void BeginFrame() override;
	void Render() override;
	void RenderObj();
	void RenderScene();
	void EndFrame() override;
	std::vector<D3D12Mesh1*> LoadMesh(std::string path = "");

	void Update();

private:
	uint64_t Signal(uint64_t &fenceValue);
	void WaitForFenceValue(uint64_t fenceValue);
	void WaitFrame();




	float rot = 0.0f;

public:
	friend class D3D12Mesh;


	D3D12MA::Allocator* MemoryAllocator;


	ComPtr<IDXGIFactory7> Factory;
	ComPtr<ID3D12Device8> Device;
	ComPtr<IDXGIAdapter1> Adapter;

	ComPtr<ID3D12CommandAllocator> CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList6> GraphicsCommandList;
	ComPtr<ID3D12CommandQueue> CommandQueue;
	ComPtr<ID3D12CommandList> CommandList;

	unique_ptr <DescriptorHeap> RTDescriptorHeap;
	unique_ptr <DescriptorHeap> DSDescriptorHeap;
	unique_ptr<DescriptorHeap> SRDescriptorHeap;


//	ComPtr<ID3D12DescriptorHeap> RTDescriptorHeap;
	//ComPtr<ID3D12DescriptorHeap> DSDescriptorHeap;
	//ComPtr<ID3D12DescriptorHeap> DescriptorHeap;

	ComPtr<ID3D12PipelineState> PipelineState;
	ComPtr<ID3D12RootSignature> RootSignature;
	ComPtr<IDXGISwapChain4> SwapChain;

	Resource VertexBuffer;
	Resource IndexBuffer;
	Resource BackBuffers[BACK_BUFFER_COUNT];
	Resource DepthBuffer;

	Resource Texture;

	unique_ptr<Model> Bone;
	unique_ptr<GraphicsMemory> graphicsMemory;


	ModelMeshPart* TestBone;
	Assimp::Importer importer;
	








	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = {};
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = {};
	//D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResouceView = {};

	HANDLE FenceEvent;
	ComPtr<ID3D12Fence> Fence;
	uint64_t FenceValue;

	uint64_t CurrentBackBufferIndex = 0;
	SIZE_T RTHandleSize = 0;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;
	float Fov;

	XMMATRIX ModelMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;

	
	std::chrono::steady_clock::time_point now;

	float x, y, z;

	float CameraX, CameraY, CameraZ;

	float ScaleX, ScaleY, ScaleZ;

	XMFLOAT3 LightPosition;


	XMVECTOR eyePosition;
	XMVECTOR focusPoint;
	XMVECTOR upDirection;


	
	DirectX::SimpleMath::Vector3 Scale;



	//Vector3 CameraPos = { 0.f, 0.f, 0.f };
	//Quaternion
	
	float Transl = 10.000;

	float pitch, yaw, roll;


	float an = 0.1;
	std::unique_ptr<Model> Modell;
	Keyboard::KeyboardStateTracker tracker;

	float sensevity = 0.05;


	float iTime = 0.f;

};

struct D3D12MeshInitInfo
{
	std::vector<Vertex> Vertices;
	std::vector<WORD> Indices;

	std::wstring TexturePath = L"C:\\Users\\nahmu\\source\\repos\\Dark Engine\\Dark Engine\\Resources\\Texture.dds";

	std::wstring VertexShader = L"shaders/LightVertex.hlsl";
	std::wstring PixelShader = L"shaders/LightPixel.hlsl";




};


class D3D12Model
{
	friend class D3D12Renderer;

public:
	D3D12Model(D3D12Mesh1* mesh, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 sc);

	void Update();



	D3D12Mesh1* Mesh;
	//D3D12Material* Material;

	ComPtr<ID3D12GraphicsCommandList6> m_CommandList;
	ComPtr<ID3D12CommandAllocator> m_Allocator;
	ID3D12Device* m_device;
	

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMMATRIX ModelMatrix;


};



class D3D12Mesh1
{
	friend class D3D12Model1;

	friend class D3D12Renderer;

	std::vector<Vertex> Vertexes;
	std::vector<WORD> Indexes;


	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	Resource VertexBuffer;
	Resource IndexBuffer;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = {};
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = {};

	static D3D12Mesh1* DefaultMesh;


public:
	void Init(ID3D12Device* device, ID3D12CommandQueue* queue, void* vertexes, size_t vertSize, void* indexes, size_t indSize);
	/*void Update();
	void Clear();*/


	ID3D12Device* m_device;
	ID3D12CommandQueue* m_Queue;






};

class D3D12Model1
{
	friend class D3D12Renderer;


public:
	D3D12Model1(D3D12Mesh1* mesh = D3D12Mesh1::DefaultMesh);


	D3D12Mesh1* Mesh;



	ComPtr<ID3D12GraphicsCommandList6> m_CommandList;
	ComPtr<ID3D12CommandAllocator> m_Allocator;
	ID3D12Device* m_device;



};














//class D3D12Mesh
//{
//	friend class D3D12Renderer;
//
//	std::vector<Vertex> Vertexes;
//	std::vector<WORD> Indexes;
//
//
//	ComPtr<ID3D12GraphicsCommandList6> CommandList;
//	ComPtr<ID3D12CommandAllocator> Allocator;
//
//
//	XMFLOAT3 Position;
//	XMFLOAT3 Rotation;
//	XMFLOAT3 Scale;
//
//	Resource VertexBuffer;
//	Resource IndexBuffer;
//
//	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = {};
//	D3D12_INDEX_BUFFER_VIEW IndexBufferView = {};
//
//	Resource Texture;
//
//
//	XMMATRIX ModelMatrix;
//
//	ComPtr<ID3D12PipelineState> PipelineState;
//	ComPtr<ID3D12RootSignature> RootSignature;
//
//	unique_ptr<DescriptorHeap> SRDescriptorHeap;
//
//	D3D12_CPU_DESCRIPTOR_HANDLE cpuTexture;
//	D3D12_GPU_DESCRIPTOR_HANDLE gpuTexture;
//
//
//	ID3D12Device8* m_device;
//
//public:
//	void Init(const D3D12Renderer* renderer, D3D12MeshInitInfo info = D3D12MeshInitInfo());
//	void Update();
//	void Clear();
//
//
//
//
//
//};