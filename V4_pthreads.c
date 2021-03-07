#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include"reader-converter/mmio.c"
#include"reader-converter/readerconverter.c"


int TRIANGLES     = 0;
double TOTAL_TIME = 0;
struct timespec t_start, t_end;
struct thread_args{
    
    int *rows;
    int *cols;
    int rowPtrSize;
    int tid;
    int threadnum;

};

void *V4_pthreads(void *arg);

void main(int argc, char *argv[]){
    
    char* str = argv[1];
    int tnum = atoi(argv[2]);

    pthread_t          *threads = (pthread_t *)malloc(tnum * sizeof(pthread_t));
    struct thread_args *targs   = (struct thread_args *)malloc(tnum * sizeof(struct thread_args));

    int *CSCrows;
    int *CSCcols;

    u_int32_t N  = cooReader(str, &CSCrows, &CSCcols);
    

    /* Initialization */

    for(int i=0, k=0; i<tnum; i++)
    {
        targs[i].rows       = CSCcols;
        targs[i].cols       = CSCrows;
        targs[i].tid        = i;
        targs[i].rowPtrSize = N;
        targs[i].threadnum  = tnum;

        pthread_create(&threads[i], NULL, V4_pthreads, (void *)&targs[i]);
    }
    
    for(int l=0; l<tnum; l++)
        pthread_join(threads[l], NULL);


    printf("Total duration : %lf\n", TOTAL_TIME);
    printf("Total triangles: %d\n", TRIANGLES);
   
}

void *V4_pthreads(void *arg){
       
    struct thread_args *targs = arg;
    int *row   = targs -> rows;
    int *col   = targs -> cols;
    int N      = targs -> rowPtrSize;
    int id     = targs -> tid;
    int nthrds = targs -> threadnum;

    int *c3 = (int *)calloc(N, sizeof(int));

    int tr          = 0;
    int bound       = 0;
    int extrabound  = 0;
    int threadStart = 0;
    int threadEnd   = 0;
    int t_ptr       = 0;

    if(nthrds > N)
    {
        printf("Number of threads greater than number of iterations!");
        exit(0);
    }

    /************************************
    * DISTRIBUTE THE ITERATIONS EQUALLY *
    ************************************/

   if(N % nthrds == 0)
    {
        bound      = N / nthrds;
        extrabound = 0;
    }
    else
    {
        bound      = N / nthrds;
        extrabound = N % nthrds;
    }

    if(t_ptr != nthrds - 1 || nthrds == 1)
    {
        threadStart = id * bound;
        threadEnd   = threadStart + bound;
    }

    if(t_ptr == nthrds - 1 && nthrds != 1)
    {
        threadStart = N - extrabound;
        threadEnd   = N;
    }

    t_ptr++;

    clock_gettime(CLOCK_REALTIME, &t_start);

    for(int i = threadStart; i < threadEnd; i++)
    {
        for(int j=row[i]; j<row[i+1]; j++)
        {
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
                else {

                    c3[col[row[i]+p1]]++;
                    c3[i]++;
                    c3[col[j]]++;

                    p1++;
                    p2++;
                    common++;
                }
            }
            tr += common;
        }
    }
    
    clock_gettime(CLOCK_REALTIME, &t_end);

    double duration = ((t_end.tv_sec - t_start.tv_sec) * 1000000 + (t_end.tv_nsec - t_start.tv_nsec) / 1000) / 1000000.0;
    printf("Thread(%d) duration: %f\tseconds\n", id, duration);
    printf("Thread(%d) detected: %d\ttriangles\n\n", id, tr);

    TOTAL_TIME += duration;
    TRIANGLES  += tr;

    pthread_exit(NULL);

    return (void *)c3;

}