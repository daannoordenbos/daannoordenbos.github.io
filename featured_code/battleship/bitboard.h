#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

// for 128 bit register
#include <emmintrin.h>

// Use 'alignas' to align the blocks properly, useful since we use a 128 bit union type.
// When the CPU loads it can load it all at once.
struct alignas(16) Bitboard
{
    union
    {
        uint64_t p[2];
        __m128i m;
    };

    /** Initializers **/
    Bitboard() = default;
    Bitboard(const Bitboard& bb) : m(bb.m) {} // Use copy constructor directly

    Bitboard(const uint64_t p0, const uint64_t p1)
    {
        m = _mm_set_epi64x(p1, p0);
    }

    // create an empty or filled bit board
    Bitboard(bool filled)
    {
        m = (filled ? _mm_set_epi64x(UINT64_C(0x0000000FFFFFFFFF), UINT64_C(0xFFFFFFFFFFFFFFFF)) : _mm_setzero_si128());
    }

    /** Bitboard operators (SIMD parallelization) **/
    Bitboard& operator<<=(int shift)
    {
        p[1] = (p[1] << shift) | (p[0] >> (64 - shift));
        p[0] <<= shift;
        return *this;
    }

    Bitboard& operator>>=(int shift)
    {
        p[0] = (p[0] >> shift) | (p[1] << (64 - shift));
        p[1] >>= shift;
        return *this;
    }

    Bitboard operator<<(int shift) const
    {
        Bitboard result(*this);
        result <<= shift;
        return result;
    }

    Bitboard operator>>(int shift) const
    {
        Bitboard result(*this);
        result >>= shift;
        return result;
    }

    Bitboard& operator|=(const Bitboard& b1)
    {
        m = _mm_or_si128(m, b1.m);
        return *this;
    }

    Bitboard& operator&=(const Bitboard& b1)
    {
        m = _mm_and_si128(m, b1.m);
        return *this;
    }

    Bitboard& operator^=(const Bitboard& b1)
    {
        m = _mm_xor_si128(m, b1.m);
        return *this;
    }

    Bitboard& operator+=(const Bitboard& b1)
    {
        m = _mm_add_epi64(m, b1.m);
        return *this;
    }

    Bitboard& operator-=(const Bitboard& b1)
    {
        m = _mm_sub_epi64(m, b1.m);
        return *this;
    }

    Bitboard operator&(const Bitboard& rhs) const
    {
        Bitboard result;
        result.m = _mm_and_si128(m, rhs.m);
        return result;
    }

    Bitboard operator|(const Bitboard& rhs) const
    {
        Bitboard result;
        result.m = _mm_or_si128(m, rhs.m);
        return result;
    }

    Bitboard operator^(const Bitboard& rhs) const
    {
        Bitboard result;
        result.m = _mm_xor_si128(m, rhs.m);
        return result;
    }

    Bitboard operator+(const Bitboard& rhs) const
    {
        Bitboard result;
        result.m = _mm_add_epi64(m, rhs.m);
        return result;
    }

    Bitboard operator-(const Bitboard& rhs) const
    {
        Bitboard result;
        result.m = _mm_sub_epi64(m, rhs.m);
        return result;
    }

    Bitboard operator~() const
    {
        Bitboard result;
        result.m = _mm_andnot_si128(m, _mm_set1_epi64x(static_cast<int64_t>(0xFFFFFFFFFFFFFFFF)));
        return result;
    }

    bool operator==(const Bitboard& rhs) const
    {
        return (this->p[0] == rhs.p[0]) && (this->p[1] == rhs.p[1]);
    }

    bool operator!=(const Bitboard& rhs) const
    {
        return (this->p[0] != rhs.p[0]) || (this->p[1] != rhs.p[1]);
    }

    operator bool() const
    {
        return (p[0] | p[1]) != 0;
    }

    /** Bitboard operations **/
    int BSF() const
    {
        return (p[0] ? __builtin_ctzll(p[0]) : 64 + __builtin_ctzll(p[1]));
    }

    inline int count() const
    {
        return __builtin_popcountll(p[0]) + __builtin_popcountll(p[1]);
    }

    void set(const uint64_t p0, const uint64_t p1)
    {
        m = _mm_set_epi64x(p1, p0);
    }

    uint64_t merge() const
    {
        return p[0] | p[1];
    }

    void removeLSB()
    {
        if (p[0] != 0)
        {
            p[0] &= (p[0] - 1);
        }
        else
        {
            p[1] &= (p[1] - 1);
        }
    }

    Bitboard adjacent();


    /** Auxiliary functions **/
	void print() const;
};

