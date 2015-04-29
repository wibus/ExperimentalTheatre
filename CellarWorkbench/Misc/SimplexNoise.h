/* Copyright (c) 2007-2012 Eliot Eshelman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* This is an adaptation of Perlin's Simplex Noise code coming from
 * an "C++" implementation of Eliot Eshelman. It was included into a class
 * for conveniance, but it can be used staticaly.
 */

/* // Original description of the functions by Eliot Eshelman
   // This text only concerns the noiseXd() methods

    2D, 3D and 4D Simplex Noise functions return 'random' values in (-1, 1).

    This algorithm was originally designed by Ken Perlin, but my code has been
    adapted from the implementation written by Stefan Gustavson (stegu@itn.liu.se)
 */

#ifndef CELLARWORKBENCH_SIMPLEXNOISE_H
#define CELLARWORKBENCH_SIMPLEXNOISE_H

#include "../libCellarWorkbench_global.h"


namespace cellar
{

/// Class offering static simplex noise functions for 2, 3 and 4 dimensions.
/// 'Tile' versions can be used to produce juxtaposable identical tiles
/// (i.e. junctions will not be noticeable)
class CELLAR_EXPORT SimplexNoise
{
public:
    /// 2 dimensions simplex noise function
    /// \param[in]  x First dimension parameter
    /// \param[in]  y Second dimension parameter
    /// \return Floating point number in the range [-1, 1]
    static float noise2d(float x, float y);

    /// 3 dimensions simplex noise function
    /// \param[in]  x First dimension parameter
    /// \param[in]  y Second dimension parameter
    /// \param[in]  z Third dimension parameter
    /// \return Floating point number in the range [-1, 1]
    static float noise3d(float x, float y, float z);

    /// 4 dimensions simplex noise function
    /// \param[in]  x First dimension parameter
    /// \param[in]  y Second dimension parameter
    /// \param[in]  z Third dimension parameter
    /// \param[in]  w Fourth dimension parameter
    /// \return Floating point number in the range [-1, 1]
    static float noise4d(float x, float y, float z, float w);

    /// 1D Simplex noise function where F(x=0) = F(x=1) and F'(x=0) = F'(x=1),
    /// where F(x) is the simplex noise function and F'(x) its first derivative.
    /// \param[in]  x First dimension parameter
    /// \param[in]  radius Radius of the loop
    /// \return Floating point number in the range [-1, 1]
    static float noiseTile1d(float x, float radius=1.0f);

    /// 2D Simplex noise function where
    /// F(x=0, y=b) = F(x=1, y=b),
    /// F(x=a, y=0) = F(x=a, y=1),
    /// F'(x=0, y=b) = F'(x=1, y=b),
    /// F'(x=a, y=0) = F'(x=a, y=1),
    /// where F(x, y) is the simplex noise function and
    /// F'(x, y) its first derivative.
    /// \param[in]  x First dimension parameter
    /// \param[in]  y Second dimension parameter
    /// \param[in]  radius Radius of the loops
    /// \return Floating point number in the range [-1, 1]
    static float noiseTile2d(float x, float y, float radius=1.0f);

private:
    /// Deleted default constructor
    SimplexNoise() = delete;
    /// Deleted copy constructor
    SimplexNoise(const SimplexNoise& other) = delete;
    /// Deleted assignement operator
    SimplexNoise& operator=(const SimplexNoise& other) = delete;

    /// Dot product between a 2D vectors
    /// \param[in] g 2D vector
    /// \param[in] x First coponent of a 2D vector
    /// \param[in] y Second component of a 2D vector
    /// \return Dot product of Vector(g[0, 1]) and Vector(x, y)
    static float dot(const int* g, const float x, const float y);

    /// Dot product between a 3D vectors
    /// \param[in] g 3D vector
    /// \param[in] x First coponent of a 3D vector
    /// \param[in] y Second component of a 3D vector
    /// \param[in] z Third component of a 3D vector
    /// \return Dot product of Vector(g[0, 1, 2]) and Vector(x, y, z)
    static float dot(const int* g, const float x, const float y, const float z);

    /// Dot product between a 3D vectors
    /// \param[in] g 3D vector
    /// \param[in] x First coponent of a 3D vector
    /// \param[in] y Second component of a 3D vector
    /// \param[in] z Third component of a 3D vector
    /// \param[in] w Fourth component of a 3D vector
    /// \return Dot product of Vector(g[0, 1, 2, 3]) and Vector(x, y, z, w)
    static float dot(const int* g, const float x, const float y, const float z, const float w);

    static const int grad3[12][3];
    static const int grad4[32][4];
    static const int perm[512];
    static const int simplex[64][4];
};
}

#endif // CELLARWORKBENCH_SIMPLEXNOISE_H
