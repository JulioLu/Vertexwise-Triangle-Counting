#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<cilk/cilk.h>
#include<pthread.h>
#include"reader-converter/mmio.c"
#include"reader-converter/readerconverter.c"

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
struct timespec t_start, t_end;

int* V3_cilk(int* row, int* col, int N);

void main(int argc, char* argv[]){
    
    char *str = argv[1];
    int  *CSCrows;
    int  *CSCcols;

    int rowptrSize = cooReader(str, &CSCrows, &CSCcols);

    V3_cilk(CSCcols, CSCrows, rowptrSize);

}

int* V3_cilk(int* row, int* col, int N){

    int *colPsearch;
    int b, j        = 0;
    int colend      = 0;
    int firstNode   = 0;
    int secondNode  = 0;
    int tr          = 0;

    int *c3 = (int *)calloc(N, sizeof(int));
    
    
    clock_gettime(CLOCK_REALTIME, &t_start);

    cilk_for (int i=0; i<N; i++)
    {
        for(int e=row[i]; e<row[i+1]; e++)
        {
            for(int j=row[i]; j<row[i+1]; j++)
            {
                for(int z=row[col[e]]; z<row[col[e]+1]; z++)
                {
                    if(col[z] == col[j])
                    {
                        pthread_mutex_lock(&mux);
                        c3[i]++;
                        pthread_mutex_unlock(&mux);
                        
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
    printf("TRIANGLES == %d\n",tr/3);
    printf("Duration: %f\n", duration);

    return c3;

}