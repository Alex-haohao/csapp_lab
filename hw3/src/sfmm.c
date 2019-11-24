/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"
#include "help.h"
#include <errno.h>

#define WSIZE 4 //one word size
#define DSIZE 8
#define M 32

#define MAX(x, y) ((x) > (y)? (x) : (y))



int find_seg(size_t size){
    int i;
    if(size ==M){
        i = 0;
    }
    else if(size>M && size<=2*M){
        i = 1;
    }
    else if(size>2*M && size<=4*M){
        i = 2;
    }
    else if(size>4*M && size<=8*M){
        i = 3;
    }
    else if(size>8*M && size<=16*M){
        i = 4;
    }
    else if(size>16*M && size<=32*M){
        i = 5;
    }
    else if(size>32*M && size<=64*M){
        i = 6;
    }
    else if(size>64*M && size<=128*M){
        i = 7;
    }
    else if(size>128*M){
        i = 8;
    }
    return i;
}

void *first_fit(size_t size){
    int i;
    if(size ==M){
        i = 0;
    }
    else if(size>M && size<=2*M){
        i = 1;
    }
    else if(size>2*M && size<=4*M){
        i = 2;
    }
    else if(size>4*M && size<=8*M){
        i = 3;
    }
    else if(size>8*M && size<=16*M){
        i = 4;
    }
    else if(size>16*M && size<=32*M){
        i = 5;
    }
    else if(size>32*M && size<=64*M){
        i = 6;
    }
    else if(size>64*M && size<=128*M){
        i = 7;
    }
    else if(size>128*M){
        i = 8;
    }




    int choose =-1;
    sf_block *block;
    for(int n =i;n<9;n++){

        block = &sf_free_list_heads[n];


        if(block->body.links.next == &sf_free_list_heads[n] &&
            block->body.links.prev == &sf_free_list_heads[n]){
            continue;
        }
        else{

            choose = n;
            break;
        }
    }

    //printf("%d\n",choose );

    if(choose ==-1){
        printf("%s\n","Did not find free blcok" );
        return NULL;
    }

    for(block = sf_free_list_heads[choose].body.links.next; block != &sf_free_list_heads[choose]; block = block->body.links.next){
        if(block->header>=size){
            sf_block *temp =block;
            block->body.links.prev->body.links.next = block->body.links.next;
            block->body.links.next->body.links.prev = block->body.links.prev;

            return temp;
        }
    }


    // printf("%lu\n",temp->header);
    //     printf("%s\n","aaaaaaaaaa");

    return NULL;
}

void addlink(sf_block *ptr,int n){

    sf_block *head =&sf_free_list_heads[n];
    if(head->body.links.next!=&sf_free_list_heads[n]){
        sf_block *p =head->body.links.next;


        while(p != head){
            if(ptr->header < p->header){
                break;
            }
            p = p->body.links.next;
        }

        p->body.links.prev->body.links.next = ptr;
        p->body.links.prev = ptr;
        ptr ->body.links.prev = p->body.links.prev;
        ptr -> body.links.next =p;

    }
    else{
        head->body.links.next = ptr;
        ptr->body.links.prev = head;
        ptr->body.links.next = head;
        head->body.links.prev = ptr;

        // block ->body.links.next = &sf_free_list_heads[7];
        // block ->body.links.prev = &sf_free_list_heads[7];
        // sf_free_list_heads[7].body.links.next = block;
        // sf_free_list_heads[7].body.links.prev = block;

    }


}

