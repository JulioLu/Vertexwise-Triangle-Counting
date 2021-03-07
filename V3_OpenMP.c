#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include"reader-converter/mmio.c"
#include"reader-converter/readerconverter.c"

struct timespec t_start, t_end;

int* V3_omp(int* row, int* col, int N, int number_of_threads);

int main(int argc, char* argv[]){
    
    char *str           = argv[1];
    int   threadnum     = atoi(argv[2]);

    int *CSCrows;
    int *CSCcols;

    int rowptrSize = cooReader(str,  &CSCrows, &CSCcols);

    V3_omp(CSCcols, CSCrows, rowptrSize, threadnum);

    return 0;
}

int* V3_omp(int* row, int* col, int N, int number_of_threads){

    int tr = 0;
    int *c3 = (int *)calloc(N, sizeof(int));

    omp_set_num_threads(number_of_threads);
    
    #pragma omp parallel
    {
        int *colPsearch;
        int b, j        = 0;
        int colend      = 0;
        int firstNode   = 0;
        int secondNode  = 0;
        
        clock_gettime(CLOCK_REALTIME, &t_start);

        #pragma omp for nowait schedule(dynamic) reduction(+:c3[:N])
        for(int i=0; i<N; i++){

            for(int e=row[i]; e<row[i+1]; e++){

                for(int j=row[i]; j<row[i+1]; j++){

                    colPsearch = col + row[col[e]];
                    for(int z=row[col[e]]; z<row[col[e]+1]; z++){

                        if(*colPsearch == col[j]){
                            
                            c3[i]++;
                            c3[col[e]]++;
                            c3[col[j]]++;
                            break;
                        }
                        colPsearch++;
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
