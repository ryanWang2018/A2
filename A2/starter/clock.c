#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

/*
Clock replacement algorithm: keep all the page frames on a circular list in the form of a clock,
    a hand points to the oldest page.(when a page fault occurs, the page the hand is opinting to is inspected)
    the action taken depends on the R bit:
        R = 0; evict the page
        R = 1; clear R and advance hand.

    when a page fault occurs, the page being pointed to by the hand is inspected. if its R bit is 0, the page is evicted,
    the new page is inserted into the clock in its place, and the hand is advanced one position. If R is 1, it is cleared and the hand is advanced to the next page.
    this process is repeated until a page is found with R = 0. It differs from second chancec onlt in the implementaton.

*/


int *lst;
int index;

int clock_evict() {
    int i = 0;
    pgtbl_entry_t *p;

    while(i<index){
        p = coremap[stack[i]].pte;
        int R = p->frame & PG_REF;
        if(R == 1){
            // clear R and move the hand to the next page
            p->frame = p->frame & (~PG_REF);

        } else {
            // R is 0, evict this page
            int j;
            for(j = i; j<memsize-1; j++){
                lst[j] = lst[j+1];
            }
            index --;
            return lst[i];
        }

        i++;
        i = i % index;

    }
	
	return 0;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {


	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
    lst = malloc(sizeof(int) * memsize);
    index = 0;
}
