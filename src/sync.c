#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

HANDLE empty_sem;
HANDLE full_sem;
CRITICAL_SECTION mutex;

DWORD WINAPI producer(LPVOID arg)
{
    for (int i = 1; i <= PRODUCE_COUNT; i++)
    {
        WaitForSingleObject(empty_sem, INFINITE);
        EnterCriticalSection(&mutex);

        buffer[in] = i;
        printf("Producer produced item %d at buffer[%d]\n", i, in);
        in = (in + 1) % BUFFER_SIZE;

        LeaveCriticalSection(&mutex);
        ReleaseSemaphore(full_sem, 1, NULL);

        Sleep(300);
    }
    return 0;
}

DWORD WINAPI consumer(LPVOID arg)
{
    for (int i = 1; i <= PRODUCE_COUNT; i++)
    {
        WaitForSingleObject(full_sem, INFINITE);
        EnterCriticalSection(&mutex);

        int item = buffer[out];
        printf("Consumer consumed item %d from buffer[%d]\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        LeaveCriticalSection(&mutex);
        ReleaseSemaphore(empty_sem, 1, NULL);

        Sleep(500);
    }
    return 0;
}

void run_sync_demo()
{
    HANDLE producer_thread;
    HANDLE consumer_thread;

    printf("\n========== Producer-Consumer Synchronization Demo ==========\n");

    empty_sem = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    full_sem = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    InitializeCriticalSection(&mutex);

    producer_thread = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    consumer_thread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    WaitForSingleObject(producer_thread, INFINITE);
    WaitForSingleObject(consumer_thread, INFINITE);

    CloseHandle(producer_thread);
    CloseHandle(consumer_thread);
    CloseHandle(empty_sem);
    CloseHandle(full_sem);
    DeleteCriticalSection(&mutex);

    printf("Synchronization demo finished.\n");
}
