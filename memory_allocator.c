#include "memory_allocator.h"
#include "malloc.h"
#define ALIGNMENT 8
/*---------------------Auxiliary functions-----------------------------*/
bool isUsed(void* alloc) {
    if((*(size_t*)alloc&1)==1)
        return true;
    return false;
}

size_t getSize(void* alloc) {
    return (*((size_t*)alloc)^1);
}

void changeBit(void* alloc) {
    *((size_t*)alloc)=*((size_t*)alloc)^1;
}

void* get_payload(void* tmp){
    return tmp+ sizeof(size_t);
}

void find_free_block(void** tmp,size_t *len){
    while (isUsed(*tmp))
    {
        *tmp+=(*((size_t*)*tmp)-1);
        *len-= *((size_t*)*tmp)-1;
    }
}

size_t get_alignment(size_t number){
   return (number + 1) + ALIGNMENT-(((char)(number+1)) % ALIGNMENT);
}

void* get_next_block(void *tmp){
    return tmp+(*(size_t*)(tmp));
}

void split(void * tmp,size_t new_size){
    size_t old_size = *(size_t*)(tmp);
    size_t* convert = (size_t*)tmp;
    void *res = tmp;
    *convert = new_size;
    changeBit(tmp);
    tmp+=new_size;
    convert=tmp;
    *convert=old_size-new_size;
    printf("%ld\n",*((size_t*)convert));
}

void defragmentation(void* ptr){
    size_t* convert = (size_t*)ptr;
    void* next = ptr+(*(size_t*)ptr);
    *convert = *convert + (*(size_t*)next);
}

void init_meta_data(void **block , size_t size){
    size_t* tmp = (size_t*)(*block);
    *tmp = size;
}

/*-----------------------end Auxiliary functions---------------------------------------------*/

MemoryAllocator* MemoryAllocator_init(void * memoryPool, size_t size){
    MemoryAllocator* first = (MemoryAllocator*)malloc(sizeof(MemoryAllocator));
    first->size = size;
    first->ptr  = memoryPool;
    init_meta_data(&first->ptr,size);
    return first;
}

void * MemoryAllocator_release(MemoryAllocator* allocator){
    free(allocator->ptr);
}

void * MemoryAllocator_allocate(MemoryAllocator* allocator,size_t size){

    size_t new_size = get_alignment(size);
    size_t len  = allocator->size;
    void * tmp = allocator->ptr;
    while(len>0){
        find_free_block(&tmp,&len);
        if (*((size_t*)tmp)==new_size) /*check if the size of free block is fit exactly*/
        {
            changeBit(tmp);
            return get_payload(tmp);
        }
        else if ((*((size_t*)tmp)) > new_size)/*check if the size of free block is fit but have more space*/
        {
            split(tmp,new_size);
            return get_payload(tmp);
        }
        else if (!isUsed(get_next_block(tmp))) /*check if the size is smaller than the necessary size*/
        {
            defragmentation(allocator+*(size_t*)tmp);
            return MemoryAllocator_allocate(allocator,size);
        }
        else{
            len -=*((size_t*)tmp)-1;
            tmp+=*(size_t*)tmp;
        }
    }
}

void  MemoryAllocator_free(MemoryAllocator* allocator,void * ptr){
    ptr-=1;
    changeBit(ptr);
    if (isUsed(get_next_block(ptr)))
    {
        defragmentation(ptr);
    }
}
size_t  MemoryAllocator_optimize(MemoryAllocator* allocator){
    size_t max = 0;
    size_t len = allocator->size;
    void* tmp = allocator->ptr;
    while (len){
        while (!isUsed(tmp) && !isUsed(get_next_block(tmp)))
        {
            defragmentation(tmp);
        }
        max = max > (*(size_t*)tmp) ? max : (*(size_t*)tmp);
        tmp+=*(size_t*)tmp;
    }
    return max;
}