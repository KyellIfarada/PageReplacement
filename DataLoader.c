//
// Created by lorenzo on 4/26/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataLoader.h"


/*
struct test_scenario
 {
    int refstr_len;
    int refstr[512];
    int page_count;
    int frame_count;
};

 * */

/**
 * Loads a test_scenario strut from a textfile.
 *
 * @param filename The name of the file to load.
 * @return A struct containing the loaded file.
 */
struct test_scenario* load_test_data(char* filename)
{
    struct test_scenario* File_Load = (struct test_scenario*)malloc(sizeof(struct test_scenario));
    FILE*  FilePtr;
    int c;
    FilePtr = fopen(filename, "r"); // read only
    if(FilePtr == NULL)
    {
        perror("Error in opening file");
    }
    do{
        if( feof(FilePtr))
        {
            break;
        }
        else {
            c = fgetc(FilePtr);                                   //Check if newline character is an issue
            File_Load->page_count = c;
            c = fgetc(FilePtr);
            File_Load->frame_count = c;
            c = fgetc(FilePtr);
            File_Load->refstr_len = c;
            for (int i = 0; i < File_Load->refstr_len; i++) {
                c = fgetc(FilePtr);
                File_Load->refstr[i] = c;
            }
        }
    }
    while(1);
    fclose(FilePtr);
    return File_Load;

}