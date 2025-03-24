#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMS 500  // Max numbers we can handle

// Global arrays shared between threads
int unsortedNums[MAX_NUMS];   // The original unsorted numbers
int sortedNums[MAX_NUMS];     // The sorted result after merging
int numCount = 0;             // How many numbers we actually read

// Struct to pass start and end indexes to threads
typedef struct {
    int startIndex;
    int endIndex;
} MyParams;

// Helper function for qsort to sort in ascending order
int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

// Thread function: sorts a part of unsortedNums
void *sorterThread(void *params) {
    MyParams *p = (MyParams *) params;
    // Sort our part using qsort
    qsort(&unsortedNums[p->startIndex], p->endIndex - p->startIndex + 1, sizeof(int), compare);
    pthread_exit(0);
}

// Thread function: merges two sorted halves
void *mergeThread(void *params) {
    MyParams *p = (MyParams *) params;
    int i = p->startIndex;  // Start of the first sorted half
    int j = p->endIndex + 1; // Start of the second sorted half
    int k = 0; // Index for sortedNums

    // Merge while both halves have elements
    while (i <= p->endIndex && j < numCount) {
        if (unsortedNums[i] < unsortedNums[j]) {
            sortedNums[k++] = unsortedNums[i++];
        } else {
            sortedNums[k++] = unsortedNums[j++];
        }
    }

    // Add remaining elements from the first half
    while (i <= p->endIndex) {
        sortedNums[k++] = unsortedNums[i++];
    }

    // Add remaining elements from the second half
    while (j < numCount) {
        sortedNums[k++] = unsortedNums[j++];
    }

    pthread_exit(0);
}

int main(int argc, const char *argv[]) {
    // Read numbers from file
    FILE *file = fopen("IntegerList.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char line[2048];
    if (fgets(line, sizeof(line), file)) {
        // Split the line by commas into numbers
        char *token = strtok(line, ",");
        while (token) {
            unsortedNums[numCount++] = atoi(token);
            token = strtok(NULL, ",");
        }
    }
    fclose(file);

    // Set up threads
    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create parameter structs for each thread
    MyParams *param1 = (MyParams *) malloc(sizeof(MyParams));
    MyParams *param2 = (MyParams *) malloc(sizeof(MyParams));
    MyParams *param3 = (MyParams *) malloc(sizeof(MyParams));

    // Split the array into two parts
    int mid = numCount / 2;
    param1->startIndex = 0;
    param1->endIndex = mid - 1; // First half
    param2->startIndex = mid;
    param2->endIndex = numCount - 1; // Second half

    // Create two sorting threads
    pthread_create(&tid1, &attr, sorterThread, param1);
    pthread_create(&tid2, &attr, sorterThread, param2);

    // Wait for the sorter threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Set merge parameters (first half boundaries)
    param3->startIndex = 0;
    param3->endIndex = mid - 1;

    // Create and wait for the merge thread
    pthread_create(&tid3, &attr, mergeThread, param3);
    pthread_join(tid3, NULL);

    // Write sorted result to file
    file = fopen("SortedIntegerList.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    for (int i = 0; i < numCount; i++) {
        fprintf(file, "%d", sortedNums[i]);
        if (i < numCount - 1) {
            fprintf(file, ","); // No comma after the last number
        }
    }
    fclose(file);

    free(param1);
    free(param2);
    free(param3);

    return 0;
}