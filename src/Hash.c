#include <stdint.h>
#include "crypto/c_keccak.h"
#include "crypto/c_groestl.h"
#include "crypto/c_blake256.h"
#include "crypto/c_jh.h"
#include "crypto/c_skein.h"

#include "Hash.h"

#define HASH_SIZE	32

static void do_blake_hash(const void* input, size_t len, char* output) {
	blake256_hash((uint8_t*)output, input, len);
}

void do_groestl_hash(const void* input, size_t len, char* output) {
	groestl(input, len * 8, (uint8_t*)output);
}

static void do_jh_hash(const void* input, size_t len, char* output) {
	int r = jh_hash(HASH_SIZE * 8, input, 8 * len, (uint8_t*)output);
	assert((SUCCESS == r));
}

static void do_skein_hash(const void* input, size_t len, char* output) {
	int r = skein_hash(8 * HASH_SIZE, input, 8 * len, (uint8_t*)output);
	assert((SKEIN_SUCCESS == r));
}

static void (* const extra_hashes[4])(const void *, size_t, char *) = {
	do_blake_hash, do_groestl_hash, do_jh_hash, do_skein_hash
};

#pragma pack(push, 1)
union hash_state {
	uint8_t b[200];
	uint64_t w[25];
};
#pragma pack(pop)

void Hash(const char *data, size_t len, HashOut output)
{
	union hash_state hs;
	keccak(data, len, hs.b, sizeof(hs));
	extra_hashes[hs.b[0] & 3](hs.b, 200, output);
}
