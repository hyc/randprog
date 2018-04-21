#include <cstdlib>
#include <cassert>

#include "Pool.h"

static thread_local char *pool_beg, *pool_end, *pool_ptr;

void pool_init(size_t size) {
	pool_beg = (char *)malloc(size);
	pool_end = pool_beg + size;
	pool_ptr = pool_beg;
}

void pool_reset() {
	pool_ptr = pool_beg;
}

void* pool_alloc(size_t n)
{
	n += (sizeof(void *)-1) & ~(sizeof(void *)-1);
	assert(pool_ptr + n < pool_end);

	auto r(pool_ptr);

	pool_ptr += n;

	return r;
}
