#include <stdio.h>


void sortProcesses(int processes[], int n, int bt[], int at[]) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (bt[i] > bt[j]) { 
                
                int temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                
                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}


void calculateWaitingTime(int processes[], int n, int bt[], int at[], int wt[]) {
    wt[0] = 0; 

    for (int i = 1; i < n; i++) {
        wt[i] = (bt[i - 1] + wt[i - 1]); 
    }
}


void calculateTurnaroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i]; 
    }
}


void calculateAverageTime(int processes[], int n, int bt[], int at[]) {
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    
    sortProcesses(processes, n, bt, at);

    
    calculateWaitingTime(processes, n, bt, at, wt);
    calculateTurnaroundTime(processes, n, bt, wt, tat);

    
    printf("\nProcess  Arrival Time  Burst Time  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("  %d\t    %d\t\t  %d\t\t  %d\t\t  %d\n", processes[i], at[i], bt[i], wt[i], tat[i]);
    }

    
    printf("\nAverage Waiting Time = %.2f", (float)total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", (float)total_tat / n);
}


int main() {
    int n;

    
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int processes[n], burst_time[n], arrival_time[n];

    
    for (int i = 0; i < n; i++) {
        processes[i] = i + 1;
        printf("Enter arrival time for Process %d: ", i + 1);
        scanf("%d", &arrival_time[i]);
        printf("Enter burst time for Process %d: ", i + 1);
        scanf("%d", &burst_time[i]);
    }

   
    calculateAverageTime(processes, n, burst_time, arrival_time);

    return 0;
}