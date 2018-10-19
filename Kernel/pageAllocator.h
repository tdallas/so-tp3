#ifndef PAGEALLOCATOR_H_
#define PAGEALLOCATOR_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
 * Every allocation needs an 8-byte header to store the allocation size while
 * staying 8-byte aligned. The address returned by "myMalloc" is the address
 * right after this header (i.e. the size occupies the 8 bytes before the
 * returned address).
 */
#define HEADER_SIZE 8

/*
 * The minimum allocation size is 16 bytes because we have an 8-byte header and
 * we need to stay 8-byte aligned.
 */
#define MIN_ALLOC_LOG2 4
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

/*
 * The maximum allocation size is currently set to 2gb. This is the total size
 * of the heap. It's technically also the maximum allocation size because the
 * heap could consist of a single allocation of this size. But of course real
 * heaps will have multiple allocations, so the real maximum allocation limit
 * is at most 1gb.
 */
#define MAX_ALLOC_LOG2 31
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)

/*
 * Allocations are done in powers of two starting from MIN_ALLOC and ending at
 * MAX_ALLOC inclusive. Each allocation size has a bucket that stores the myFree
 * list for that allocation size.
 *
 * Given a bucket index, the size of the allocations in that bucket can be
 * found with "(size_t)1 << (MAX_ALLOC_LOG2 - bucket)".
 */
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

/*Address for size of ram*/
#define SYSTEM_RAM_ADDRESS 0x1000000
/*
 * myFree lists are stored as circular doubly-linked lists. Every possible
 * allocation size has an associated myFree list that is threaded through all
 * currently myFree blocks of that size. That means MIN_ALLOC must be at least
 * "sizeof(list_t)". MIN_ALLOC is currently 16 bytes, so this will be true for
 * both 32-bit and 64-bit.
 */
typedef struct list_t {
  struct list_t *prev, *next;
} list_t;

static int update_max_ptr(uint8_t *new_value);
static void list_init(list_t *list);
static void list_push(list_t *list, list_t *entry);
static void list_remove(list_t *entry);
static list_t *list_pop(list_t *list);
static uint8_t *ptr_for_node(size_t index, size_t bucket);
static size_t node_for_ptr(uint8_t *ptr, size_t bucket);
static int parent_is_split(size_t index);
static void flip_parent_is_split(size_t index);
static size_t bucket_for_request(size_t request);
static int lower_bucket_limit(size_t bucket);
void *myMalloc(size_t request);
void myFree(void *ptr);

#endif