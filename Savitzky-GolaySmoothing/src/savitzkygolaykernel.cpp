/*
 * Performs Savitzky Golay Filtering
 *
 * Developed by Anubhav Rohatgi
 * Date 26/04/2016
 */
#include "savitzkygolaykernel.h"
#include <stdexcept>
#include <string>
#include <assert.h>
#include <math.h>

SavitzkyGolayKernel::SavitzkyGolayKernel(
        cv::Size const& size,
        cv::Point const& origin,
        int hor_degree, int vert_degree) :
        m_horDegree(hor_degree),
        m_vertDegree(vert_degree),
        m_width(size.width),
        m_height(size.height),
        m_numTerms(calcNumTerms(hor_degree,vert_degree)),
        m_numDataPoints(size.width * size.height )
{
    if(hor_degree < 0)
        throw std::invalid_argument("Sav Kernel : invalid horizontal degree");
    if(vert_degree < 0)
        throw std::invalid_argument("Sav Kernel : invalid vertical degree");
    if(m_numTerms > m_numDataPoints)
        throw std::invalid_argument("Sav Kernel : too high degree");

    //Lets allocate some memory now
    m_dataPoints.resize(m_numDataPoints, 0.0);
    m_coeffs.resize(m_numTerms);
    AlignArray<float,4>(m_numDataPoints).swap(m_kernel);

    //Build equations
    m_equations.reserve(m_numTerms * m_numDataPoints);
    for (int y = 1; y <= m_height; ++y) {
        for (int x = 1; x <= m_width; ++x) {
            double pow1 = 1.0;
            for (int i = 0; i <= m_vertDegree; ++i) {
                double pow2 = pow1;
                for (int j = 0; j <= m_horDegree; ++j) {
                    m_equations.push_back(pow2);
                    pow2 *= x;
                }
                pow1 *= y;
            }
        }
    }

    QR(); //factorize
    //recalulate factors now from origin point
    recalcForOrigin(origin);
}


void SavitzkyGolayKernel::QR()
{
    m_rotations.clear();
    m_rotations.reserve(
        m_numTerms * (m_numTerms - 1) / 2
        + (m_numDataPoints - m_numTerms) * m_numTerms
    );

    int jj = 0; // j * m_numTerms + j
    for (int j = 0; j < m_numTerms; ++j, jj += m_numTerms + 1) {
        int ij = jj + m_numTerms; // i * m_numTerms + j
        for (int i = j + 1; i < m_numDataPoints; ++i, ij += m_numTerms) {
            double const a = m_equations[jj];
            double const b = m_equations[ij];

            if (b == 0.0) {
                m_rotations.push_back(Rotation(1.0, 0.0));
                continue;
            }

            double sin, cos;

            if (a == 0.0) {
                cos = 0.0;
                sin = copysign(1.0, b);
                m_equations[jj] = fabs(b);
            } else if (fabs(b) > fabs(a)) {
                double const t = a / b;
                double const u = copysign(sqrt(1.0 + t*t), b);
                sin = 1.0 / u;
                cos = sin * t;
                m_equations[jj] = b * u;
            } else {
                double const t = b / a;
                double const u = copysign(sqrt(1.0 + t*t), a);
                cos = 1.0 / u;
                sin = cos * t;
                m_equations[jj] = a * u;
            }
            m_equations[ij] = 0.0;

            m_rotations.push_back(Rotation(sin, cos));

            int ik = ij + 1; // i * m_numTerms + k
            int jk = jj + 1; // j * m_numTerms + k
            for (int k = j + 1; k < m_numTerms; ++k, ++ik, ++jk) {
                double const temp = cos * m_equations[jk] + sin * m_equations[ik];
                m_equations[ik] = cos * m_equations[ik] - sin * m_equations[jk];
                m_equations[jk] = temp;
            }
        }
    }
}


void SavitzkyGolayKernel::recalcForOrigin(cv::Point const& origin)
{
    std::fill(m_dataPoints.begin(), m_dataPoints.end(), 0.0);
    m_dataPoints[origin.y * m_width + origin.x] = 1.0;

    // Rotate data points.
    double* const dp = &m_dataPoints[0];
    std::vector<Rotation>::const_iterator rot(m_rotations.begin());
    for (int j = 0; j < m_numTerms; ++j) {
        for (int i = j + 1; i < m_numDataPoints; ++i, ++rot) {
            double const temp = rot->cos * dp[j] + rot->sin * dp[i];
            dp[i] = rot->cos * dp[i] - rot->sin * dp[j];
            dp[j] = temp;
        }
    }

    // Solve R*x = d by back-substitution.
    int ii = m_numTerms * m_numTerms - 1; // i * m_numTerms + i
    for (int i = m_numTerms - 1; i >= 0; --i, ii -= m_numTerms + 1) {
        double sum = dp[i];
        int ik = ii + 1;
        for (int k = i + 1; k < m_numTerms; ++k, ++ik) {
            sum -= m_equations[ik] * m_coeffs[k];
        }

        assert(m_equations[ii] != 0.0);
        m_coeffs[i] = sum / m_equations[ii];
    }

    int ki = 0;
    for (int y = 1; y <= m_height; ++y) {
        for (int x = 1; x <= m_width; ++x) {
            double sum = 0.0;
            double pow1 = 1.0;
            int ci = 0;
            for (int i = 0; i <= m_vertDegree; ++i) {
                double pow2 = pow1;
                for (int j = 0; j <= m_horDegree; ++j) {
                    sum += pow2 * m_coeffs[ci];
                    ++ci;
                    pow2 *= x;
                }
                pow1 *= y;
            }
            m_kernel[ki] = (float)sum;
            ++ki;
        }
    }
}

