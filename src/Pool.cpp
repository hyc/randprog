#include <cstdlib>
#include <cassert>

#include "Pool.h"

#ifdef USE_VALGRIND
#include <valgrind/memcheck.h>
#define VGMEMP_MARK(m,s)    VALGRIND_MAKE_MEM_NOACCESS(m,s)
#define VGMEMP_CREATE(h,r,z)    VALGRIND_CREATE_MEMPOOL(h,r,z)
#define VGMEMP_TRIM(h,a,s)  VALGRIND_MEMPOOL_TRIM(h,a,s)
#define VGMEMP_ALLOC(h,a,s) VALGRIND_MEMPOOL_ALLOC(h,a,s)
#else
#define VGMEMP_MARK(m,s)
#define VGMEMP_CREATE(h,r,z)
#define VGMEMP_TRIM(h,a,s)
#define VGMEMP_ALLOC(h,a,s)
#endif

static thread_local char *pool_beg, *pool_end, *pool_ptr;

void pool_init(size_t size) {
	pool_beg = (char *)malloc(size);
	pool_end = pool_beg + size;
	pool_ptr = pool_beg;
	VGMEMP_MARK(pool_beg, size);
	VGMEMP_CREATE(&pool_beg, 0, 0);
}

void pool_reset() {
	pool_ptr = pool_beg;
	VGMEMP_TRIM(&pool_beg, pool_beg, 0);
}

void* pool_alloc(size_t n)
{
	n += (sizeof(void *)-1) & ~(sizeof(void *)-1);
	assert(pool_ptr + n < pool_end);

	auto r(pool_ptr);

	pool_ptr += n;
	VGMEMP_ALLOC(&pool_beg, r, n);

	return r;
}