const Bitboard squareMask[100] = {
    Bitboard(UINT64_C(1) <<  0,                 0),
    Bitboard(UINT64_C(1) <<  1,                 0),
    Bitboard(UINT64_C(1) <<  2,                 0),
    Bitboard(UINT64_C(1) <<  3,                 0),
    Bitboard(UINT64_C(1) <<  4,                 0),
    Bitboard(UINT64_C(1) <<  5,                 0),
    Bitboard(UINT64_C(1) <<  6,                 0),
    Bitboard(UINT64_C(1) <<  7,                 0),
    Bitboard(UINT64_C(1) <<  8,                 0),
    Bitboard(UINT64_C(1) <<  9,                 0),
    Bitboard(UINT64_C(1) << 10,                 0),
    Bitboard(UINT64_C(1) << 11,                 0),
    Bitboard(UINT64_C(1) << 12,                 0),
    Bitboard(UINT64_C(1) << 13,                 0),
    Bitboard(UINT64_C(1) << 14,                 0),
    Bitboard(UINT64_C(1) << 15,                 0),
    Bitboard(UINT64_C(1) << 16,                 0),
    Bitboard(UINT64_C(1) << 17,                 0),
    Bitboard(UINT64_C(1) << 18,                 0),
    Bitboard(UINT64_C(1) << 19,                 0),
    Bitboard(UINT64_C(1) << 20,                 0),
    Bitboard(UINT64_C(1) << 21,                 0),
    Bitboard(UINT64_C(1) << 22,                 0),
    Bitboard(UINT64_C(1) << 23,                 0),
    Bitboard(UINT64_C(1) << 24,                 0),
    Bitboard(UINT64_C(1) << 25,                 0),
    Bitboard(UINT64_C(1) << 26,                 0),
    Bitboard(UINT64_C(1) << 27,                 0),
    Bitboard(UINT64_C(1) << 28,                 0),
    Bitboard(UINT64_C(1) << 29,                 0),
    Bitboard(UINT64_C(1) << 30,                 0),
    Bitboard(UINT64_C(1) << 31,                 0),
    Bitboard(UINT64_C(1) << 32,                 0),
    Bitboard(UINT64_C(1) << 33,                 0),
    Bitboard(UINT64_C(1) << 34,                 0),
    Bitboard(UINT64_C(1) << 35,                 0),
    Bitboard(UINT64_C(1) << 36,                 0),
    Bitboard(UINT64_C(1) << 37,                 0),
    Bitboard(UINT64_C(1) << 38,                 0),
    Bitboard(UINT64_C(1) << 39,                 0),
    Bitboard(UINT64_C(1) << 40,                 0),
    Bitboard(UINT64_C(1) << 41,                 0),
    Bitboard(UINT64_C(1) << 42,                 0),
    Bitboard(UINT64_C(1) << 43,                 0),
    Bitboard(UINT64_C(1) << 44,                 0),
    Bitboard(UINT64_C(1) << 45,                 0),
    Bitboard(UINT64_C(1) << 46,                 0),
    Bitboard(UINT64_C(1) << 47,                 0),
    Bitboard(UINT64_C(1) << 48,                 0),
    Bitboard(UINT64_C(1) << 49,                 0),
    Bitboard(UINT64_C(1) << 50,                 0),
    Bitboard(UINT64_C(1) << 51,                 0),
    Bitboard(UINT64_C(1) << 52,                 0),
    Bitboard(UINT64_C(1) << 53,                 0),
    Bitboard(UINT64_C(1) << 54,                 0),
    Bitboard(UINT64_C(1) << 55,                 0),
    Bitboard(UINT64_C(1) << 56,                 0),
    Bitboard(UINT64_C(1) << 57,                 0),
    Bitboard(UINT64_C(1) << 58,                 0),
    Bitboard(UINT64_C(1) << 59,                 0),
    Bitboard(UINT64_C(1) << 60,                 0),
    Bitboard(UINT64_C(1) << 61,                 0),
    Bitboard(UINT64_C(1) << 62,                 0),
    Bitboard(UINT64_C(1) << 63,                 0),
    Bitboard(                0, UINT64_C(1) <<  0),
    Bitboard(                0, UINT64_C(1) <<  1),
    Bitboard(                0, UINT64_C(1) <<  2),
    Bitboard(                0, UINT64_C(1) <<  3),
    Bitboard(                0, UINT64_C(1) <<  4),
    Bitboard(                0, UINT64_C(1) <<  5),
    Bitboard(                0, UINT64_C(1) <<  6),
    Bitboard(                0, UINT64_C(1) <<  7),
    Bitboard(                0, UINT64_C(1) <<  8),
    Bitboard(                0, UINT64_C(1) <<  9),
    Bitboard(                0, UINT64_C(1) << 10),
    Bitboard(                0, UINT64_C(1) << 11),
    Bitboard(                0, UINT64_C(1) << 12),
    Bitboard(                0, UINT64_C(1) << 13),
    Bitboard(                0, UINT64_C(1) << 14),
    Bitboard(                0, UINT64_C(1) << 15),
    Bitboard(                0, UINT64_C(1) << 16),
    Bitboard(                0, UINT64_C(1) << 17),
    Bitboard(                0, UINT64_C(1) << 18),
    Bitboard(                0, UINT64_C(1) << 19),
    Bitboard(                0, UINT64_C(1) << 20),
    Bitboard(                0, UINT64_C(1) << 21),
    Bitboard(                0, UINT64_C(1) << 22),
    Bitboard(                0, UINT64_C(1) << 23),
    Bitboard(                0, UINT64_C(1) << 24),
    Bitboard(                0, UINT64_C(1) << 25),
    Bitboard(                0, UINT64_C(1) << 26),
    Bitboard(                0, UINT64_C(1) << 27),
    Bitboard(                0, UINT64_C(1) << 28),
    Bitboard(                0, UINT64_C(1) << 29),
    Bitboard(                0, UINT64_C(1) << 30),
    Bitboard(                0, UINT64_C(1) << 31),
    Bitboard(                0, UINT64_C(1) << 32),
    Bitboard(                0, UINT64_C(1) << 33),
    Bitboard(                0, UINT64_C(1) << 34),
    Bitboard(                0, UINT64_C(1) << 35)
};

#endif // BITBOARD_H_INCLUDED
