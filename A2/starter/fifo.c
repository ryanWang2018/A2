#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int *queue;
int index;
int fifo_evict() {
    int res = queue[0];
    int i;
    for(i = 0 ; i < memsize-1; i ++){
        queue[i] = queue[i+1];

    }
    index--;
	
	return res;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {
    int i;
    for(i = 0; i<index; i++){
        if(queue[i] == (p->frame >> PAGE_SHIFT)){
            return;
        }
    }
    queue[index] = p->frame >> PAGE_SHIFT;
    index++;

	
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {
    queue = malloc(sizeof(int) * memsize);
    index = 0;

}
