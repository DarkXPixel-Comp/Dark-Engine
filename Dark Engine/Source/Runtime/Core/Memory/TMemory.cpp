#include "TMemory.h"

TMemory::TMemory()
{
    m_FreeId.insert(UINT64_MAX);
}

TMemory::~TMemory()
{
    for (auto& i : m_Buckets)
    {
        Delete(i.second);
    }

}



Bucket TMemory::Allocate(size_t size)
{
    Bucket bucket = {};
    uint64_t m = *std::min_element(m_FreeId.begin(), m_FreeId.end());


    if (m < id_counter)
    {
        m_FreeId.erase(m);
        bucket.id = m;
    }
    else
        bucket.id = id_counter++;

    bucket.sizePointer = size;
    bucket.ptr = malloc(size);
    if (!bucket.ptr)
    {
        free(bucket.ptr);
        return Bucket();
    }
   
    m_Buckets.insert(std::make_pair(bucket.id, bucket));


    return bucket;
}


void TMemory::Delete(Bucket& bucket)
{
    if (bucket.ptr)
    {
        free(bucket.ptr);
        bucket.ptr = nullptr;
        m_FreeId.insert(bucket.id);
    }

    bucket.id = 0;
    bucket.sizePointer = 0;
}

