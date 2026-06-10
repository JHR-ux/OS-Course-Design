#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_scheduler_demo();
void run_memory_demo();
void run_sync_demo();
void run_filesystem_demo();

int main() {
    int choice;
    while (1) {
        printf("\n====================================\n");
        printf("   OS Course Design Simulator\n");
        printf("====================================\n");
        printf("1. Processor Scheduling\n");
        printf("2. Memory Management\n");
        printf("3. Process Synchronization\n");
        printf("4. Simple File System\n");
        printf("0. Exit\n");
        printf("Please choose: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                run_scheduler_demo();
                break;
            case 2:
                run_memory_demo();
                break;
            case 3:
                run_sync_demo();
                break;
            case 4:
                run_filesystem_demo();
                break;
            case 0:
                printf("Exit system.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
