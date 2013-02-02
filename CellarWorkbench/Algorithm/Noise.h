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

#ifndef CELLARWORKBENCH_NOISE_H
#define CELLARWORKBENCH_NOISE_H

#include "libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT SimplexNoise
    {
    public:
        SimplexNoise();

        static float noise2d(const float x, const float y);
        static float noise3d(const float x, const float y, const float z);
        static float noise4d(const float x, const float y, const float z, const float w);

        static float noiseTile1d(const float x, const float zoom=1.0f);
        static float noiseTile2d(const float x, const float y, const float zoom=1.0f);

    private:
        static int fastfloor(const float x);

        static float dot(const int* g, const float x, const float y);
        static float dot(const int* g, const float x, const float y, const float z);
        static float dot(const int* g, const float x, const float y, const float z, const float w);

        static const int grad3[12][3];
        static const int grad4[32][4];
        static const int perm[512];
        static const int simplex[64][4];
    };
}

#endif // CELLARWORKBENCH_NOISE_H
