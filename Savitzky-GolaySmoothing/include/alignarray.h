/*
 * This template specifies the alignment of the array
 * in terms of units. This will ultimately weild an optimize
 * SSE code  while we are working with image strides and
 * performing convolution of 2d Containers.
 *
 * Developed by Anubhav Rohatgi
 * Date 26/04/2016
 */
#pragma once

#ifndef ALIGNARRAY_H
#define ALIGNARRAY_H


#include <stddef.h>
#include <stdint.h>

#define DEFINE_NON_COPYABLE(Class) \
    private: \
    Class(Class const&); \
    Class& operator=(Class const&);

/**
 * @brief An array of elements starting at address with a
 *        specified alignment. The alignement is specified
 *        in terms of units where bytes = units * sizeof(T)
 */
template<typename T, size_t align_in_units>
class AlignArray
{
    DEFINE_NON_COPYABLE(AlignArray)

public:
        /**
         * @brief AlignArray Constructs a null array.
         */
    AlignArray() : m_pAlignedData(nullptr), m_pStorage(nullptr) { }

    AlignArray(size_t size);

    ~AlignArray() {
        delete[] m_pStorage;
    }

    T* data() {
        return m_pAlignedData;
    }

    T const* data() const {
        return m_pAlignedData;
    }

    T& operator[] (size_t index) {
        return m_pAlignedData[index];
    }

    T const& operator[] (size_t index) const {
        return m_pAlignedData[index];
    }

    void swap(AlignArray& other);

private:
    T* m_pAlignedData;
    T* m_pStorage;
};

template<typename T, size_t align_in_units>
inline void swap(AlignArray<T, align_in_units>& o1,
                 AlignArray<T, align_in_units>& o2)
{
    o1.swap(o2);
}



template<typename T, size_t align_in_units>
AlignArray<T, align_in_units>::AlignArray(size_t size)
{
    int const a = (align_in_units > 1) ? align_in_units : 1;
    int const am1 = a - 1;
    m_pStorage = new T[size + am1];
    m_pAlignedData = m_pStorage + ( (a - ((uintptr_t(m_pStorage)/sizeof(T)) & am1)) & am1);
}


template<typename T, size_t align_in_units>
void AlignArray<T, align_in_units>::swap(AlignArray& other)
{
    T* temp = m_pAlignedData;
    m_pAlignedData = other.m_pAlignedData;
    other.m_pAlignedData = temp;

    temp = m_pStorage;
    m_pStorage = other.m_pStorage;
    other.m_pStorage = temp;
}

#endif // ALIGNARRAY_H
