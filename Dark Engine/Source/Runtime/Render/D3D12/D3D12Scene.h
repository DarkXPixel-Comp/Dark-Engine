#pragma once

#include "D3D12.h"
#include "D3D12Model.h"
#include "D3D12Camera.h"

class D3D12Scene
{
	std::vector<D3D12Model*> m_models;
	D3D12Camera m_camera;
public:
	D3D12Scene() : m_camera({ 0, 0, 0 }, { 0, 0, 0 }) {}
	void SetCamera(D3D12Camera camera);
	void AddModel(D3D12Model* model);
	void AddModels(std::vector<D3D12Model*> models);
	void SetModels(std::vector<D3D12Model*> models);
	void ResetModels();
	std::vector<D3D12Model*> GetModels() const { return m_models; }
	D3D12Camera GetCamera() const { return m_camera; }
};