void *coalescing(void* target){


        sf_block *freeblock = (sf_block *)(target);//Go to free block header
        size_t free_block_size = freeblock->header & BLOCK_SIZE_MASK;



        sf_footer *prev_footer = (sf_footer *)( (void*)(target));
        size_t prev_size = (*prev_footer ^ sf_magic())& BLOCK_SIZE_MASK;

        sf_block *prev_blcok = (sf_block *)((void*)(target)-prev_size);
        sf_block *next_blcok = (sf_block *)((void*)(target)+free_block_size);

        size_t next_blcok_size = next_blcok->header& BLOCK_SIZE_MASK;


        // printf("%s\n","+++++++++++++++++++++" );
        // printf("%lu\n",free_block_size );
        // printf("%lu\n",prev_blcok_size );
        // printf("%lu\n",next_blcok_size );
        // printf("%s\n","+++++++++++++++++++++" );


        // printf("%s\n","--------------------" );
        // printf("%lu\n",freeblock->header );
        // printf("%lu\n",prev_blcok->header );
        // printf("%lu\n",next_blcok->header );
        // printf("%s\n","--------------------" );



        if((prev_blcok->header & THIS_BLOCK_ALLOCATED) ==2 && (next_blcok->header & THIS_BLOCK_ALLOCATED)==2){
            return target;
        }
        else if((prev_blcok->header & THIS_BLOCK_ALLOCATED) ==2 && (next_blcok->header & THIS_BLOCK_ALLOCATED)!=2){
            next_blcok->body.links.prev->body.links.next = next_blcok->body.links.next;
            next_blcok->body.links.next->body.links.prev = next_blcok->body.links.prev;
            freeblock->body.links.prev->body.links.next = freeblock->body.links.next;
            freeblock->body.links.next->body.links.prev = freeblock->body.links.prev;

            freeblock ->header = freeblock ->header + next_blcok_size ;
            size_t new_size =  (freeblock ->header)& BLOCK_SIZE_MASK;
            sf_footer *new_footer = (sf_footer *)( (void*)(freeblock)+new_size);
            *new_footer = freeblock->header ^sf_magic();


            size_t new_size_seg = find_seg(new_size);
            addlink(freeblock,new_size_seg);

            return ((void*)(freeblock));
        }
        else if((prev_blcok->header & THIS_BLOCK_ALLOCATED) !=2 && (next_blcok->header & THIS_BLOCK_ALLOCATED)==2){
            prev_blcok->body.links.prev->body.links.next = prev_blcok->body.links.next;
            prev_blcok->body.links.next->body.links.prev = prev_blcok->body.links.prev;
            freeblock->body.links.prev->body.links.next = freeblock->body.links.next;
            freeblock->body.links.next->body.links.prev = freeblock->body.links.prev;

            prev_blcok ->header = prev_blcok ->header + free_block_size ;
            size_t new_size =  (prev_blcok ->header & BLOCK_SIZE_MASK );
            sf_footer *new_footer = (sf_footer *)( (void*)(prev_blcok)+new_size);
            *new_footer = prev_blcok->header ^sf_magic();
            // printf("%s\n","HAHAHAHAHAHAAH" );
            // printf("%lu\n", new_size);
            // printf("%s\n","HAHAHAHAHAHAAH" );


            size_t new_size_seg = find_seg(new_size);
            addlink(prev_blcok,new_size_seg);


            return ((void*)(prev_blcok));
        }

        else if((prev_blcok->header & THIS_BLOCK_ALLOCATED) !=2 && (next_blcok->header & THIS_BLOCK_ALLOCATED)!=2){
            prev_blcok->body.links.prev->body.links.next = prev_blcok->body.links.next;
            prev_blcok->body.links.next->body.links.prev = prev_blcok->body.links.prev;
            freeblock->body.links.prev->body.links.next = freeblock->body.links.next;
            freeblock->body.links.next->body.links.prev = freeblock->body.links.prev;
            next_blcok->body.links.prev->body.links.next = next_blcok->body.links.next;
            next_blcok->body.links.next->body.links.prev = next_blcok->body.links.prev;
            prev_blcok ->header = prev_blcok ->header + free_block_size +next_blcok_size;
            size_t new_size =  (prev_blcok ->header)& BLOCK_SIZE_MASK;

            sf_footer *new_footer = (sf_footer *)( (void*)(prev_blcok)+new_size);
            *new_footer = prev_blcok->header ^sf_magic();


            size_t new_size_seg = find_seg(new_size);
            addlink(prev_blcok,new_size_seg);

            return ((void*)(prev_blcok));
        }
        return NULL;

        // freeblock ->prev_footer = alloc_footer;
        // sf_header new_free_block_size = PREV_BLOCK_ALLOCATED + free_block_size - asize;

        // freeblock ->header = new_free_block_size;
}



