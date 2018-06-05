#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
void printPowerSet(int *set, int set_size)
{
    //set_size of power set of a set with set_size n is (2**n -1)
    unsigned int pow_set_size = pow(2, set_size);
    int counter, j;

    printf("#const n = %d.\n", set_size);
    printf("#const p = %d.\n", pow_set_size - 1);

    /*Run from counter 000..0 to 111..1*/
    for(counter = 1; counter < pow_set_size; counter++)
    {
      for(j = 0; j < set_size; j++)
       {
          /* Check if jth bit in the counter is set
             If set then pront jth element from set */
          if(counter & (1<<j))
            printf("powerSet(%d,%d).\n", counter, set[j]);
       }
    }
}
 
/*Driver program to test printPowerSet*/
int main( int argc, char *argv[] )
{

    int size;

    if( argc == 2 )
    {
      size = atoi(argv[1]);
    }
    else if( argc > 2 ) {
      printf("Too many arguments supplied. Only one argument expected: number of elements.\n");
      return -1;
   }
   else {
      printf("One argument expected: number of elements.\n");
      return -1;
   }

    int * set = (int*) malloc(size * sizeof(int));
    for(int i = 0; i < size; i++)
    {
        set[i] = i+1;
    }
    printPowerSet(set, size);
    free(set);
    return 0;
}