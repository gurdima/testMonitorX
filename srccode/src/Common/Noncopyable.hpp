#pragma once

/**
 * @brief Should be used as a base class for classes that 
 *        prohibit copying and copy constructing
 */
class Noncopyable
{
 protected:
    Noncopyable() {}
    ~Noncopyable() {}
 private:
    Noncopyable( const Noncopyable& );
    const Noncopyable& operator=( const Noncopyable& );
};

