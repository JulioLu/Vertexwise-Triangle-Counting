#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<cilk/cilk.h>
#include<pthread.h>
#include"reader-converter/mmio.c"
#include"reader-converter/readerconverter.c"

struct timespec t_start, t_end;
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

int *V4_cilk(int *row, int *col, int N);

int main(int argc, char* argv[]){
    
    char *str = argv[1];
    int  *CSCrows;
    int  *CSCcols;

    int rowptrSize = cooReader(str, &CSCrows, &CSCcols);

    V4_cilk(CSCcols, CSCrows, rowptrSize);

    return 0;
}

int *V4_cilk(int *row, int *col, int N){

    int tr = 0;
    int *c3 = (int *)calloc(N, sizeof(int));


    clock_gettime(CLOCK_REALTIME, &t_start);

    cilk_for (int i=0; i<N; i++) {
        cilk_for (int j=row[i]; j<row[i+1]; j++) {

            int common = 0;
            int p1     = 0;
            int p2     = 0;

            while(row[i]+p1 < row[i+1] && row[col[j]]+p2 < row[col[j]+1])
            {
                if(col[row[i]+p1] < col[row[col[j]]+p2]){
                    p1++;
                }
                else if(col[row[i]+p1] > col[row[col[j]]+p2]){
                    p2++;
                }
                else{

                    c3[col[row[i]+p1]]++;
                    c3[i]++;
                    c3[col[j]]++;
                    p1++;
                    p2++;
                    common++;
                }
            }
        }
    }
    
    clock_gettime(CLOCK_REALTIME, &t_end);

    for(int i=0; i<N; i++) tr += c3[i];

    double duration = ((t_end.tv_sec - t_start.tv_sec) * 1000000 + (t_end.tv_nsec - t_start.tv_nsec) / 1000) / 1000000.0;
    printf("~ Triangles  : %d\n",tr/3);
    printf("~ V3 duration: %lf seconds\n", duration);

    return c3;

}