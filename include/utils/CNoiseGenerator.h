#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

#define _RANDOM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )

/*
*   Implementation of the perlin noise function based on these references :
*
*       c++ implementation of perlin3d https://github.com/sol-prog/Perlin_Noise/blob/master/PerlinNoise.cpp
*       javascript implementation of perlin2d https://github.com/josephg/noisejs/blob/master/perlin.js
*/

namespace engine
{

    class CNoiseGenerator
    {

    public :

        static void Init();
        static void Release();

        ~CNoiseGenerator();

        static void Config( int octaves, float persistance, float lacunarity, float noiseScale );
        static float GetNoise1d( float x );
        static float GetNoise2d( float x, float y );

    private :

        static CNoiseGenerator* s_instance;

        CNoiseGenerator();

        void _config( int octaves, float persistance, float lacunarity, float noiseScale );

        float _getNoise1d( float x );
        float _getNoise2d( float x, float y );
        float _fade( float t );
        float _lerp( float a, float b, float t );
        float _dotgrad( int hash, float x, float y );
        float _perlin2d( float x, float y );

        int m_octaves;
        float m_persistance;
        float m_lacunarity;
        float m_noiseScale;

        std::vector< int > m_permutations;
        std::vector< CVec2 > m_octaveOffsets;

    };

}

