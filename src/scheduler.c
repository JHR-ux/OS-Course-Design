#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 20

typedef struct {
    char name[20];
    int arrival;
    int burst;
    int priority;
    int remaining;
    int start;
    int finish;
    int turnaround;
    double weighted_turnaround;
    int completed;
} Process;

static void copy_processes(Process dest[], Process src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

static void print_result(Process p[], int n, const char *algo) {
    double avg_t = 0, avg_w = 0;
    printf("\n[%s Result]\n", algo);
    printf("%-8s%-10s%-8s%-10s%-12s%-12s\n", "Name", "Arrival", "Burst", "Finish", "Turnaround", "Weighted");
    for (int i = 0; i < n; i++) {
        avg_t += p[i].turnaround;
        avg_w += p[i].weighted_turnaround;
        printf("%-8s%-10d%-8d%-10d%-12d%-12.2f\n", p[i].name, p[i].arrival, p[i].burst,
               p[i].finish, p[i].turnaround, p[i].weighted_turnaround);
    }
    printf("Average turnaround time: %.2f\n", avg_t / n);
    printf("Average weighted turnaround time: %.2f\n", avg_w / n);
}

static void fcfs(Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j+1].arrival) {
                Process tmp = p[j]; p[j] = p[j+1]; p[j+1] = tmp;
            }
        }
    }

    int time = 0;
    printf("\nFCFS running order: ");
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival) time = p[i].arrival;
        printf("%s ", p[i].name);
        time += p[i].burst;
        p[i].finish = time;
        p[i].turnaround = p[i].finish - p[i].arrival;
        p[i].weighted_turnaround = (double)p[i].turnaround / p[i].burst;
    }
    printf("\n");
    print_result(p, n, "FCFS");
}

static void sjf(Process p[], int n) {
    int time = 0, completed = 0;
    printf("\nSJF running order: ");
    while (completed < n) {
        int idx = -1;
        int min_burst = 999999;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time && p[i].burst < min_burst) {
                min_burst = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        printf("%s ", p[idx].name);
        time += p[idx].burst;
        p[idx].finish = time;
        p[idx].turnaround = p[idx].finish - p[idx].arrival;
        p[idx].weighted_turnaround = (double)p[idx].turnaround / p[idx].burst;
        p[idx].completed = 1;
        completed++;
    }
    printf("\n");
    print_result(p, n, "SJF");
}

static void priority_schedule(Process p[], int n) {
    int time = 0, completed = 0;
    printf("\nPriority running order: ");
    while (completed < n) {
        int idx = -1;
        int best_priority = 999999;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time && p[i].priority < best_priority) {
                best_priority = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        printf("%s ", p[idx].name);
        time += p[idx].burst;
        p[idx].finish = time;
        p[idx].turnaround = p[idx].finish - p[idx].arrival;
        p[idx].weighted_turnaround = (double)p[idx].turnaround / p[idx].burst;
        p[idx].completed = 1;
        completed++;
    }
    printf("\n");
    print_result(p, n, "Priority");
}

static void rr(Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    printf("\nRR running order: ");
    while (completed < n) {
        int progressed = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time) {
                progressed = 1;
                printf("%s ", p[i].name);
                int run_time = p[i].remaining > quantum ? quantum : p[i].remaining;
                time += run_time;
                p[i].remaining -= run_time;
                if (p[i].remaining == 0) {
                    p[i].finish = time;
                    p[i].turnaround = p[i].finish - p[i].arrival;
                    p[i].weighted_turnaround = (double)p[i].turnaround / p[i].burst;
                    completed++;
                }
            }
        }
        if (!progressed) time++;
    }
    printf("\n");
    print_result(p, n, "Round Robin");
}

static void hrrn(Process p[], int n) {
    int time = 0, completed = 0;
    printf("\nHRRN running order: ");
    while (completed < n) {
        int idx = -1;
        double best_ratio = -1.0;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time) {
                int wait = time - p[i].arrival;
                double ratio = (double)(wait + p[i].burst) / p[i].burst;
                if (ratio > best_ratio) {
                    best_ratio = ratio;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        printf("%s ", p[idx].name);
        time += p[idx].burst;
        p[idx].finish = time;
        p[idx].turnaround = p[idx].finish - p[idx].arrival;
        p[idx].weighted_turnaround = (double)p[idx].turnaround / p[idx].burst;
        p[idx].completed = 1;
        completed++;
    }
    printf("\n");
    print_result(p, n, "HRRN Extension");
}

void run_scheduler_demo() {
    int n, quantum;
    Process original[MAX_PROCESS], temp[MAX_PROCESS];

    printf("\nInput process number: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX_PROCESS) {
        printf("Invalid process number.\n");
        return;
    }

    printf("Input process info: name arrival_time burst_time priority\n");
    printf("Example: P1 0 5 2\n");
    for (int i = 0; i < n; i++) {
        scanf("%s %d %d %d", original[i].name, &original[i].arrival, &original[i].burst, &original[i].priority);
        original[i].remaining = original[i].burst;
        original[i].completed = 0;
    }
    printf("Input time quantum for RR: ");
    scanf("%d", &quantum);

    copy_processes(temp, original, n);
    fcfs(temp, n);
    copy_processes(temp, original, n);
    sjf(temp, n);
    copy_processes(temp, original, n);
    rr(temp, n, quantum);
    copy_processes(temp, original, n);
    priority_schedule(temp, n);
    copy_processes(temp, original, n);
    hrrn(temp, n);
}