void *sf_malloc(size_t size) {
    //size_t asize;

    if(size ==0){
    sf_errno = EINVAL;
    return NULL;
    }

    // if (size <= DSIZE) {
    //     asize = 2 * DSIZE;
    // }

    if(sf_mem_start() == sf_mem_end()) {

        for (int i = 0; i < NUM_FREE_LISTS; i++)
        {
            sf_free_list_heads[i].body.links.prev = &sf_free_list_heads[i];
            sf_free_list_heads[i].body.links.next = &sf_free_list_heads[i];
        }

        void *new_page = sf_mem_grow();
        if(new_page ==NULL){
            sf_errno = EINVAL;
            return NULL;
        }
        void *new_mem = sf_mem_start();

//         for(int i = 0; i < NUM_FREE_LISTS; i++){
// sf_free_list_heads[i].body.links.next = sf_free_list_heads[i].body.links.prev = &sf_free_list_heads[i];
// }


        sf_prologue *prologue = (sf_prologue *)new_mem;
        prologue ->padding1 =0;
        prologue ->header =35;
        prologue ->footer =35^sf_magic();

        void *newblock = new_mem+32;

        size_t blcok_size = 4096 - 40 - 8;
        // printf("%zu\n", blcok_size);


        sf_block *block = (sf_block*)newblock;
        block ->prev_footer = prologue -> footer;
        block ->header = blcok_size+PREV_BLOCK_ALLOCATED;
        // block ->body.links.next = &sf_free_list_heads[7];
        // block ->body.links.prev = &sf_free_list_heads[7];


        addlink(block,7);

        // printf("%lu\n",sf_free_list_heads[7].header );
        // printf("%lu\n",sf_free_list_heads[7].body.links.next->header );
        // printf("%lu\n",sf_free_list_heads[7].body.links.next->body.links.next->header );
        // printf("%lu\n",sf_free_list_heads[7].body.links.next->body.links.next->body.links.next->header );




        // sf_free_list_heads[7].body.links.next = block;
        // sf_free_list_heads[7].body.links.prev = block;


        newblock = newblock + 4048;
        sf_footer *blockfooter = (sf_footer*)newblock;
        *blockfooter = block->header ^sf_magic();


        void *end = sf_mem_end()-8;
        sf_epilogue *epilogue = (sf_epilogue *)end;
        epilogue ->header =8 +THIS_BLOCK_ALLOCATED;

    }

    size_t asize;
    asize = size + 16;
    if (asize <= 32){
        asize = 32;
    }
    else{
        asize =size_allign(asize);
    }

    // printf("%s\n","asize" );
    // printf("%lu\n",asize );


     void *place ;
     size_t total_alloc_size = 16+asize;  //add header and footer

    //sf_show_blocks();

    while((place = first_fit(total_alloc_size)) == NULL){

    place = sf_mem_grow();
    if(place == NULL){
        sf_errno = ENOMEM;
        return NULL;
    }

    sf_block  *epi = (sf_block *)(place - 8 -8);
    epi ->header = 4096;
    sf_footer *new_blockfooter = (sf_footer *)( sf_mem_end()- 8 - sizeof(sf_footer)); //Go to block footer
    *new_blockfooter = epi->header ^sf_magic();


    void *end = sf_mem_end()-8;
    sf_epilogue *new_epiloguen = (sf_epilogue *)end;
    new_epiloguen ->header =8+THIS_BLOCK_ALLOCATED;

    int seg_place = find_seg(4096);

    // printf("%lu\n",sf_free_list_heads[7].header );
    // printf("%lu\n",sf_free_list_heads[7].body.links.next->header );
    // printf("%lu\n",sf_free_list_heads[7].body.links.next->body.links.next->header );
    // printf("%lu\n",sf_free_list_heads[7].body.links.next->body.links.next->body.links.next->header );
    sf_block *head =&sf_free_list_heads[seg_place];
    if(head->body.links.next!=&sf_free_list_heads[seg_place]){
        sf_block *p =head->body.links.next;


        while(p != head){
            if(epi->header < p->header){
                break;
            }
            p = p->body.links.next;
        }

        p->body.links.prev->body.links.next = epi;
        p->body.links.prev = epi;
        epi ->body.links.prev = p->body.links.prev;
        epi -> body.links.next =p;

    }
    else{
        head->body.links.next = epi;
        epi->body.links.prev = head;
        epi->body.links.next = head;
        head->body.links.prev = epi;
    }

    place = coalescing((void*)(epi));

}

    sf_block *block = (sf_block *)((void *)place);
    size_t free_block_size = block->header & BLOCK_SIZE_MASK;
    block ->header = asize +PREV_BLOCK_ALLOCATED +THIS_BLOCK_ALLOCATED;
    sf_footer *blockfooter = (sf_footer *)( (void*)(block)  + asize); //Go to block footer
    sf_footer alloc_footer = block->header ^sf_magic();
    *blockfooter = alloc_footer;
    sf_block *freeblock = (sf_block *)( (void*)(blockfooter));//Go to free block header
    freeblock ->prev_footer = alloc_footer;
    sf_header new_free_block_size = PREV_BLOCK_ALLOCATED + free_block_size - asize;

    freeblock ->header = new_free_block_size;

    int seg_place = find_seg(new_free_block_size);

    addlink(freeblock,seg_place);

    // freeblock ->body.links.next = &sf_free_list_heads[seg_place];
    // freeblock ->body.links.prev = &sf_free_list_heads[seg_place];
    // sf_free_list_heads[seg_place].body.links.next = block;
    // sf_free_list_heads[seg_place].body.links.prev = block;

    size_t new_free_block_only_size = freeblock->header & BLOCK_SIZE_MASK;
    sf_footer *freefooter = (sf_footer *)( (void*)(freeblock)  + new_free_block_only_size);//Go to free block footer
    *freefooter = freeblock->header ^sf_magic();
    // printf("%p\n", &freefooter);

    void *result = ((void*)(block)  + 16);
    return result;
}


