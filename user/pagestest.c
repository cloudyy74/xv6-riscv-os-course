#include "kernel/types.h"
#include "user/user.h"
#include "kernel/riscv.h"

int var1;

int main() {
    printf("start memory\n");
    if (pages(0, 0, FLAG_ALL) < 0) {
        fprintf(2, "pages failed\n");
        exit(1);
    }
    
    printf("\nalloc %ldbytes on heap\n", 10000 * sizeof(int));
    int* heap_arr = (int*)malloc(10000 * sizeof(int));
    if (heap_arr < 0) {
        fprintf(2, "malloc failed\n");
        exit(1);
    }
    if (pages(0, 0, FLAG_ALL) < 0) {
        fprintf(2, "pages failed\n");
        exit(1);
    }

    int var2;
    
    printf("\nA and D pages after remove all flags\n");
    rmflags(0, 0, FLAG_D | FLAG_A);
    if (pages(0, 0, FLAG_D | FLAG_A) < 0) {
        fprintf(2, "pages failed\n");
        exit(1);
    };

    printf("\nbefore write and read to the global\n");
    if (pages(&var1, sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages failed\n");
        exit(-1);
    }

    printf("\nafter read to the global\n");
    var2 = var1;
    if (pages(&var1, sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages failed\n");
        exit(-1);
    }

    printf("\nafter write to the global\n");
    var1 = 0;
    if (pages(&var1, sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages failed\n");
        exit(-1);
    }

    printf("\nafter write and read to the stack\n");
    var2 = 1;
    var1 = var2;
    if (pages(&var2, sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages failed\n");
        exit(-1);
    }

    printf("\nbefore read and write to heap\n");
    if (pages(heap_arr, 10000 * sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages error\n");
        exit(1);
    }

    printf("\nafter read and write to heap (other pages)\n");
    heap_arr[0] = heap_arr[3000];
    if (pages(heap_arr, 10000 * sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages error\n");
        exit(1);
    }

    printf("\nremove all flags from heap\n");
    rmflags(heap_arr, 10000 * sizeof(int), FLAG_D | FLAG_A);
    if (pages(heap_arr, 10000 * sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages error\n");
        exit(1);
    }

    printf("\nfree heap memory\n");
    free(heap_arr);
    if (pages(0, 0, FLAG_ALL) < 0) {
        fprintf(2, "pages failed\n");
        exit(1);
    }

    char stack_arr[3000];

    printf("\nremove all flags from stack array\n");
    rmflags(stack_arr, 3000 * sizeof(int), FLAG_D | FLAG_A);
    if (pages(stack_arr, 3000 * sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages error\n");
        exit(1);
    }

    printf("\nwrite to stack array\n");
    stack_arr[2500] = 1;
    if (pages(stack_arr, 3000 * sizeof(int), FLAG_ALL)) {
        fprintf(2, "pages error\n");
        exit(1);
    }

    exit(0);
}
