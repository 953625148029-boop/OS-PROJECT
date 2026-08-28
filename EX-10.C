```c
#include <stdio.h>

#define P 5       // Number of processes
#define R 3       // Number of resource types

int allocation[P][R];
int max[P][R];
int need[P][R];
int available[R];

/* Calculate Need = Maximum - Allocation */
void calculate_need()
{
    int i, j;

    for (i = 0; i < P; i++)
    {
        for (j = 0; j < R; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

/* Check whether the system is in a safe state */
int is_safe(int safe_seq[])
{
    int work[R];
    int finish[P] = {0};
    int i, j, count = 0;

    /* Work = Available */
    for (i = 0; i < R; i++)
    {
        work[i] = available[i];
    }

    while (count < P)
    {
        int found = 0;

        for (i = 0; i < P; i++)
        {
            if (!finish[i])
            {
                int can_allocate = 1;

                /* Check Need <= Work */
                for (j = 0; j < R; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        can_allocate = 0;
                        break;
                    }
                }

                if (can_allocate)
                {
                    /* Work = Work + Allocation */
                    for (j = 0; j < R; j++)
                    {
                        work[j] += allocation[i][j];
                    }

                    safe_seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found)
        {
            return 0;   // Unsafe state
        }
    }

    return 1;   // Safe state
}

/* Request resources for a process */
int request_resources(int p_id, int request[R])
{
    int i;
    int safe_seq[P];

    /* Step 1: Check request <= Need */
    for (i = 0; i < R; i++)
    {
        if (request[i] > need[p_id][i])
        {
            printf("Error: Process P%d has exceeded its maximum claim.\n",
                   p_id);
            return 0;
        }
    }

    /* Step 2: Check request <= Available */
    for (i = 0; i < R; i++)
    {
        if (request[i] > available[i])
        {
            printf("Process P%d must wait; resources are not available.\n",
                   p_id);
            return 0;
        }
    }

    /* Step 3: Temporarily allocate resources */
    for (i = 0; i < R; i++)
    {
        available[i] -= request[i];
        allocation[p_id][i] += request[i];
        need[p_id][i] -= request[i];
    }

    /* Check whether the new state is safe */
    if (is_safe(safe_seq))
    {
        printf("Request can be granted safely.\n");

        printf("New safe sequence: ");

        for (i = 0; i < P; i++)
        {
            printf("P%d ", safe_seq[i]);
        }

        printf("\n");

        return 1;
    }
    else
    {
        /* Roll back the temporary allocation */
        for (i = 0; i < R; i++)
        {
            available[i] += request[i];
            allocation[p_id][i] -= request[i];
            need[p_id][i] += request[i];
        }

        printf("Request denied: granting it would leave the system "
               "in an unsafe state.\n");

        return 0;
    }
}

int main()
{
    int i, j;

    /* Input Allocation Matrix */
    printf("Enter Allocation matrix (%d processes x %d resources):\n",
           P, R);

    for (i = 0; i < P; i++)
    {
        printf("P%d: ", i);

        for (j = 0; j < R; j++)
        {
            scanf("%d", &allocation[i][j]);
        }
    }

    /* Input Maximum Matrix */
    printf("\nEnter Maximum matrix (%d processes x %d resources):\n",
           P, R);

    for (i = 0; i < P; i++)
    {
        printf("P%d: ", i);

        for (j = 0; j < R; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }

    /* Input Available Resources */
    printf("\nEnter Available resources vector (%d resources):\n", R);

    for (j = 0; j < R; j++)
    {
        scanf("%d", &available[j]);
    }

    /* Calculate Need Matrix */
    calculate_need();

    printf("\nNeed matrix:\n");

    for (i = 0; i < P; i++)
    {
        printf("P%d: ", i);

        for (j = 0; j < R; j++)
        {
            printf("%d ", need[i][j]);
        }

        printf("\n");
    }

    /* Check initial safe state */
    int safe_seq[P];

    if (is_safe(safe_seq))
    {
        printf("\nSystem is in a SAFE state.\n");

        printf("Safe sequence: ");

        for (i = 0; i < P; i++)
        {
            printf("P%d ", safe_seq[i]);
        }

        printf("\n");
    }
    else
    {
        printf("\nSystem is in an UNSAFE state "
               "(deadlock may occur).\n");

        return 0;
    }

    /* Resource Request */
    int p_id;
    int request[R];

    printf("\nEnter process number requesting resources: ");
    scanf("%d", &p_id);

    if (p_id < 0 || p_id >= P)
    {
        printf("Invalid process number.\n");
        return 0;
    }

    printf("Enter request vector (%d resources): ", R);

    for (j = 0; j < R; j++)
    {
        scanf("%d", &request[j]);
    }

    request_resources(p_id, request);

    return 0;
}
```
