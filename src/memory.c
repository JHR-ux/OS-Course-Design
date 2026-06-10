#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCK 20
#define MAX_REF 50

typedef struct {
    int id;
    int start;
    int size;
    int free;
} Block;

static void print_blocks(Block blocks[], int count) {
    printf("\nMemory partition table:\n");
    printf("%-8s%-10s%-10s%-10s\n", "ID", "Start", "Size", "Status");
    for (int i = 0; i < count; i++) {
        printf("%-8d%-10d%-10d%-10s\n", blocks[i].id, blocks[i].start, blocks[i].size,
               blocks[i].free ? "Free" : "Used");
    }
}

static void first_fit_demo() {
    Block blocks[MAX_BLOCK] = {{1, 0, 100, 1}, {2, 100, 200, 1}, {3, 300, 150, 1}, {4, 450, 300, 1}};
    int count = 4;
    int request[] = {80, 120, 60, 200};
    int req_count = 4;

    printf("\n[First Fit Dynamic Partition Demo]\n");
    print_blocks(blocks, count);
    for (int r = 0; r < req_count; r++) {
        int allocated = 0;
        printf("\nRequest %d KB\n", request[r]);
        for (int i = 0; i < count; i++) {
            if (blocks[i].free && blocks[i].size >= request[r]) {
                printf("Allocated in block %d\n", blocks[i].id);
                blocks[i].free = 0;
                allocated = 1;
                break;
            }
        }
        if (!allocated) printf("Allocation failed.\n");
        print_blocks(blocks, count);
    }
}

static void best_fit_demo() {
    Block blocks[MAX_BLOCK] = {{1, 0, 100, 1}, {2, 100, 200, 1}, {3, 300, 150, 1}, {4, 450, 300, 1}};
    int count = 4;
    int request[] = {80, 120, 60, 200};
    int req_count = 4;

    printf("\n[Best Fit Dynamic Partition Demo]\n");
    print_blocks(blocks, count);
    for (int r = 0; r < req_count; r++) {
        int idx = -1;
        int min_remain = 999999;
        printf("\nRequest %d KB\n", request[r]);
        for (int i = 0; i < count; i++) {
            if (blocks[i].free && blocks[i].size >= request[r]) {
                int remain = blocks[i].size - request[r];
                if (remain < min_remain) {
                    min_remain = remain;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            printf("Allocated in block %d\n", blocks[idx].id);
            blocks[idx].free = 0;
        } else {
            printf("Allocation failed.\n");
        }
        print_blocks(blocks, count);
    }
}

static int find_page(int frames[], int frame_count, int page) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

static void fifo_page() {
    int refs[] = {7,0,1,2,0,3,0,4,2,3,0,3,2};
    int n = 13, frame_count = 3;
    int frames[10];
    int next = 0, faults = 0;
    for (int i = 0; i < frame_count; i++) frames[i] = -1;

    printf("\n[FIFO Page Replacement]\n");
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        if (find_page(frames, frame_count, page) == -1) {
            frames[next] = page;
            next = (next + 1) % frame_count;
            faults++;
            printf("Page %d: fault, frames = ", page);
        } else {
            printf("Page %d: hit,   frames = ", page);
        }
        for (int j = 0; j < frame_count; j++) printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page faults: %d, page fault rate: %.2f%%\n", faults, (double)faults / n * 100);
}

static void lru_page() {
    int refs[] = {7,0,1,2,0,3,0,4,2,3,0,3,2};
    int n = 13, frame_count = 3;
    int frames[10], last_used[10];
    int faults = 0;
    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("\n[LRU Page Replacement]\n");
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int pos = find_page(frames, frame_count, page);
        if (pos == -1) {
            int replace = 0;
            for (int j = 1; j < frame_count; j++) {
                if (last_used[j] < last_used[replace]) replace = j;
            }
            frames[replace] = page;
            last_used[replace] = i;
            faults++;
            printf("Page %d: fault, frames = ", page);
        } else {
            last_used[pos] = i;
            printf("Page %d: hit,   frames = ", page);
        }
        for (int j = 0; j < frame_count; j++) printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page faults: %d, page fault rate: %.2f%%\n", faults, (double)faults / n * 100);
}

void run_memory_demo() {
    first_fit_demo();
    best_fit_demo();
    fifo_page();
    lru_page();
}
