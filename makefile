VPATH:=src
all:rca
.PHONY:all
rpg:pthread_gemm
	@./pthread_gemm 4 4 4 1
	@./pthread_gemm 4 4 4 4
	@./pthread_gemm 512 512 512 4
	@./pthread_gemm 1024 1024 1024 4
	@./pthread_gemm 1536 1536 1536 4
	@./pthread_gemm 2048 2048 2048 4
	@./pthread_gemm 512 512 512 1
	@./pthread_gemm 1024 1024 1024 1
	@./pthread_gemm 1536 1536 1536 1
	@./pthread_gemm 2048 2048 2048 1
pthread_gemm:pthread_gemm.cpp matrix_tool.cpp
	g++ $^ -g -o $@ -lpthread
pthread_add:pthread_add.cpp
	g++ $^ -g -o $@ -lpthread
rpa:pthread_add
	./pthread_add 4 1
	./pthread_add 4 10
test_nan:test_nan.cpp
	g++ $^ -o $@
get_root:get_root.cpp
	@g++ $^ -g -o $@ -lpthread
rget_root:get_root
	for i in $$(seq 1 20); do \
		./get_root 1 -2 1; \
	done
	for i in $$(seq 1 20); do \
		./get_root 4 5 6; \
	done
	for i in $$(seq 1 20); do \
		./get_root 2 8 3; \
	done
cal_area:cal_area.cpp
	@g++ $^ -g -o $@ -lpthread
rca:cal_area
	for i in $$(seq 1 20); do \
		./cal_area;\
	done

