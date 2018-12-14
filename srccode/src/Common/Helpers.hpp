#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Typedefs.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Net/StreamSocket.h"
#include "Poco/Exception.h"
/*---------------------------------------------------------------------------*/

#include <stddef.h> // for ssize_t
#include <string>
/*---------------------------------------------------------------------------*/

/**
 * @brief C++ style function to get array length (number of 
 *        elements)
 * @param T Type of array
 * @param N Number of array elements
 * @return size_t 
 * @retval number of array elements 
 */
template <typename T, size_t N>
inline
size_t sizeOfArray( const T(&)[ N ] )
{
  return N;
}

/**
 * @brief Force element to be not bigger than specified boundary
 * @param T 
 * @param elem Element to check/modify
 * @param bound Bound to compare element with
 */
template <typename T> void cutToUpperBound(T& elem, const T& bound)
{
    if(elem > bound) {
        elem = bound;
    }
}

/**
 * @brief Force element to be not smaller than specified 
 *        boundary
 * @param T 
 * @param elem Element to check/modify
 * @param bound Bound to compare element with
 */
template <typename T> void cutToLowerBound(T& elem, const T& bound)
{
    if(elem < bound) {
        elem = bound;
    }
}

/**
 * @brief Force element to be in specified range
 * 
 * @param T 
 * @param elem Element to check/modify
 * @param lowerBound Lower bound of range
 * @param upperBound Upper bound of range
 */
template <typename T> void cutToBounds(T& elem, const T& lowerBound, const T& upperBound)
{
    if(lowerBound > upperBound) {
        throw Poco::RangeException("cutToBounds: Upper bound is smaller than lower bound");
    }

    cutToLowerBound(elem, lowerBound);
    cutToUpperBound(elem, upperBound);
}

/**
 * @brief Read sofware version string
 */
std::string getSoftwareVersion();


bool isValidChannelNumber(const ChannelID& channel);
void checkChannelNumber(const ChannelID& channel);

bool isValidSubchannelNumber(const SubchannelID& subchannel);
void checkSubchannelNumber(const SubchannelID& channel);

bool readUntilNewline(Poco::Net::StreamSocket& socket, std::string& collector);

void sendString(Poco::Net::StreamSocket& socket, std::string& data);

bool readBytes(Poco::Net::StreamSocket& socket, char* buffer, const size_t number);
