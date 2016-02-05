#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "ipa.h"
#include "interval.h"

/* Private function for counting lines in a file */
size_t nlines(FILE * fp){
    rewind(fp);
    char * lineptr = NULL;
    size_t n = 0;
    size_t nlines = 0;
    while(getline(&lineptr, &n, fp) != EOF){
        nlines++; 
    }
    rewind(fp);
    free(lineptr);
    return nlines;
}

IPA * load_intervals(char * filename){
    FILE * int_file;
    unsigned int start, stop;
    int_file = fopen(filename, "r");

    if(int_file == NULL){
        printf("Cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    IPA * ipa = init_ipa();

    ipa->size = nlines(int_file);
    ipa->v = (Interval*)malloc(ipa->size * sizeof(Interval));

    /* WARNING: This loop assumes the file is formated with one pair of
     * integers on each line. No checking for this is done. If this function is
     * ported to production code, be sure to implement a check. */
    for(int i = 0; fscanf(int_file, "%d\t%d", &start, &stop) != EOF; i++){
        ipa->v[i].start = start;
        ipa->v[i].stop = stop;
    }

    fclose(int_file);

    return(ipa);
}
