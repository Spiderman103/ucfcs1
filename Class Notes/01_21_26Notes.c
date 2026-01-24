
#include <stdio.h>
#include <stdlib.h>

//Dynamic Memory Allocation Review Notes

int main() {

    int a; //integer type variable
    int *pa; //pointer to some integer type address

    pa = &a; //assign address of a to pointer pa.

    int *pb;
    pb = (int *) malloc(sizeof(int)); //Asking to allocate memory for an integer type.

    /* All Different Memory Allocation Functions
        malloc() - memory allocation
        calloc() 
        realloc()
        free() - After you are done using the dynamically allocated memory, you should free it to avoid memory leaks.
            - After freeing malloc, calloc, or realloc calls, check if the pointer is equal to NULL
            - IF equal to Null, all good and dynamic memory is all clear
                -  IF NOT equal to NULL, memory leaks occur (USE VALGRIND TO CHECK FOR MEMORY LEAKS).
    */

    int arr[10];

    int *darr;
    darr = malloc(10 * sizeof(int));

    /* Memory using Realloc()
        10 3 5 2 9 18 20 25 3 11
        Elements in Dynamic Array

        darr = (int *) realloc(15 * sizeof(int))
    */


    /* Memory using Calloc()

       10 3 5 2 9 18 20 25 3 11
        Elements in Dynamic Array

        darr = (int *) calloc(10, sizeof(int))
    
    */


    /* Dynamically Allocating Structures
    
        typedef struct {
            int a;
            char name[10];
        } data;

        data x;
        printf("%d", x.a);

        data *p = (data *) malloc(n * sizeof(data));


        Static Approach for Arrays of Structs
            data x[100]; Array of type data structures (Allocatin total 1400 bytes).

        Dynamic Approach for Arrays of Structs
            data *p = (data *) malloc(n * sizeof(data));
            
    */


    /* Dynamically Allocating 2D Arrays
    
        int **arr2;

        arr2 = malloc(n * sizeof(int));

        for(int i = 0; i < n; ++i)
        {
            arr2[i] = malloc(m * sizeof(int));
            for(int j = 0; j < m; ++j)
            {
                arr2[i][j]
            }

        }

        For referencing a component in a struct
            -  If the array of structs is dynamic, USE ARROW ->
            - If the array of structs is static, USE DOT .
    */

    printf("Hello World\n");

    return 0;
}