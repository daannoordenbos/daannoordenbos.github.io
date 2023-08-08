#include <iostream>
#include "bitboard.h"

void Bitboard::print() const
{
    std::cout << "Bitboard:\n";
    for (int i = 0; i < 100; ++i)
    {
        bool isSet = (*this & squareMask[i]);
        std::cout << (isSet ? " X" : " -") << ((i % 10 == 9) ? "\n" : "");
    }
    std::cout << "\n";
}

Bitboard Bitboard::adjacent()
    {
        Bitboard result(*this);
        const Bitboard d1(0xEFFBFEFFBFEFFBFE, 0x0000000FFBFEFFBF);
        const Bitboard d2(0xF7FDFF7FDFF7FDFF, 0x00000007FDFF7FDF);
        const Bitboard d3(0xFFFFFFFFFFFFFC00, 0x0000000FFFFFFFFF);
        const Bitboard d4(0xFFFFFFFFFFFFFFFF, 0x0000000003FFFFFF);
        return (((result & d1) >> 1) | ((result & d2) << 1) | ((result & d3) >> 10) | ((result & d4) << 10)) & (~result);
    }
