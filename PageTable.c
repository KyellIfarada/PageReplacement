//
// Created by lorenzo on 4/26/23.
//

// enumeration to represent replacement algorithms.

#include <stdio.h>
#include <stdlib.h>

#include "DataLoader.c"
#include "PageTable.h"


//forward declarations for structs
struct page_table_entry;
struct page_table;

/*
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of frames.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */

struct page_table_entry
{
    int page_frame;
    int page_number;
    int page_value;
    unsigned int bit ;
    int frequency;
};

struct page_table
{
    int page_count;
    int frame_count;
    int algorithm;
    unsigned int verbose;
    int page_faults;
    struct page_table_entry QueueArray[512];
    int FrameArray[];
};

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose)
{
          struct page_table* PageTable =(struct page_table*)malloc(sizeof(struct page_table));
          PageTable->page_count = page_count;
          PageTable->frame_count = frame_count;
          PageTable->algorithm = algorithm;
          PageTable->verbose = verbose;

          return PageTable;
};

/**
 * Destroys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt)
{
    struct page_table_entry* iter = (*pt)->QueueArray;

    while(iter != NULL )
    {
        struct page_table_entry* tmp = iter++;
        free(iter);

    }
    free (*pt);
    *pt = NULL;

};
/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page)
{

    //Declare Statics to Be Utilized
    int freeslots = 0;
    int timer = 0;

    static int i,j,count, k=0;
    static int w = 0;
    static int PageFaultFiFo =       0;
    static int PageFaultLRU =        0;
    static int  PageFaultMFU    =    0;
    static int FrameLoop = 1;
    struct page_table_entry* PageEntry= (struct page_table_entry*)malloc(sizeof(struct page_table_entry));
    //Declare Default FrameArray to be modified by Replacement Methods
    for(int h = 0; h < pt->frame_count; h++)
    {
        pt->FrameArray[h]=-1;
    }
    //Switch between algorithms based on table algorithm
    switch (pt->algorithm) {
        case 0:

            //FIFO
            //Assign Page_Entry it's value and Turn on Bit.
            PageEntry->page_value = page;
            PageEntry->bit = 01;
            pt->QueueArray[i] = *PageEntry;
            //Find Size of QueueArray
            for (int y = 0; y < i; y++) {
                count++;
            }

            //Find if Page is Availible in FrameArray
            int exists = 0;
            for (int z = 0; z < pt->frame_count; z++) {
                if (pt->FrameArray[z] == PageEntry->page_value) {
                    exists = 1;
                }
            }
            //If Page is not Availible in FrameArray ; add it.
            if (exists == 0) {

                //Place with Page Entry
                pt->FrameArray[w] = PageEntry->page_value;
                FrameLoop++;
                //If you happen to re-loop in you frame turn off bit of victim page
                if (FrameLoop > pt->frame_count) {
                    int o = w;
                    FrameLoop % pt->frame_count;
                    for (int d = 0; d < count; d++) {
                        if (pt->QueueArray[d].page_value == pt->FrameArray[(o + 1) % pt->frame_count]) {
                            pt->QueueArray[d].bit = 00;
                        }
                    }
                }
                //Move  Position for next replacement in Frame
                w = (w + 1) % pt->frame_count;

                //Record PageFault
                PageFaultFiFo++;

            }
            pt->page_faults = PageFaultFiFo;
            PageEntry->page_number = i;


            //Assign PageFrame
            for(int z = 0 ; z < pt->frame_count; z++)

            {
                if(pt->FrameArray[z] == pt->QueueArray[i].page_value)
                    pt->QueueArray[i].page_frame = z;

            }
            i++;
            break;


        case 1:

            //LRU
            PageEntry->page_value = page;
            PageEntry->bit = 01;
            pt->QueueArray[i] = *PageEntry;
            PageEntry->page_number = j;

            int *UsedUp = ((int *) malloc(pt->frame_count * sizeof(int)));


            int AlertManifest = 0;
            //Checks if Already in Frame
            for (int x = 0; x < pt->frame_count; x++) {
                if (pt->FrameArray[x] == PageEntry->page_value) {
                    UsedUp[x] = timer;
                    AlertManifest = 1;
                }
            }
            if (AlertManifest == 1) {
                PageFaultLRU++;
                // Empty Spaces in Frame? Then
                if (freeslots < pt->frame_count) {
                    pt->FrameArray[freeslots] = PageEntry->page_value;
                    UsedUp[freeslots] = timer;
                    freeslots++;

                }

                    //else
                else {
                    int minimum = UsedUp[0];
                    int ThisIndex = 0;
                    int t;
                    //locate index of UsedUp Page
                    for (t = 0; t < pt->frame_count; t++) {
                        if (UsedUp[t] < minimum) {
                            minimum = UsedUp[t];
                            ThisIndex = t;

                        }
                    }

                    //FYI THIS PORTION IS NOT DONE
                    // get rid of page just used and get current page
                    // store page getting removed
                    for(int r =0; r < j ; r++)
                    if (UsedUp[ThisIndex] == pt->QueueArray[j].page_value)
                    {
                        pt->QueueArray[j].page_frame = pt->FrameArray[ThisIndex];
                    }
                    // replace page removed with current page
                    pt->FrameArray[ThisIndex] = PageEntry->page_value;
                     UsedUp[ThisIndex] = timer;

                }


            }
            timer++;
            j++;
            break;


        case 2:



            //MFU
            PageEntry->page_value = page;
            PageEntry->bit = 01;
            pt->QueueArray[k] = *PageEntry;
            PageEntry->page_number = k;
            for(int a = 0; a < k; a++)
            {
                //Page Found
                int v;
                int store;
                for(v=0;v<pt->frame_count;v++)
                    if(pt->FrameArray[v] == PageEntry->page_value)
                        store = PageEntry->page_value;
                    else
                        store = -1;


                if(store == -1)   // PageFault Happens
                {
                    int iter;
                    for (iter=0; iter<= pt->frame_count; iter++)
                        if (pt->FrameArray[iter] ==-1)
                        {
                            store = iter;
                        }
                        else
                        {
                                store == -1;
                        }
                    if(store ==-1)
                    {
                        int queueiter;
                        int frameiter;
                        int MyNum=0;
                        for (frameiter=1; frameiter<pt->frame_count; frameiter++)
                            for(queueiter=0; queueiter < k; queueiter++ ){
                            if(pt->FrameArray[frameiter] == pt->QueueArray[k].page_value) {
                                if (pt->QueueArray[k].frequency > pt->QueueArray[MyNum].frequency)
                                {
                                    MyNum = frameiter;
                                }

                            }

                            }
                        store= MyNum;
                    }
                    PageFaultMFU++;
                    pt->FrameArray[store] = pt->QueueArray[k].page_value;
                    pt->QueueArray[k].frequency= 1;


                }
                else{
                    for(int t; t < k  ; t++) {
                        if (pt->FrameArray[store] == pt->QueueArray[k].page_value)
                        {
                            pt->QueueArray[k].frequency++;
                        }

                    }

                }



            }

            for( int b =0; b< k ; k++)
            {
                for(int e ; e < pt->frame_count; e++) {
                    if (pt->FrameArray[e] == pt->QueueArray[b].page_value)
                        pt->QueueArray[b].page_frame = e ;
                }
            }
            k++;
            break;


    }
};
/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt) {

   switch(pt->algorithm) {
       case 0:
           printf("===Page Table===\n");
           printf("Mod: FIFO");
           printf("Page Fault: \n");
           printf("page frame | dirty valid\n ");
           for (int p = 0; p < pt->page_count; p++) {
               printf("%d      %d,      %01d\n",  pt->QueueArray[p].page_number,  pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
           };
       case 1:
           printf("===Page Table===\n");
           printf("Mod: LRU");
           printf("Page Fault: \n");
           printf("page frame | dirty valid\n ");
           for (int p = 0; p < pt->page_count; p++) {
               printf("%d      %d,      %01d\n",  pt->QueueArray[p].page_number,  pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
           };
           break;

       case 2 :
           printf("===Page Table===\n");
           printf("Mod: MFU");
           printf("Page Fault: \n");
           printf("page frame | dirty valid\n ");
           for (int p = 0; p < pt->page_count; p++) {
               printf("%d      %d,      %01d\n",  pt->QueueArray[p].page_number,  pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
           };
           break;
   }



}

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt)
{
    switch(pt->algorithm) {
        case 0:

            printf("page frame | dirty valid\n ");
            for (int p = 0; p < pt->page_count; p++) {
                printf("%d      %d,      %01d\n", pt->QueueArray[p].page_number,  pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
            };
        case 1:

            printf("page frame | dirty valid\n ");
            for (int p = 0; p < pt->page_count; p++) {
                printf("%d      %d,      %01d\n", pt->QueueArray[p].page_number, pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
            };
            break;

        case 2 :

            printf("page frame | dirty valid\n ");
            for (int p = 0; p < pt->page_count; p++) {

                printf("%d      %d,      %01d\n", pt->QueueArray[p].page_number,  pt->QueueArray[p].page_frame, pt->QueueArray[p].bit);
            };
            break;
    }

};




