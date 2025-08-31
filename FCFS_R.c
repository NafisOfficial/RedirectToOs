#include <stdio.h>


void calculateWaitingTime(int processes[], int n, int bt[], int wt[]) {
    wt[0] = 0; 


    for (int i = 1; i < n; i++) {
        wt[i] = bt[i - 1] + wt[i - 1];
    }
}


void calculateTurnaroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
    }
}


void calculateAverageTime(int processes[], int n, int bt[]) {
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;

    
    calculateWaitingTime(processes, n, bt, wt);
    calculateTurnaroundTime(processes, n, bt, wt, tat);

    
    printf("\nProcess  Burst Time  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("  %d\t    %d\t\t  %d\t\t  %d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    
    printf("\nAverage Waiting Time = %.2f", (float)total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", (float)total_tat / n);
}


int main() {
    int n;

    
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int processes[n], burst_time[n];

   
    for (int i = 0; i < n; i++) {
        processes[i] = i + 1;
        printf("Enter burst time for Process %d: ", i + 1);
        scanf("%d", &burst_time[i]);
    }

    
    calculateAverageTime(processes, n, burst_time);

    return 0;
}