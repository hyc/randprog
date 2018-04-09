// keccak.c
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>
// A baseline Keccak (3rd round) implementation.

// #include "hash-ops.h"
#include "c_keccak.h"

const uint64_t keccakf_rndc[24] = 
{
    0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
    0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
    0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
    0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
    0x8000000000008003, 0x8000000000008002, 0x8000000000000080, 
    0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
    0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

const int keccakf_rotc[24] = 
{
    1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14, 
    27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44
};

const int keccakf_piln[24] = 
{
    10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4, 
    15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1 
};

// update the state with given number of rounds


void keccakf(uint64_t st[25], int rounds)
{
    int i, j, round;
    uint64_t t, bc[5];

    for (round = 0; round < rounds; ++round) {

        // Theta
        bc[0] = st[0] ^ st[5] ^ st[10] ^ st[15] ^ st[20];
        bc[1] = st[1] ^ st[6] ^ st[11] ^ st[16] ^ st[21];
        bc[2] = st[2] ^ st[7] ^ st[12] ^ st[17] ^ st[22];
        bc[3] = st[3] ^ st[8] ^ st[13] ^ st[18] ^ st[23];
        bc[4] = st[4] ^ st[9] ^ st[14] ^ st[19] ^ st[24];

        for (i = 0; i < 5; ++i) {
            t = bc[(i + 4) % 5] ^ ROTL64(bc[(i + 1) % 5], 1);
            st[i     ] ^= t;
            st[i +  5] ^= t;
            st[i + 10] ^= t;
            st[i + 15] ^= t;
            st[i + 20] ^= t;
        }

        // Rho Pi
        t = st[1];
        for (i = 0; i < 24; ++i) {
            bc[0] = st[keccakf_piln[i]];
            st[keccakf_piln[i]] = ROTL64(t, keccakf_rotc[i]);
            t = bc[0];
        }

        //  Chi
        for (j = 0; j < 25; j += 5) {
            bc[0] = st[j    ];
            bc[1] = st[j + 1];
            bc[2] = st[j + 2];
            bc[3] = st[j + 3];
            bc[4] = st[j + 4];
            st[j    ] ^= (~bc[1]) & bc[2];
            st[j + 1] ^= (~bc[2]) & bc[3];
            st[j + 2] ^= (~bc[3]) & bc[4];
            st[j + 3] ^= (~bc[4]) & bc[0];
            st[j + 4] ^= (~bc[0]) & bc[1];
        }

        //  Iota
        st[0] ^= keccakf_rndc[round];
    }
}

/*
typedef uint64_t ulong;
#define rotate ROTL64

#define bitselect(a, b, c)	(((~(c)) & (a)) | ((c) & (b)))

void keccakf(ulong *s, int rnds)
{
    for(int i = 0; i < 24; ++i) 
    {
		ulong bc[5], xor[5], tmp1, tmp2;
        bc[0] = s[0] ^ s[5] ^ s[10] ^ s[15] ^ s[20] ^ rotate(s[2] ^ s[7] ^ s[12] ^ s[17] ^ s[22], 1UL);
        bc[1] = s[1] ^ s[6] ^ s[11] ^ s[16] ^ s[21] ^ rotate(s[3] ^ s[8] ^ s[13] ^ s[18] ^ s[23], 1UL);
        bc[2] = s[2] ^ s[7] ^ s[12] ^ s[17] ^ s[22] ^ rotate(s[4] ^ s[9] ^ s[14] ^ s[19] ^ s[24], 1UL);
        bc[3] = s[3] ^ s[8] ^ s[13] ^ s[18] ^ s[23] ^ rotate(s[0] ^ s[5] ^ s[10] ^ s[15] ^ s[20], 1UL);
        bc[4] = s[4] ^ s[9] ^ s[14] ^ s[19] ^ s[24] ^ rotate(s[1] ^ s[6] ^ s[11] ^ s[16] ^ s[21], 1UL);
        
        tmp1 = s[1] ^ bc[0];
        
        s[0] ^= bc[4];
        s[1] = rotate(s[6] ^ bc[0], 44UL);
        s[6] = rotate(s[9] ^ bc[3], 20UL);
        s[9] = rotate(s[22] ^ bc[1], 61UL);
        s[22] = rotate(s[14] ^ bc[3], 39UL);
        s[14] = rotate(s[20] ^ bc[4], 18UL);
        s[20] = rotate(s[2] ^ bc[1], 62UL);
        s[2] = rotate(s[12] ^ bc[1], 43UL);
        s[12] = rotate(s[13] ^ bc[2], 25UL);
        s[13] = rotate(s[19] ^ bc[3], 8UL);
        s[19] = rotate(s[23] ^ bc[2], 56UL);
        s[23] = rotate(s[15] ^ bc[4], 41UL);
        s[15] = rotate(s[4] ^ bc[3], 27UL);
        s[4] = rotate(s[24] ^ bc[3], 14UL);
        s[24] = rotate(s[21] ^ bc[0], 2UL);
        s[21] = rotate(s[8] ^ bc[2], 55UL);
        s[8] = rotate(s[16] ^ bc[0], 35UL);
        s[16] = rotate(s[5] ^ bc[4], 36UL);
        s[5] = rotate(s[3] ^ bc[2], 28UL);
        s[3] = rotate(s[18] ^ bc[2], 21UL);
        s[18] = rotate(s[17] ^ bc[1], 15UL);
        s[17] = rotate(s[11] ^ bc[0], 10UL);
        s[11] = rotate(s[7] ^ bc[1], 6UL);
        s[7] = rotate(s[10] ^ bc[4], 3UL);
        s[10] = rotate(tmp1, 1UL);
        
        tmp1 = s[0]; tmp2 = s[1]; s[0] = bitselect(s[0] ^ s[2], s[0], s[1]); s[1] = bitselect(s[1] ^ s[3], s[1], s[2]); s[2] = bitselect(s[2] ^ s[4], s[2], s[3]); s[3] = bitselect(s[3] ^ tmp1, s[3], s[4]); s[4] = bitselect(s[4] ^ tmp2, s[4], tmp1);
        tmp1 = s[5]; tmp2 = s[6]; s[5] = bitselect(s[5] ^ s[7], s[5], s[6]); s[6] = bitselect(s[6] ^ s[8], s[6], s[7]); s[7] = bitselect(s[7] ^ s[9], s[7], s[8]); s[8] = bitselect(s[8] ^ tmp1, s[8], s[9]); s[9] = bitselect(s[9] ^ tmp2, s[9], tmp1);
        tmp1 = s[10]; tmp2 = s[11]; s[10] = bitselect(s[10] ^ s[12], s[10], s[11]); s[11] = bitselect(s[11] ^ s[13], s[11], s[12]); s[12] = bitselect(s[12] ^ s[14], s[12], s[13]); s[13] = bitselect(s[13] ^ tmp1, s[13], s[14]); s[14] = bitselect(s[14] ^ tmp2, s[14], tmp1);
        tmp1 = s[15]; tmp2 = s[16]; s[15] = bitselect(s[15] ^ s[17], s[15], s[16]); s[16] = bitselect(s[16] ^ s[18], s[16], s[17]); s[17] = bitselect(s[17] ^ s[19], s[17], s[18]); s[18] = bitselect(s[18] ^ tmp1, s[18], s[19]); s[19] = bitselect(s[19] ^ tmp2, s[19], tmp1);
        tmp1 = s[20]; tmp2 = s[21]; s[20] = bitselect(s[20] ^ s[22], s[20], s[21]); s[21] = bitselect(s[21] ^ s[23], s[21], s[22]); s[22] = bitselect(s[22] ^ s[24], s[22], s[23]); s[23] = bitselect(s[23] ^ tmp1, s[23], s[24]); s[24] = bitselect(s[24] ^ tmp2, s[24], tmp1);
        s[0] ^= keccakf_rndc[i];
    }
}
*/

// compute a keccak hash (md) of given byte length from "in"
typedef uint64_t state_t[25];

int keccak(const uint8_t *in, int inlen, uint8_t *md, int mdlen)
{
    /*state_t st;
    uint8_t temp[144];
    int i, rsiz, rsizw;

    rsiz = sizeof(state_t) == mdlen ? HASH_DATA_AREA : 200 - 2 * mdlen;
    rsizw = rsiz / 8;
    
    memset(st, 0, sizeof(st));

    for ( ; inlen >= rsiz; inlen -= rsiz, in += rsiz) {
        for (i = 0; i < rsizw; i++)
            st[i] ^= ((uint64_t *) in)[i];
        keccakf(st, KECCAK_ROUNDS);
    }
    
    // last block and padding
    memcpy(temp, in, inlen);
    temp[inlen++] = 1;
    memset(temp + inlen, 0, rsiz - inlen);
    temp[rsiz - 1] |= 0x80;

    for (i = 0; i < rsizw; i++)
        st[i] ^= ((uint64_t *) temp)[i];

    keccakf(st, KECCAK_ROUNDS);

    memcpy(md, st, mdlen);

    return 0;*/
    
	uint64_t st[25];
	
	memcpy(st, in, 76);
	memset(&st[10], 0x00, 120);
	st[9] = (st[9] & 0x00000000FFFFFFFFULL) | 0x0000000100000000ULL;
	st[16] = 0x8000000000000000ULL;
	
	keccakf(st, 24);
	
	memcpy(md, st, 200);
	return(0);
}

void keccak1600(const uint8_t *in, int inlen, uint8_t *md)
{
    keccak(in, inlen, md, sizeof(state_t));
}
