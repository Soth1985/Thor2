//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include <Thor/Core/Hash/ThMurmurHash3.h>

using namespace Thor;
using namespace Thor::Hash;

THOR_INLINE uint32_t rotl32(uint32_t x, int8_t r)
{
  return (x << r) | (x >> (32 - r));
}

THOR_INLINE uint64_t rotl64(uint64_t x, int8_t r)
{
  return (x << r) | (x >> (64 - r));
}

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

THOR_INLINE ThU32 getblock32(const ThU32* p, int i)
{
  return p[i];
}

THOR_INLINE ThU64 getblock64(const ThU64* p, int i)
{
  return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

THOR_INLINE ThU32 fmix32(ThU32 h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//----------

THOR_INLINE ThU64 fmix64(ThU64 k)
{
  k ^= k >> 33;
  k *= UINT64_C(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= UINT64_C(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}

//-----------------------------------------------------------------------------

ThU32 Thor::Hash::Murmur3(const ThU8* key, ThSize len, ThU32 seed)
{
  const ThU8* data = key;
  const ThI32 nblocks = len / 4;

  ThU32 h1 = seed;

  const ThU32 c1 = 0xcc9e2d51;
  const ThU32 c2 = 0x1b873593;

  const ThU32* blocks = (const ThU32*)(data + nblocks * 4);

  for(ThI32 i = -nblocks; i; i++)
  {
    ThU32 k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = rotl32(k1,15);
    k1 *= c2;
    
    h1 ^= k1;
    h1 = rotl32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const ThU8* tail = (const ThU8*)(data + nblocks * 4);

  ThU32 k1 = 0;

  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  return h1;
}
