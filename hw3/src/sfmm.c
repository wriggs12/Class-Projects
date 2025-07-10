/**
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"

int init_heap();
int is_valid(void *pp);
int is_alloced(size_t header);
int calc_fib_index(size_t size);
int extend_wilderness(struct sf_block *wilderness);
size_t calc_required_block_size(size_t size);
size_t get_block_size(struct sf_block *block);
size_t get_payload_size(struct sf_block *block);
void init_wilderness(struct sf_block *end);
void split_block(struct sf_block *block, size_t block_size, size_t payload_size, int free_list_index);

double _max_util = 0.0;
double _cur_util = 0.0;

void *sf_malloc(size_t size) {
    if (!size)
        return NULL;

    if (sf_mem_start() == sf_mem_end())
        for (int i = 0; i < NUM_FREE_LISTS; ++i)
            sf_free_list_heads[i].body.links.next = &sf_free_list_heads[i], sf_free_list_heads[i].body.links.prev = &sf_free_list_heads[i];

    size_t block_size = calc_required_block_size(size);

    for (int i = calc_fib_index(block_size); i < NUM_FREE_LISTS - 1; ++i)
    {
        struct sf_block *next = sf_free_list_heads[i].body.links.next;
        size_t free_block_size = get_block_size(next);

        while (next != &sf_free_list_heads[i] && free_block_size < block_size)
        {
            next = next->body.links.next;
            free_block_size = get_block_size(next);
        }

        if (next == &sf_free_list_heads[i])
            continue;

        split_block(next, block_size, size, i);

        return next->body.payload;
    }

    if (sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next == &sf_free_list_heads[NUM_FREE_LISTS - 1] && !init_heap())
        return NULL;

    struct sf_block *wilderness = sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next;

    while (get_block_size(wilderness) < block_size)
        if (!extend_wilderness(wilderness))
            return NULL;

    split_block(wilderness, block_size, size, 9);

    _cur_util += size;
    if (_cur_util > _max_util)
        _max_util = _cur_util;

    return wilderness->body.payload;
}

void sf_free(void *pp) {
    if (!is_valid(pp))
        abort();
    
    struct sf_block *block = (struct sf_block *)((char *)pp - 16);   
    struct sf_block *next = (struct sf_block *)((char *)block + get_block_size(block));

    _cur_util -= get_payload_size(block);

    // Update Header
    block->header = get_block_size(block) | (block->header & 0x4);
    next->prev_footer = block->header;
    next->header = next->header & ~0x4;

    // Check Up
    if (!is_alloced(block->prev_footer))
    {
        size_t size = get_block_size(block);
        size_t prev_size = block->prev_footer & 0xFFFFFFF0;

        block = (struct sf_block *)((char *)block - prev_size);

        block->header = (size + prev_size) | (block->header & 0xF);
        next->prev_footer = block->header;

        block->body.links.prev->body.links.next = block->body.links.next;
        block->body.links.next->body.links.prev = block->body.links.prev;
    }

    // Check Down
    if (!is_alloced(next->header))
    {
        size_t size = get_block_size(block);
        size_t next_size = get_block_size(next);

        struct sf_block *final = (struct sf_block *)((char *)next + next_size);

        block->header = (size + next_size) | (block->header & 0xF);
        final->prev_footer = block->header;
        final->header = final->header & ~0x7;

        if (next != sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next)
        {
            next->body.links.prev->body.links.next = next->body.links.next;
            next->body.links.next->body.links.prev = next->body.links.prev;
        }
    }

    // Update Free List
    if (next == sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next || next == sf_mem_end() - 16)
    {
        sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next = block;
        sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.prev = block;

        block->body.links.prev = &sf_free_list_heads[NUM_FREE_LISTS - 1];
        block->body.links.next = &sf_free_list_heads[NUM_FREE_LISTS - 1];
    }
    else
    {
        int fib_index = calc_fib_index(get_block_size(block));

        struct sf_block *first_free = sf_free_list_heads[fib_index].body.links.next;

        sf_free_list_heads[fib_index].body.links.next = block;
        first_free->body.links.prev = block;

        block->body.links.prev = &sf_free_list_heads[fib_index];
        block->body.links.next = first_free;
    }
}

void *sf_realloc(void *pp, size_t rsize) {
    if (!is_valid(pp))
        abort();
    
    if (rsize == 0)
    {
        sf_free(pp);
        return NULL;
    }
    
    struct sf_block *block = (struct sf_block *)((char *)pp - 16);
    size_t block_size = get_block_size(block);
    size_t payload_size = get_payload_size(block);

    if (rsize > payload_size)
    {
        if (block_size - rsize - 16 < sizeof(struct sf_block))
        {
            _cur_util += rsize - payload_size;
            return pp;
        }


        _cur_util -= payload_size;
        void *new_block = sf_malloc(rsize);
        memcpy(new_block, pp, payload_size);
        _cur_util += payload_size;
        sf_free(pp);
        return new_block;
    }

    size_t req_size = calc_required_block_size(rsize);

    if (block_size - req_size <= sizeof(struct sf_block))
    {
        block->header = (rsize << 32) | block_size | 0x8 | (block->header & 0x4);
        return block->body.payload;
    }

    struct sf_block *extra = (void *)((char *)block + req_size);
    size_t extra_size = block_size - req_size;

    block->header = (rsize << 32) | req_size | 0x8 | (block->header & 0x4);

    extra->prev_footer = block->header;
    extra->header = ((payload_size - rsize) << 32) | extra_size | 0xc;

    struct sf_block *next = (void *)((char *)extra + extra_size);
    next->prev_footer = extra->header;

    sf_free(extra->body.payload);

    return block->body.payload;
}

double sf_fragmentation() {
    int payload_sum = 0;
    int block_sum = 0;

    struct sf_block *block = sf_mem_start();
    
    while (block != sf_mem_end())
    {
        if (is_alloced(block->header))
        {
            block_sum += get_block_size(block);
            payload_sum += get_payload_size(block);
        }

        block += get_block_size(block);
    }

    if (block_sum == 0)
        return 0.0;
    return (double)payload_sum / block_sum;
}

double sf_utilization() {
    if (sf_mem_start() == sf_mem_end())
        return 0.0;
    return _max_util / (sf_mem_end() - sf_mem_start());
}

int is_valid(void *pp)
{
    if (pp == NULL || (size_t)pp % 16 != 0)
        sf_errno = EINVAL;

    struct sf_block *block = (struct sf_block *)((char *)pp - 16);
    size_t block_size = get_block_size(block);

    if (block_size < sizeof(struct sf_block) || block_size % 16 != 0)
        sf_errno = EINVAL;

    void *header = &block->header;
    void *footer = (void *)((char *)block + block_size - 8);

    if (header < sf_mem_start() + 8 || footer > sf_mem_end() - 8)
        sf_errno = EINVAL;

    if ((block->header & 0x8) != 0x8)
        sf_errno = EINVAL;

    if ((block->header & 0x4) != 0x4 && (block->prev_footer & 0x8) == 0x8)
        sf_errno = EINVAL;

    if (sf_errno == EINVAL)
        return 0;    
    return 1;
}

void split_block(struct sf_block *block, size_t min_block_size, size_t req_payload_size, int free_list_index)
{
    size_t cur_block_size = get_block_size(block);
    struct sf_block *next = (struct sf_block *)((char *)block + cur_block_size);

    int alloc_info = 0x8;
    if (block->prev_footer & 0x8)
        alloc_info = alloc_info | 0x4;

    if (cur_block_size >= min_block_size + sizeof(struct sf_block))
    {
        // Remove this block from free list
        block->body.links.prev->body.links.next = block->body.links.next;
        block->body.links.next->body.links.prev = block->body.links.prev;

        // Update Block Header
        block->header = (req_payload_size << 32) | min_block_size | alloc_info;

        // Create block from excess
        struct sf_block *extra = (struct sf_block *)((char *)block + (int)min_block_size);
        extra->prev_footer = block->header;
        extra->header = (cur_block_size - min_block_size) | 0x4;

        // Update Footer for excess block
        next->prev_footer = extra->header;

        // Add excess block to free list
        extra->body.links.prev = &sf_free_list_heads[free_list_index];
        extra->body.links.next = sf_free_list_heads[free_list_index].body.links.next;

        sf_free_list_heads[free_list_index].body.links.next = extra;
        extra->body.links.next->body.links.prev = extra;
    }
    else
    {
        // Remove from free list
        block->body.links.next->body.links.prev = block->body.links.prev;
        block->body.links.prev->body.links.next = block->body.links.next;

        // Update Block Header
        block->header = (req_payload_size << 32) | cur_block_size | alloc_info;
        
        // Update footer of next block
        next->prev_footer = block->header;
        next->header = next->header | 0x4;
    }
}

int init_heap()
{
    struct sf_block *wilderness;
    if (sf_mem_end() == sf_mem_start())
        wilderness = sf_mem_end();
    else
        wilderness = (void *)((char *)sf_mem_end() - 16);

    if (!extend_wilderness(wilderness))
        return 0;

    struct sf_block *prologue = sf_mem_start();
    prologue->header = (size_t)0x28;

    if (prologue == wilderness)
    {
        wilderness = (void *)((char *)sf_mem_start() + 32);
        wilderness->prev_footer = prologue->header;

        wilderness->header = (size_t)(sf_mem_end() - (void *)wilderness - 16);

        struct sf_block *epilogue = (void *)((char *)sf_mem_end() - 16);
        epilogue->header = (size_t)0x18;
        epilogue->prev_footer = wilderness->header;
    }

    init_wilderness(wilderness);

    return 1;
}

void init_wilderness(struct sf_block *wilderness)
{
    wilderness->body.links.next = &sf_free_list_heads[NUM_FREE_LISTS - 1];
    wilderness->body.links.prev = &sf_free_list_heads[NUM_FREE_LISTS - 1];

    sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.next = wilderness;
    sf_free_list_heads[NUM_FREE_LISTS - 1].body.links.prev = wilderness;
}

size_t get_block_size(struct sf_block *block) { return block->header & 0xFFFFFFF0; }

size_t get_payload_size(struct sf_block *block) { return (block->header >> 32) & 0xFFFFFFFF; }

int is_alloced(size_t header) { return header & 0x8; }

int calc_fib_index(size_t size)
{
    int fib_index = 0;

    int a = 1, b = 1;

    while (32*b < (int)size && fib_index < NUM_FREE_LISTS - 2)
    {
        int temp = b;
        b = b + a;
        a = temp;

        fib_index++;
    }

    return fib_index;
}

int extend_wilderness(struct sf_block *wilderness)
{
    if (!sf_mem_grow())
    {
        sf_errno = ENOMEM;
        return 0;
    }

    wilderness->header = (size_t)(sf_mem_end() - (void *)wilderness - 16);

    struct sf_block *epilogue = (void *)((char *)sf_mem_end() - 16);
    epilogue->header = (size_t)0x18;
    epilogue->prev_footer = wilderness->header;

    return 1;
}

size_t calc_required_block_size(size_t size)
{
    size_t aligned_size = size + 16;
    aligned_size = ((aligned_size + 15) >> 4) << 4;

    return aligned_size;
}