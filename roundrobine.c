#include <stdio.h>

int main() {
    int n, i, tq; 
    int burst[20], arrival[20], remaining[20];
    int time = 0, completed = 0;
    float totalWT = 0, totalTAT = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter arrival time of process %d: ", i+1);
        scanf("%d", &arrival[i]);
        printf("Enter burst time of process %d: ", i+1);
        scanf("%d", &burst[i]);
        remaining[i] = burst[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &tq);

    printf("\nGantt Chart:\n");
    printf("Time\tProcess\n");

    while(completed < n) {
        int flag = 0;
        for(i = 0; i < n; i++) {
            if(remaining[i] > 0 && arrival[i] <= time) {
                flag = 1;
                int execTime = (remaining[i] > tq) ? tq : remaining[i];
                printf("%d\tP%d\n", time, i+1);

                time += execTime;
                remaining[i] -= execTime;

                if(remaining[i] == 0) {
                    completed++;
                    int tat = time - arrival[i];
                    int wt = tat - burst[i];
                    totalTAT += tat;
                    totalWT += wt;
                }
            }
        }
        if(!flag) time++;  // If no process is ready, increment time
    }

    printf("\nAverage Turnaround Time: %.2f", totalTAT/n);
    printf("\nAverage Waiting Time: %.2f\n", totalWT/n);

    return 0;
}
