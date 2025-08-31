#include <stdio.h>

int main() {
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int process[n], arrival[n], burst[n];
    int completion[n], turnaround[n], waiting[n];

    
    for (i = 0; i < n; i++) {
        process[i] = i + 1;
        printf("Enter Arrival Time of P%d: ", i + 1);
        scanf("%d", &arrival[i]);
        printf("Enter Burst Time of P%d: ", i + 1);
        scanf("%d", &burst[i]);
    }

   
    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arrival[i] > arrival[j]) {
                
                int temp = arrival[i];
                arrival[i] = arrival[j];
                arrival[j] = temp;
                
                temp = burst[i];
                burst[i] = burst[j];
                burst[j] = temp;
               
                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    
    int time = 0;
    for (i = 0; i < n; i++) {
        if (time < arrival[i])
            time = arrival[i]; 
        time += burst[i];
        completion[i] = time;

        turnaround[i] = completion[i] - arrival[i];
        waiting[i] = turnaround[i] - burst[i];
    }


    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", process[i], arrival[i], burst[i], completion[i], turnaround[i], waiting[i]);
    }

 
    float avgTAT = 0, avgWT = 0;
    for (i = 0; i < n; i++) {
        avgTAT += turnaround[i];
        avgWT += waiting[i];
    }
    avgTAT /= n;
    avgWT /= n;

    printf("\nAverage Turnaround Time: %.2f", avgTAT);
    printf("\nAverage Waiting Time: %.2f\n", avgWT);

    return 0;
}

