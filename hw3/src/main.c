#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    // sf_errno = 0;
    // sf_malloc(3 * PAGE_SZ - 2 * sizeof(sf_block));

    // printf("%lu\n",sizeof(sf_block) );
    // sf_show_blocks();


   sf_malloc(3 * PAGE_SZ - 2 * sizeof(sf_block));

    sf_show_heap();


    sf_mem_fini();

    return EXIT_SUCCESS;
}
