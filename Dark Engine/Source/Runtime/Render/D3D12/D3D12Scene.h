#pragma once

#include "D3D12Main.h"
#include "D3D12Model.h"
#include "D3D12Camera.h"

class D3D12Scene
{
	TArray<D3D12Model*> m_models;
	D3D12Camera m_camera;
public:
	D3D12Scene() : m_camera({ 0, 0, 0 }, { 0, 0, 0 }) {}
	void SetCamera(D3D12Camera camera);
	void AddModel(D3D12Model* model);
	void AddModels(TArray<D3D12Model*> models);
	void SetModels(TArray<D3D12Model*> models);
	void ResetModels();
	TArray<D3D12Model*> GetModels() const { return m_models; }
	D3D12Camera& GetCamera() { return m_camera; }
};