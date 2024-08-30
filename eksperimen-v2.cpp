#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ALLOCATIONS 10000
#define MAX_SIZE 1024
#define NUM_ITERATIONS 10
#define LONG_LIVED_FREQUENCY 100  // Setiap 100 alokasi, jadikan objek sebagai long-lived
#define LONG_LIVED_LIFETIME 5     // Long-lived objek akan hidup selama 5 iterasi

typedef struct {
    void *ptr;
    size_t size;
    int lifetime_remaining;
} MemoryBlock;

MemoryBlock long_lived_blocks[MAX_ALLOCATIONS];

size_t total_memory_allocated = 0;
size_t total_memory_freed = 0;
size_t peak_memory_used = 0;

void allocate_and_deallocate_memory(int num_operations, size_t *current_memory_usage, int iteration) {
    for (int i = 0; i < num_operations; i++) {
        size_t size = rand() % MAX_SIZE + 1;
        void *ptr = malloc(size);

        if (ptr == NULL) {
            printf("Gagal mengalokasikan memori pada operasi %d\n", i);
            exit(1);
        }

        total_memory_allocated += size;
        *current_memory_usage += size;

        if (*current_memory_usage > peak_memory_used) {
            peak_memory_used = *current_memory_usage;
        }

        // Tentukan apakah ini menjadi long-lived block
        if (i % LONG_LIVED_FREQUENCY == 0) {
            int idx = i / LONG_LIVED_FREQUENCY;
            long_lived_blocks[idx].ptr = ptr;
            long_lived_blocks[idx].size = size;
            long_lived_blocks[idx].lifetime_remaining = LONG_LIVED_LIFETIME;
        } else {
            total_memory_freed += size;
            *current_memory_usage -= size;
            free(ptr);
        }
    }
}

void deallocate_long_lived_blocks(size_t *current_memory_usage) {
    for (int i = 0; i < MAX_ALLOCATIONS / LONG_LIVED_FREQUENCY; i++) {
        if (long_lived_blocks[i].ptr != NULL && long_lived_blocks[i].lifetime_remaining <= 0) {
            total_memory_freed += long_lived_blocks[i].size;
            *current_memory_usage -= long_lived_blocks[i].size;
            free(long_lived_blocks[i].ptr);
            long_lived_blocks[i].ptr = NULL;
        }
    }
}

void update_long_lived_blocks() {
    for (int i = 0; i < MAX_ALLOCATIONS / LONG_LIVED_FREQUENCY; i++) {
        if (long_lived_blocks[i].ptr != NULL) {
            long_lived_blocks[i].lifetime_remaining--;
        }
    }
}

int main() {
    srand(time(NULL));

    clock_t start_time = clock();

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
        printf("=== Iteration %d ===\n", iteration + 1);

        size_t current_memory_usage = 0;

        allocate_and_deallocate_memory(MAX_ALLOCATIONS, &current_memory_usage, iteration);

        printf("Iteration %d: Current memory usage: %zu bytes\n", iteration + 1, current_memory_usage);
        printf("Iteration %d: Peak memory used so far: %zu bytes\n", iteration + 1, peak_memory_used);

        deallocate_long_lived_blocks(&current_memory_usage);

        printf("Iteration %d: Memory after deallocation: %zu bytes\n", iteration + 1, current_memory_usage);

        update_long_lived_blocks();
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("=== Statistik Penggunaan Memori (Optimasi yang Buruk) ===\n");
    printf("Total memory allocated: %zu bytes\n", total_memory_allocated);
    printf("Total memory freed: %zu bytes\n", total_memory_freed);
    printf("Peak memory used: %zu bytes\n", peak_memory_used);
    printf("Memory leak: %zu bytes\n", total_memory_allocated - total_memory_freed);
    printf("Time elapsed: %.5f seconds\n", execution_time);

    return 0;
}

