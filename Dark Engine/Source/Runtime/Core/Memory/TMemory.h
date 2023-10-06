#pragma once
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "HAL/Platform.h"



struct Bucket
{
	void* ptr = nullptr;
	size_t sizePointer = 0;
	uint64 id = UINT64_MAX;
};



class TMemory
{
	std::unordered_map<uint64, Bucket> m_Buckets;
	std::unordered_set<uint64> m_FreeId;
	uint64 id_counter = 0;


public:
	TMemory();
	Bucket Allocate(size_t size);
	void Delete(Bucket& bucket);
	~TMemory();

};