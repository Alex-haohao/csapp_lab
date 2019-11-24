#include <stdio.h>
#include "sfmm.h"

void *set_prologue(void *new_mem);
void *set_epilogue(void *new_mem);
void *set_freeblock(void *new_mem);
void *first_fit(size_t size);
void freelist_out(sf_block *ptr,int n);
void addlink(sf_block *ptr,int n);
void *coalescing(void* target);
size_t size_allign(size_t size);
int find_seg(size_t n);