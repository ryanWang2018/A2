#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#define MAXLINE 256

extern int memsize;

extern int debug;

extern char *tracefile;

extern struct frame *coremap;


int current_index;
int *addr_memory_list;
int track_list_size;

int *track_add_list;




/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
    int i;
    int max_distance = 0;
    int max_distance_item = -1;
    int j = current_index;
    // loop through addre the in memory, and try to find a i that has
    // max distance according to the opt algorithm.
    for(i = 0; i<memsize; i++){

        int exit = 1;
        // loop through the rest of addr_meory_list;
        while (exit == 1 || j < track_list_size){
            // if the j'th address is equal to the current i'th address
            // compare with the max_distance
            if(track_add_list[j] == addr_memory_list[i]){
                // if it is bigger than the max_distance
                if (j > max_distance){
                    max_distance = j;
                    max_distance_item = i;
                    exit = 0;
                }
            }
            j++;
        }
        // if exit if false, we did not find i'th address in the rest track_add_list
        // then this address will not be accessed in the future
        if(exit == 1){
            return i; 
        }
        
    }
    addr_memory_list[max_distance_item] = track_add_list[current_index];
	
	return max_distance_item;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {
    current_index += 1;
    int current_add = p->frame >> PAGE_SHIFT;
    if(current_index < memsize){
        addr_memory_list[current_add] = track_add_list[current_index];
    }
	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {
    FILE *track_file;
    FILE *track_file1;
    char buf[MAXLINE];
    track_list_size = 0;

    addr_t vaddr = 0;
    current_index = 0;

    addr_memory_list = malloc(memsize * sizeof(int));

    char type;

    if ((track_file = fopen(tracefile, "r")) == NULL){
        perror("Error for reading tracefile");
        exit(1);
    }

    while(fgets(buf, MAXLINE, track_file) != NULL){
        track_list_size++;

    }
    fclose(track_file);

    track_add_list = malloc(track_list_size * sizeof(int));

    if ((track_file1 = fopen(tracefile, "r")) == NULL){
        perror("Error for reading tracefile");
        exit(1);
    }
    int index = 0;
    while(fgets(buf, MAXLINE, track_file1) != NULL){
        if(buf[0] != '='){
            sscanf(buf, "%c %lx", &type, &vaddr);
            track_add_list[index] = vaddr;
            index += 1;
        }

    }
    fclose(track_file1);

}

