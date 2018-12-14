#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Helpers.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Types.h"
#include "Poco/Exception.h"
/*---------------------------------------------------------------------------*/

/**
 * @brief Get low byte of word
 */
inline Poco::UInt8 getLowByte(const Poco::UInt16& word)
{
    return static_cast<Poco::UInt8>(word);
}

/**
 * @brief Get high byte of word
 */
inline Poco::UInt8 getHiByte(const Poco::UInt16& word)
{
    return static_cast<Poco::UInt8>((word >> 8) & 0xFF);
}

/*---------------------------------------------------------------------------*/

/**
 * @brief Get low word of dword
 */
inline Poco::UInt16 getLowWord(const Poco::UInt32& dword)
{
    return static_cast<Poco::UInt16>(dword);
}

/**
 * @brief Get high word of dword
 */
inline Poco::UInt16 getHiWord(const Poco::UInt32& dword)
{
    return static_cast<Poco::UInt16>((dword >> 16) & 0xFFFF);
}

/*---------------------------------------------------------------------------*/

/**
 * @brief Swap high and low bytes in word
 */
inline Poco::UInt16 swapBytes(const Poco::UInt16& word)
{
    Poco::UInt16 res = getLowByte(word) << 8 | getHiByte(word);
    return res;
}

/*---------------------------------------------------------------------------*/

/**
 * @brief Set standalone byte value in variable
 * @param dst Variable that contains byte to change
 * @param src Value of byte to write
 * @param byteNum Number of byte to change
 */
template <typename T>
void setByte(T& dst, const Poco::UInt8& src, const Poco::UInt8 byteNum)
{
    if(sizeof(dst) <= byteNum) {
        throw Poco::RangeException("setByte failure: byte num is out of range");
    }
    
    *(reinterpret_cast<Poco::UInt8*>(&dst) + byteNum) = src;
}

/**
 * @brief Get standalone byte value in variable
 * @param dst Variable that contains byte to read
 * @param byteNum Number of byte to read
 */
template <typename T>
Poco::UInt8 getByte(const T& dst, const Poco::UInt8 byteNum)
{
    if(sizeof(dst) <= byteNum) {
        throw Poco::RangeException("getByte failure: byte num is out of range");
    }

    return *(reinterpret_cast<const Poco::UInt8*>(&dst) + byteNum);
}

/*---------------------------------------------------------------------------*/

/**
 * @brief Set bits in 'data' that are set in 'mask'
 */
template <typename T>
void setBits(T& data, const T& mask)
{
    data |= mask;
}

/**
 * @brief Clear bits in 'data' that are set in 'mask'
 */
template <typename T>
void clearBits(T& data, const T& mask)
{
    data &= ~mask;
}

/**
 * @brief Set/clear bits in 'data' that are set in 'mask'
 * @param set True to set, false to clear
 */
template <typename T>
void changeBits(T& data, const T& mask, bool set)
{
    set ? setBits(data, mask) : clearBits(data, mask);
}

/*---------------------------------------------------------------------------*/

/**
 * @brief Get number of first set bit
 */
template <typename T>
unsigned char calculateBitOffset(const T& value)
{
    unsigned char res = 0;

    for(unsigned char i = 0; i < sizeof(value) * 8; ++i) {
        if(value & (1UL << i)) {
            res = i;
            break;
        }
    }

    return res;
}
