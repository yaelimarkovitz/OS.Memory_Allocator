//
// Created by mby on 02/07/2020.
#include "memory_allocator.h"
#include "malloc.h"
int main(){

    /*---------check init------------*/

    void * memorypool = malloc(10000);
    MemoryAllocator* my = MemoryAllocator_init(memorypool,10000);

    /*----------check allocate----------*/

    void* frame = MemoryAllocator_allocate(my,50);
    printf("%lu\n", *((size_t*)frame - 1));
    void * frame2 = MemoryAllocator_allocate(my,100);
    printf("%lu\n", *((size_t*)frame2 - 1));

    /*--------check free----------*/
    MemoryAllocator_free(my,frame);

    return 0;
}
//

