SRCS1 := apc_func.c apc_main.c
TRGT1 := arbitrary_precision_arithmetic_calculator

${TRGT1} : ${SRCS1}
	gcc $^ -o $@

clean :
	rm -f ${TRGT1}
