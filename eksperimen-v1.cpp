#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ALLOCATIONS 10000
#define MAX_SIZE 1024
#define NUM_ITERATIONS 10
#define LONG_LIVED_FREQUENCY 100  // Setiap 100 alokasi, jadikan objek sebagai long-lived
#define LONG_LIVED_LIFETIME 5     // Long-lived objek akan hidup selama 5 iterasi

// Struktur Linked List untuk memantau blok memori
typedef struct MemoryBlock {
    void *ptr;
    size_t size;
    int lifetime_remaining;
    struct MemoryBlock *next;
} MemoryBlock;

MemoryBlock *allocated_blocks = NULL;
MemoryBlock *long_lived_blocks = NULL;

size_t total_memory_allocated = 0;
size_t total_memory_freed = 0;
size_t peak_memory_used = 0;
int total_fragments = 0;

void add_block(MemoryBlock **list, void *ptr, size_t size, int lifetime_remaining) {
    MemoryBlock *new_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    if (!new_block) {
        printf("Gagal mengalokasikan memori untuk blok\n");
        exit(1);
    }
    new_block->ptr = ptr;
    new_block->size = size;
    new_block->lifetime_remaining = lifetime_remaining;
    new_block->next = *list;
    *list = new_block;
}

void remove_block(MemoryBlock **list, MemoryBlock *block_to_remove) {
    MemoryBlock *prev = NULL, *curr = *list;
    while (curr != NULL) {
        if (curr == block_to_remove) {
            if (prev) {
                prev->next = curr->next;
            } else {
                *list = curr->next;
            }
            free(curr->ptr);
            total_memory_freed += curr->size;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void allocate_memory(size_t *current_memory_usage, int iteration) {
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        size_t size = rand() % MAX_SIZE + 1;
        void *ptr = malloc(size);

        if (ptr == NULL) {
            printf("Gagal mengalokasikan memori pada alokasi %d\n", i);
            exit(1);
        }

        total_memory_allocated += size;
        *current_memory_usage += size;

        if (*current_memory_usage > peak_memory_used) {
            peak_memory_used = *current_memory_usage;
        }

        // Tentukan apakah ini menjadi long-lived block
        if (i % LONG_LIVED_FREQUENCY == 0) {
            add_block(&long_lived_blocks, ptr, size, LONG_LIVED_LIFETIME);
        } else {
            add_block(&allocated_blocks, ptr, size, 0);
        }
    }
}

void deallocate_memory(size_t *current_memory_usage) {
    MemoryBlock *curr = allocated_blocks;
    while (curr != NULL) {
        *current_memory_usage -= curr->size;
        MemoryBlock *next = curr->next;
        remove_block(&allocated_blocks, curr);
        curr = next;
    }
}

void deallocate_long_lived_blocks(size_t *current_memory_usage) {
    MemoryBlock *curr = long_lived_blocks;
    while (curr != NULL) {
        if (curr->lifetime_remaining <= 0) {
            *current_memory_usage -= curr->size;
            MemoryBlock *next = curr->next;
            remove_block(&long_lived_blocks, curr);
            curr = next;
        } else {
            curr->lifetime_remaining--;
            curr = curr->next;
        }
    }
}

int count_memory_fragments() {
    int fragments = 0;
    MemoryBlock *curr = allocated_blocks;
    while (curr != NULL) {
        fragments++;
        curr = curr->next;
    }
    return fragments;
}

void free_all_blocks() {
    MemoryBlock *curr = allocated_blocks;
    while (curr != NULL) {
        MemoryBlock *next = curr->next;
        free(curr->ptr);
        free(curr);
        curr = next;
    }
    allocated_blocks = NULL;

    curr = long_lived_blocks;
    while (curr != NULL) {
        MemoryBlock *next = curr->next;
        free(curr->ptr);
        free(curr);
        curr = next;
    }
    long_lived_blocks = NULL;
}

int main() {
    srand(time(NULL));

    clock_t start_time = clock();

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
        printf("=== Iteration %d ===\n", iteration + 1);

        size_t current_memory_usage = 0;

        allocate_memory(&current_memory_usage, iteration);

        int fragments = count_memory_fragments();
        total_fragments += fragments;

        printf("Iteration %d: Current memory usage: %zu bytes\n", iteration + 1, current_memory_usage);
        printf("Iteration %d: Peak memory used so far: %zu bytes\n", iteration + 1, peak_memory_used);
        printf("Iteration %d: Fragments: %d\n", iteration + 1, fragments);

        deallocate_memory(&current_memory_usage);

        deallocate_long_lived_blocks(&current_memory_usage);

        printf("Iteration %d: Memory after deallocation: %zu bytes\n", iteration + 1, current_memory_usage);
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("=== Statistik Penggunaan Memori (Optimasi yang Lebih Baik) ===\n");
    printf("Total memory allocated: %zu bytes\n", total_memory_allocated);
    printf("Total memory freed: %zu bytes\n", total_memory_freed);
    printf("Peak memory used: %zu bytes\n", peak_memory_used);
    printf("Memory leak: %zu bytes\n", total_memory_allocated - total_memory_freed);
    printf("Total fragments: %d\n", total_fragments);
    printf("Average free block size: %.2f bytes\n", total_fragments > 0 ? (double)total_memory_freed / total_fragments : 0.0);
    printf("Time elapsed: %.5f seconds\n", execution_time);

    // Bersihkan semua blok memori yang tersisa untuk mencegah memory leak
    free_all_blocks();

    return 0;
}

