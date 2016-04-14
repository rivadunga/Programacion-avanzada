#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int i, j = 0;
    int a[6];
    int sum = 0;

   #pragma omp parallel shared(sum) private(j)
    {
           #pragma omp sections
        {
                   #pragma omp section
            {
                omp_set_nested(1);
                           #pragma omp parallel num_threads(4) shared(i)
                {
                                   #pragma omp critical
                    {
                        printf("i :: %d\n", i);
                        ++i;
                    }

                                   #pragma omp master
                    printf("T :: %d\n", omp_get_num_threads());
                }
            }

                   #pragma omp section
            {
                           #pragma omp parallel for
                for (j = 0; j < 6; ++j)
                {
                    a[j] = rand() % 10;
                    printf("a[%d] :: %d\n", j,a[j]);
                    sum       += a[j];
                }
            }
        }
    }

    printf("SUM :: %d\n", sum);

    return 0;
}
