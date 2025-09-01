#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m; // n = number of processes, m = number of resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], need[n][m], avail[m];
    printf("Enter Allocation Matrix:\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Max Matrix:\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available Resources:\n");
    for(int i=0;i<m;i++)
        scanf("%d", &avail[i]);

    // Calculate Need matrix
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            need[i][j] = max[i][j] - alloc[i][j];

    int finish[n];
    for(int i=0;i<n;i++) finish[i] = 0;

    int safeSeq[n], count = 0;

    while(count < n) {
        bool found = false;
        for(int p=0;p<n;p++) {
            if(!finish[p]) {
                int j;
                for(j=0;j<m;j++)
                    if(need[p][j] > avail[j]) break;

                if(j == m) { // All needs can be satisfied
                    for(int k=0;k<m;k++)
                        avail[k] += alloc[p][k];
                    safeSeq[count++] = p;
                    finish[p] = 1;
                    found = true;
                }
            }
        }
        if(!found) {
            printf("System is in an UNSAFE state!\n");
            return 0;
        }
    }

    printf("System is in a SAFE state.\nSafe Sequence: ");
    for(int i=0;i<n;i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
