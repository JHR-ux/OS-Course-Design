#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10



int buffer[BUFFER_SIZE];
int in = 0, out = 0;

HANDLE empty_sem;
HANDLE full_sem;
CRITICAL_SECTION pc_mutex;

DWORD WINAPI producer(LPVOID arg)
{
    for (int i = 1; i <= PRODUCE_COUNT; i++)
    {
        WaitForSingleObject(empty_sem, INFINITE);

        EnterCriticalSection(&pc_mutex);

        buffer[in] = i;
        printf("Producer produced item %d at buffer[%d]\n", i, in);

        in = (in + 1) % BUFFER_SIZE;

        LeaveCriticalSection(&pc_mutex);

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

        EnterCriticalSection(&pc_mutex);

        int item = buffer[out];

        printf("Consumer consumed item %d from buffer[%d]\n",
            item, out);

        out = (out + 1) % BUFFER_SIZE;

        LeaveCriticalSection(&pc_mutex);

        ReleaseSemaphore(empty_sem, 1, NULL);

        Sleep(500);
    }

    return 0;
}

void producer_consumer_demo()
{
    HANDLE pThread;
    HANDLE cThread;

    printf("\n========== Producer Consumer ==========\n");

    empty_sem = CreateSemaphore(NULL,
        BUFFER_SIZE,
        BUFFER_SIZE,
        NULL);

    full_sem = CreateSemaphore(NULL,
        0,
        BUFFER_SIZE,
        NULL);

    InitializeCriticalSection(&pc_mutex);

    pThread = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    cThread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    WaitForSingleObject(pThread, INFINITE);
    WaitForSingleObject(cThread, INFINITE);

    CloseHandle(pThread);
    CloseHandle(cThread);

    CloseHandle(empty_sem);
    CloseHandle(full_sem);

    DeleteCriticalSection(&pc_mutex);

    printf("Producer Consumer Finished.\n");
}


int readCount = 0;

HANDLE rw_mutex;
HANDLE write_sem;

DWORD WINAPI reader(LPVOID arg)
{
    int id = *(int*)arg;

    WaitForSingleObject(rw_mutex, INFINITE);

    readCount++;

    if (readCount == 1)
        WaitForSingleObject(write_sem, INFINITE);

    ReleaseMutex(rw_mutex);

    printf("Reader %d is reading...\n", id);

    Sleep(1000);

    WaitForSingleObject(rw_mutex, INFINITE);

    readCount--;

    if (readCount == 0)
        ReleaseSemaphore(write_sem, 1, NULL);

    ReleaseMutex(rw_mutex);

    return 0;
}

DWORD WINAPI writer(LPVOID arg)
{
    int id = *(int*)arg;

    WaitForSingleObject(write_sem, INFINITE);

    printf("Writer %d is writing...\n", id);

    Sleep(1500);

    ReleaseSemaphore(write_sem, 1, NULL);

    return 0;
}

void reader_writer_demo()
{
    printf("\n========== Reader Writer ==========\n");

    rw_mutex = CreateMutex(NULL, FALSE, NULL);
    write_sem = CreateSemaphore(NULL, 1, 1, NULL);

    HANDLE threads[5];

    int r1 = 1;
    int r2 = 2;
    int r3 = 3;

    int w1 = 1;
    int w2 = 2;

    threads[0] = CreateThread(NULL, 0, reader, &r1, 0, NULL);
    threads[1] = CreateThread(NULL, 0, reader, &r2, 0, NULL);
    threads[2] = CreateThread(NULL, 0, writer, &w1, 0, NULL);
    threads[3] = CreateThread(NULL, 0, reader, &r3, 0, NULL);
    threads[4] = CreateThread(NULL, 0, writer, &w2, 0, NULL);

    WaitForMultipleObjects(5,
        threads,
        TRUE,
        INFINITE);

    for (int i = 0; i < 5; i++)
        CloseHandle(threads[i]);

    CloseHandle(rw_mutex);
    CloseHandle(write_sem);

    printf("Reader Writer Finished.\n");
}


#define N 5

HANDLE chopstick[N];

DWORD WINAPI philosopher(LPVOID arg)
{
    int id = *(int*)arg;

    printf("Philosopher %d is thinking.\n", id);

    Sleep(1000);

    if (id == N - 1)
    {
        WaitForSingleObject(chopstick[(id + 1) % N],
            INFINITE);

        WaitForSingleObject(chopstick[id],
            INFINITE);
    }
    else
    {
        WaitForSingleObject(chopstick[id],
            INFINITE);

        WaitForSingleObject(chopstick[(id + 1) % N],
            INFINITE);
    }

    printf("Philosopher %d is eating.\n", id);

    Sleep(1000);

    ReleaseSemaphore(chopstick[id],
        1,
        NULL);

    ReleaseSemaphore(chopstick[(id + 1) % N],
        1,
        NULL);

    return 0;
}

void dining_philosophers_demo()
{
    printf("\n========== Dining Philosophers ==========\n");

    HANDLE threads[N];

    int ids[N];

    for (int i = 0; i < N; i++)
    {
        chopstick[i] =
            CreateSemaphore(NULL, 1, 1, NULL);

        ids[i] = i;
    }

    for (int i = 0; i < N; i++)
    {
        threads[i] =
            CreateThread(NULL,
                0,
                philosopher,
                &ids[i],
                0,
                NULL);
    }

    WaitForMultipleObjects(N,
        threads,
        TRUE,
        INFINITE);

    for (int i = 0; i < N; i++)
    {
        CloseHandle(threads[i]);
        CloseHandle(chopstick[i]);
    }

    printf("Dining Philosophers Finished.\n");
}



void run_sync_demo()
{
    int choice;

    while (1)
    {
        printf("\n========== Synchronization Demo ==========\n");
        printf("1. Producer Consumer\n");
        printf("2. Reader Writer\n");
        printf("3. Dining Philosophers\n");
        printf("0. Back\n");

        printf("Choose: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            producer_consumer_demo();
            break;

        case 2:
            reader_writer_demo();
            break;

        case 3:
            dining_philosophers_demo();
            break;

        case 0:
            return;

        default:e
            printf("Invalid choice.\n");
        }
    }
}
