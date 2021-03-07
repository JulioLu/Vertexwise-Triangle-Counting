
hello_openmp:
	
	@printf " \n\n~~~~~~~~~~V3-Sequential~~~~~~~~~~\n"
	gcc -O3 -o V3 V3.c
	./V3 data/dblp-2010.mtx
	@printf " \n\n~~~~~~~~~~V3-OpenMP~~~~~~~~~~\n"
	gcc -O3 -o V3_OpenMP V3_OpenMP.c -fopenmp
	./V3_OpenMP data/dblp-2010.mtx 4
	@#@printf " \n\n~~~~~~~~~~V3-OpenCilk~~~~~~~~~~\n"
	@#gcc -O3 -o V3_OpenCilk V3_OpenCilk.c -fcilkplus
	@#./V3_OpenCilk data/dblp-2010.mtx
	
	
	@printf "\n\n\n\n~~~~~~~~~~V4-Sequential~~~~~~~~~~\n"
	gcc -O3 -o V4 V4.c
	./V4 data/dblp-2010.mtx
	@printf " \n\n\n~~~~~~~~~~V4-pthreads~~~~~~~~~~\n"
	gcc -O3 -o V4_pthreads V4_pthreads.c -lpthread
	./V4_pthreads data/dblp-2010.mtx 8
	@printf "\n\n\n~~~~~~~~~~V4-OpenMP~~~~~~~~~~\n"
	gcc -O3 -o V4_OpenMP V4_OpenMP.c -fopenmp
	./V4_OpenMP data/dblp-2010.mtx 4
	@#@printf "\n\n\n~~~~~~~~~~V4-OpenCilk~~~~~~~~~~\n"
	@#gcc -O3 -o V4_OpenCilk V4_OpenCilk.c -fcilkplus
	@#./V4_OpenCilk data/dblp-2010.mtx