// void *set_epilogue(void *new_mem){

//     return epilogue;
// }

// void *set_prologue(void *new_mem){

//     return ;
// }

void sf_free(void *pp) {

    if(pp == NULL){
        abort();
    }


    sf_block *block = (sf_block *)(pp-16);

    if( (void *)block < (sf_mem_start()+32)){
        abort();
    }

    if((void *)block >= (sf_mem_end()-8)){
        abort();
    }

    if((block->header &THIS_BLOCK_ALLOCATED) != 2){
        abort();
    }
    if(block->header < 32){
        abort();
    }


    size_t block_size = block->header & BLOCK_SIZE_MASK;
    sf_footer *this_footer = (sf_footer *)( (void*)(block)+block_size);

    sf_footer *prev_footer = (sf_footer *)( (void*)(block));
    size_t prev_size = (*prev_footer ^ sf_magic())& BLOCK_SIZE_MASK;
    sf_block *prev_blcok = (sf_block *)((void*)(block)-prev_size);

    if((block->header & PREV_BLOCK_ALLOCATED)!= 1){
        if((prev_blcok->header & THIS_BLOCK_ALLOCATED) ==2 ){
            abort();
        }
    }

    if((*this_footer ^ sf_magic())!= block->header){
        abort();
    }

    block->header = block->header - THIS_BLOCK_ALLOCATED;
    *this_footer = block->header ^ sf_magic() ;

    sf_block *next_block = (sf_block *)( (void*)(this_footer));//Go to free block header
    next_block->prev_footer = *this_footer;

    if((next_block->header & PREV_BLOCK_ALLOCATED) == 1){
        next_block->header = next_block->header - PREV_BLOCK_ALLOCATED;
        size_t next_block_size = next_block->header & BLOCK_SIZE_MASK;
        sf_footer *next_footer = (sf_footer *)( (void*)(next_block)+next_block_size);
        *next_footer = next_block->header ^ sf_magic() ;
    }

    int seg_num = find_seg(block_size);
    addlink(block,seg_num);
    coalescing((void*)(block));


    return;
}


