#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /* code */
    double A[5] = {
        [0] = 9.0,
        [1] = 2.9,
        [4] = 3.E+25,
        [3] = .00007,
    };

    for (size_t i = 0; i < 5; i++)
    {
        printf("element %zu is %g, \t its queare is %g\n", i, A[i], A[i]*A[i]);
    }
    
    return EXIT_SUCCESS;
}
