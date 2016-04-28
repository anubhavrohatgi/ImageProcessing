/*
 * Calculates the Savitzky Golay Filtering kernel
 *
 * Developed by Anubhav Rohatgi
 * Date 26/04/2016
 */
#pragma once

#ifndef SAVITZKYGOLAYKERNEL_H
#define SAVITZKYGOLAYKERNEL_H


#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "alignarray.h"
/**
 * @brief The SavitzkyGolayKernel class Calculates the Savitzky - Golay Filter
 *          kernel.
 */
class SavitzkyGolayKernel
{
public:
    SavitzkyGolayKernel(cv::Size const& size,
                        cv::Point const& origin,
                        int hor_degree, int vert_degree);

    void recalcForOrigin(cv::Point const& origin);

    int width() const {
        return m_width;
    }

    int height() const {
        return m_height;
    }

    float const* data() const {
        return m_kernel.data();
    }

    float operator[](size_t index) const {
        return m_kernel[index];
    }

private:

    struct Rotation {
        double sin;
        double cos;

        Rotation(double s, double c) : sin(s), cos(c) {}
    };

    /**
     * @brief QR Factorization
     */
    void QR();


    /**
     * @brief m_equations A matrix of m_numDataPoints rows and
     *      m_numVars columns. Stored row wise (rows strides)
     */
    std::vector<double> m_equations;

    /**
     * @brief m_dataPoints number of data points stored row wise
     */
    std::vector<double> m_dataPoints;

    /**
     * @brief m_coeffs Polynomial coefficients of size m_numVars.
     */
    std::vector<double> m_coeffs;


    /**
     * @brief m_rotations The rotations applied to m_equations as part of
     *      QR factorization. Later on same rotations are applied to copy
     *      of m_dataPoints.
     */
    std::vector<Rotation> m_rotations;

    /**
     * @brief m_kernel A 16-byte convolution kernel of size m_numDataPoints.
     */
    AlignArray<float,4> m_kernel;


    /**
     * @brief m_horDegree degree of polynomial in horizontal direction.
     */
    int m_horDegree;

    /**
     * @brief m_vertDegree  degree of polynomial in vertical direction.
     */
    int m_vertDegree;

    /**
     * @brief m_width  Dimensions of the convolution kernel
     * @brief m_height
     */
    int m_width;
    int m_height;

    /**
     * @brief m_numTerms The number of terms in polynomial expression.
     */
    int m_numTerms;

    /**
     * @brief m_numDataPoints The number of data points. This corresponds
     *      to the number of items in the convolution kernel.
     */
    int m_numDataPoints;
};

/**
 * @brief calcNumTerms Calculates the number of terms in the polynomial
 * @param hor_degree Horizontal Degree
 * @param vert_degree Vertical Degree
 * @return outputs the number of terms.
 */
inline int calcNumTerms(int const hor_degree, int const vert_degree)
{
    return (hor_degree + 1) * (vert_degree + 1);
}


#endif // SAVITZKYGOLAYKERNEL_H