void *sf_realloc(void *pp, size_t rsize) {

    if(pp == NULL){
        sf_errno = EINVAL;
        return NULL;
    }


    sf_block *block = (sf_block *)(pp-16);

    if( (void *)block < (sf_mem_start()+32)){
        sf_errno = EINVAL;
        return NULL;
    }

    if((void *)block >= (sf_mem_end()-8)){
        sf_errno = EINVAL;
        return NULL;
    }

    if((block->header &THIS_BLOCK_ALLOCATED) != 2){
        sf_errno = EINVAL;
        return NULL;
    }
    if(block->header < 32){
        sf_errno = EINVAL;
        return NULL;
    }

    size_t block_size = block->header & BLOCK_SIZE_MASK;
    sf_footer *this_footer = (sf_footer *)( (void*)(block)+block_size);

    sf_footer *prev_footer = (sf_footer *)( (void*)(block));
    size_t prev_size = (*prev_footer ^ sf_magic())& BLOCK_SIZE_MASK;
    sf_block *prev_blcok = (sf_block *)((void*)(block)-prev_size);

    if((block->header & PREV_BLOCK_ALLOCATED)!= 1){
        if((prev_blcok->header & THIS_BLOCK_ALLOCATED) ==2 ){
            sf_errno = EINVAL;
            return NULL;
        }
    }

    if((*this_footer ^ sf_magic())!= block->header){
        sf_errno = EINVAL;
        return NULL;
    }

    if(rsize < 0){
        sf_errno = EINVAL;
        return NULL;
    }
    if(rsize == 0){
        sf_free(pp);
        return NULL;
    }

    if(rsize == block_size){
        return pp;
    }


    //This is for large size
    if(rsize > block_size){
        void* new_space = sf_malloc(rsize);
        if (new_space == NULL)
        {
            sf_errno = EINVAL;
            return NULL;
        }
        else{
         memcpy((void*)(new_space), (void*)(pp), block_size);
         sf_free(pp);
         return new_space;
        }
    }
    // this is for small size
    else if(rsize < block_size && rsize > 0 ){
        size_t asize = rsize + 16;
        if (asize<32){
            asize = 32;
        }
        else{
            asize = size_allign(asize);
        }


        if(block_size - asize >=32){

            sf_block *alloc_block = (sf_block *)( (void *)(block));
            if((prev_blcok->header & THIS_BLOCK_ALLOCATED) ==2 ){
                alloc_block -> header = asize +1+2 ;
            }
            else{
                alloc_block -> header = asize +2;
            }

        sf_footer *alloc_footer = (sf_footer *)( (void*)(block)+asize);
        size_t alloc_footer_size = (alloc_block -> header ^ sf_magic());
        *alloc_footer  =  alloc_footer_size;

        sf_block *free_block = (sf_block *)( (void *)(alloc_footer));
        free_block -> header = block_size - asize +1;

        sf_footer *free_footer = (sf_footer *)( (void*)(free_block)+block_size - asize);
        size_t free_footer_size = (free_block -> header ^ sf_magic());
        *free_footer  =  free_footer_size;

        size_t new_size_seg = find_seg(block_size - asize);
        addlink(free_block,new_size_seg);
        free_block = coalescing((void*)free_block);

        sf_block *next_block = (sf_block *)( (void *)(free_footer));
        if((next_block ->header & PREV_BLOCK_ALLOCATED) ==1){
            next_block ->header =  next_block ->header -1;
            sf_footer *next_block_footer = (sf_footer *)( (void*)(next_block)+(next_block->header &BLOCK_SIZE_MASK));
            *next_block_footer  =  next_block ->header ^ sf_magic();
        }
        return pp;
        }
        else{
        return pp;
        }
    }

    return NULL;
}

size_t size_allign(size_t size){
    if(size%16 != 0){
            size = size+ (16 - size%16);
            return size;
        }
        else {
            return size;
        }
}
