#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_FILES 20
#define MAX_CONTENT 256

typedef struct {
    char name[50];
    char content[MAX_CONTENT];
    int used;
} SimFile;

static SimFile files[MAX_FILES];

static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

static int find_file(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static void create_file(const char* name) {
    if (find_file(name) != -1) {
        printf("File already exists.\n");
        return;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            files[i].used = 1;
            strcpy(files[i].name, name);
            files[i].content[0] = '\0';
            printf("Create file %s successfully.\n", name);
            return;
        }
    }

    printf("No free file control block.\n");
}

static void write_file(const char* name, const char* content) {
    int idx = find_file(name);

    if (idx == -1) {
        printf("File not found.\n");
        return;
    }

    strncpy(files[idx].content, content, MAX_CONTENT - 1);
    files[idx].content[MAX_CONTENT - 1] = '\0';

    printf("Write file %s successfully.\n", name);
}

static void read_file(const char* name) {
    int idx = find_file(name);

    if (idx == -1) {
        printf("File not found.\n");
        return;
    }

    printf("Read file %s: %s\n", name, files[idx].content);
}

static void delete_file(const char* name) {
    int idx = find_file(name);

    if (idx == -1) {
        printf("File not found.\n");
        return;
    }

    files[idx].used = 0;
    files[idx].name[0] = '\0';
    files[idx].content[0] = '\0';

    printf("Delete file %s successfully.\n", name);
}

static void list_files() {
    printf("\nFile directory:\n");

    int count = 0;

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            printf("- %s\n", files[i].name);
            count++;
        }
    }

    if (count == 0) {
        printf("No files.\n");
    }

    printf("Used file control blocks: %d\n", count);
    printf("Free file control blocks: %d\n", MAX_FILES - count);
}

void run_filesystem_demo() {
    int choice;
    char name[50];
    char content[MAX_CONTENT];

    while (1) {
        printf("\n[Simple File System]\n");
        printf("1. Create file\n");
        printf("2. Write file\n");
        printf("3. Read file\n");
        printf("4. Delete file\n");
        printf("5. List files\n");
        printf("0. Back\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1:
            printf("File name: ");
            if (scanf("%49s", name) != 1) {
                printf("Invalid file name.\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();
            create_file(name);
            break;

        case 2:
            printf("File name: ");
            if (scanf("%49s", name) != 1) {
                printf("Invalid file name.\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();

            printf("Content: ");
            if (fgets(content, MAX_CONTENT, stdin) == NULL) {
                printf("Invalid content.\n");
                break;
            }
            content[strcspn(content, "\n")] = '\0';

            write_file(name, content);
            break;

        case 3:
            printf("File name: ");
            if (scanf("%49s", name) != 1) {
                printf("Invalid file name.\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();
            read_file(name);
            break;

        case 4:
            printf("File name: ");
            if (scanf("%49s", name) != 1) {
                printf("Invalid file name.\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();
            delete_file(name);
            break;

        case 5:
            list_files();
            break;

        case 0:
            return;

        default:
            printf("Invalid choice.\n");
        }
    }
}