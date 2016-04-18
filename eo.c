#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define M    10

int main()
{
    srand(time(NULL));
    int   i, j = 0;
    float mat[M][2];
    float c1[2], c2[2];
    float cV1[2], cV2[2];
    float sum1[2], sum2[2];
    int   len1, len2 = 0;

    #pragma omp parallel for
    for (i = 0; i < M; i++)
    {
        mat[i][0] = rand() * 1.0 / RAND_MAX;
        mat[i][1] = rand() * 1.0 / RAND_MAX;
    }

    c1[0] = rand() * 1.0 / RAND_MAX;
    c1[1] = rand() * 1.0 / RAND_MAX;
    c2[0] = rand() * 1.0 / RAND_MAX;
    c2[1] = rand() * 1.0 / RAND_MAX;


    while (sqrt(pow((c1[0] - cV1[0]), 2) + pow((c1[1] - cV1[1]), 2)) > 0.0001 ||
           sqrt(pow((c2[0] - cV2[0]), 2) + pow((c2[1] - cV2[1]), 2)) > 0.0001)
    {
        sum1[0] = 0;
        sum1[1] = 0;
        sum2[0] = 0;
        sum2[1] = 0;
        len1    = 0;
        len2    = 0;

        #pragma omp parallel for private(i,j)
        for (i = 0; i < M; i++)
        {
            int dis1 = sqrt(pow((c1[0] - mat[i][0]), 2) + pow((c1[1] - mat[i][1]), 2));
            int dis2 = sqrt(pow((c2[0] - mat[i][0]), 2) + pow((c2[1] - mat[i][1]), 2));

            #pragma omp critical
            {
                if (dis1 < dis2)
                {
                    sum1[0] += mat[i][0];
                    sum1[1] += mat[i][1];
                    len1++;
                }
                else
                {
                    sum2[0] += mat[i][0];
                    sum2[1] += mat[i][1];
                    len2++;
                }
            }
        }

        cV1[0] = c1[0];
        cV1[1] = c1[1];
        cV2[0] = c2[0];
        cV2[1] = c2[1];

        if (len1 > 0)
        {
            c1[0] = sum1[0] / len1;
            c1[1] = sum1[1] / len1;
        }
        if (len2 > 0)
        {
            c2[0] = sum2[0] / len2;
            c2[1] = sum2[1] / len2;
        }

        printf("====== \n");
        printf("C1(%f,%f) CV1(%f,%f)\n", c1[0], c1[1], cV1[0], cV1[1]);
        printf("C2(%f,%f) CV2(%f,%f)\n", c2[0], c2[1], cV2[0], cV2[1]);
    }

    printf("\n== FINAL == \n");
    printf("C1(%f,%f)\n", c1[0], c1[1]);
    printf("C2(%f,%f)\n", c2[0], c2[1]);

    return 0;
}
