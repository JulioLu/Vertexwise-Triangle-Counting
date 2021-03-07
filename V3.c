#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"reader-converter/mmio.c"
#include"reader-converter/readerconverter.c"

struct timespec t_start, t_end;

int* V3(int* row, int* col, int N);

void main(int argc, char *argv[]){

    char *str = argv[1];
    int  *CSCrows;
    int  *CSCcols;

    int rowptrSize = cooReader(str,  &CSCrows, &CSCcols);

    V3(CSCcols, CSCrows, rowptrSize);

    free(CSCrows);
    free(CSCcols);

}

int* V3(int* row, int* col, int N){

    int tr = 0;
    int *colPsearch;
    int *c3 = (int *)calloc(N, sizeof(int));

    clock_gettime(CLOCK_REALTIME, &t_start);

    for(int i=0; i<N; i++)
    {
        for(int e=row[i]; e<row[i+1]; e++)
        {
            for(int j=row[i]; j<row[i+1]; j++)
            {
                for(int z=row[col[e]]; z<row[col[e]+1]; z++)
                {
                    if(col[z] == col[j])
                    {    
                        c3[i]++;
                        c3[col[e]]++;
                        c3[col[j]]++;
                        break;
                    }
                }
            }
        }
    }
    
    clock_gettime(CLOCK_REALTIME, &t_end);

    for(int i=0; i<N; i++) tr+=c3[i];
    
    double duration = ((t_end.tv_sec - t_start.tv_sec) * 1000000 + (t_end.tv_nsec - t_start.tv_nsec) / 1000) / 1000000.0;
    printf("~ Triangles  : %d\n",tr/3);
    printf("~ V3 duration: %lf seconds\n", duration);

    
    return c3;

}
