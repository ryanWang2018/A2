#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

/*
The LRU algorithm: it is necessary to maintain a linked list of all pages in memory, with
the most recently used page at the front and hte least recenty used page at the reat.
or reverse in order to simplify the implementation.

*/

int *ls;
int index;

int lru_evict() {
    int delete = ls[0];
    int i;
    for (i = 1; i < memsize; i++){
        ls[i-1] = ls[i];
    }
    index--;
    return delete;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
    int frame_number = p->frame >> PAGE_SHIFT;

    int i; 
    // loop through ls, if we can find the frame number that is already in the list
    // we move this frame to the end list as the newly accessed.
    for (i = 0; i < index; i++ ){
        if(ls[i] == frame_number){
            int j;
            for(j = i; j<index; j++){
                ls[j] = ls[j+1];

            }
            ls[index-1] = frame_number;
            return;
        }

    }
    ls[index+1] = frame_number;
    index++;

	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
    index = 0;
    ls = malloc(sizeof(int) * memsize);

}
