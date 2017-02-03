#pragma once
#include <Thor/Core/Common.h>

namespace Thor
{
    namespace Hash
    {        
        struct Sha3_Ctx
        {
            static const ThI32 sha3_224_hash_size = 28;
            static const ThI32 sha3_256_hash_size = 32;
            static const ThI32 sha3_384_hash_size = 48;
            static const ThI32 sha3_512_hash_size = 64;
            static const ThI32 sha3_max_permutation_size = 25;
            static const ThI32 sha3_max_rate_in_qwords = 24;
            /* 1600 bits algorithm hashing state */
            uint64_t hash[sha3_max_permutation_size];
            /* 1536-bit buffer for leftovers */
            uint64_t message[sha3_max_rate_in_qwords];
            /* count of bytes in the message[] buffer */
            unsigned rest;
            /* size of a message block processed at once */
            unsigned block_size;
        };

        void Sha3_224_Init(Sha3_Ctx* ctx);
        void Sha3_256_Init(Sha3_Ctx* ctx);
        void Sha3_384_Init(Sha3_Ctx* ctx);
        void Sha3_512_Init(Sha3_Ctx* ctx);
        void Sha3_Update(Sha3_Ctx* ctx, const ThU8* msg, size_t size);
        void Sha3_Final(Sha3_Ctx* ctx, ThU8* result);
    }
}
