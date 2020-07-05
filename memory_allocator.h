#include <stdio.h>
#include <stdbool.h>
typedef struct MemoryAllocator{
    size_t  size;
    void*   ptr;
}MemoryAllocator;

MemoryAllocator*    MemoryAllocator_init(void * memoryPool, size_t size);
void *              MemoryAllocator_release(MemoryAllocator* allocator);
void *              MemoryAllocator_allocate(MemoryAllocator*  allocator,size_t size);
void                MemoryAllocator_free(MemoryAllocator* allocator,void * ptr);
size_t              MemoryAllocator_optimize(MemoryAllocator* allocator);
