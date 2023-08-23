#pragma once
#include <list>
#include <unordered_map>
#include <unordered_set>



struct Bucket
{
	void* ptr = nullptr;
	size_t sizePointer = 0;
	uint64_t id = UINT64_MAX;
};



class TMemory
{
	std::unordered_map<uint64_t, Bucket> m_Buckets;
	std::unordered_set<uint64_t> m_FreeId;
	uint64_t id_counter = 0;


public:
	TMemory();
	Bucket Allocate(size_t size);
	void Delete(Bucket& bucket);
	~TMemory();

};