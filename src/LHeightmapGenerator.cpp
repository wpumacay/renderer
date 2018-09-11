
#include "LHeightmapGenerator.h"

namespace engine
{

	LHeightmapGenerator::LHeightmapGenerator( int octaves, float persistance, float lacunarity, float noiseScale )
	{
		m_octaves = octaves;
		m_persistance = persistance;
		m_lacunarity = lacunarity;
		m_noiseScale = noiseScale;		

		for ( int o = 0; o < octaves; o++ )
		{
			m_octaveOffsets.push_back( LVec2( _RANDOM( -10000.0f, 10000.0f ),
											  _RANDOM( -10000.0f, 10000.0f ) ) );
		}

		// Initialize the permutation vector with ken perlin's reference permutation ...
		// and double it to avoid overflow

		permutations = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
						 8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
						 35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
						 134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
						 55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
						 18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
						 250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
						 189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 
						 43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
						 97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
						 107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
						 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

		permutations.insert( permutations.end(), permutations.begin(), permutations.end() );		
	}

	LHeightmapGenerator::LHeightmapGenerator()
	{
		m_octaves = 4;
		m_persistance = 0.5;
		m_lacunarity = 2;
		m_noiseScale = 10;

		for ( int o = 0; o < m_octaves; o++ )
		{
			m_octaveOffsets.push_back( LVec2( _RANDOM( -10000.0f, 10000.0f ),
											  _RANDOM( -10000.0f, 10000.0f ) ) );
		}


		// Initialize the permutation vector with ken perlin's reference permutation ...
		// and double it to avoid overflow

		permutations = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
						 8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
						 35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
						 134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
						 55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
						 18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
						 250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
						 189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 
						 43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
						 97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
						 107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
						 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

		permutations.insert( permutations.end(), permutations.begin(), permutations.end() );
	}

	LHeightmapGenerator::~LHeightmapGenerator()
	{
		permutations.clear();
	}

	float LHeightmapGenerator::getHeight( float x, float y )
	{
		//return 0.0f; // to test a flat patch

		float _ampl = 1;
		float _freq = 1;
		float _noiseHeight = 0;

		for ( int o = 0; o < m_octaves; o++ )
		{
			float _sampleX = x / m_noiseScale * _freq + m_octaveOffsets[ o ].x;
			float _sampleY = y / m_noiseScale * _freq + m_octaveOffsets[ o ].y;

			_noiseHeight += _perlin2d( _sampleX, _sampleY ) * _ampl;

			_ampl *= m_persistance;
			_freq *= m_lacunarity;
		}

		return _noiseHeight;
	}

	/* NOISE IMPLEMENTATION */

	float LHeightmapGenerator::_perlin2d( float x, float y )
	{
		// Calculate unit square position in grid ( wrap around by 255 )
		int _X = ( int ) floor( x ) & 255;
		int _Y = ( int ) floor( y ) & 255;

		// Calculate relative position [0-1] inside unit square
		float _xf = x - floor( x );
		float _yf = y - floor( y );

		// std::cout << "_xf: " << _xf << std::endl;
		// std::cout << "_yf: " << _yf << std::endl;

		// Smooth using ease function for smooth imterpolation
		float _u = _fade( _xf );
		float _v = _fade( _yf );

		// std::cout << "_u: " << _u << std::endl;
		// std::cout << "_v: " << _u << std::endl;

		int _hash00 = permutations[ permutations[ _X ] + _Y ];
		int _hash01 = permutations[ permutations[ _X ] + _Y + 1 ];
		int _hash10 = permutations[ permutations[ _X + 1 ] + _Y ];
		int _hash11 = permutations[ permutations[ _X + 1 ] + _Y + 1 ];

		float _d00 = _dotgrad( _hash00, _xf, _yf );
		float _d01 = _dotgrad( _hash01, _xf, _yf - 1 );
		float _d10 = _dotgrad( _hash10, _xf - 1, _yf );
		float _d11 = _dotgrad( _hash11, _xf - 1, _yf - 1 );

		float _val = _lerp( _lerp( _d00, _d10, _u ),
					  		_lerp( _d01, _d11, _u ),
					  		_v );
		// std::cout << "_val: " << _val << std::endl;
		return _val;
	}

	float LHeightmapGenerator::_lerp( float a, float b, float t )
	{
		return ( 1 - t ) * a + t * b;
	}

	float LHeightmapGenerator::_fade( float t )
	{
		return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
	}

	float LHeightmapGenerator::_dotgrad( int hash, float xf, float yf )
	{
		// Because we are in 2d, there are just 4 options for the gradient vectors ...
		// and so, just 4 options for the dot product

		switch ( hash & 0xf )
		{
			case 0x0 : return xf + yf;
			case 0x1 : return -xf + yf;
			case 0x2 : return xf - yf;
			case 0x3 : return -xf - yf;
			case 0x4 : return xf + yf;
			case 0x5 : return -xf + yf;
			case 0x6 : return xf - yf;
			case 0x7 : return -xf - yf;
			case 0x8 : return xf + yf;
			case 0x9 : return -xf + yf;
			case 0xa : return xf - yf;
			case 0xb : return -xf - yf;
			case 0xc : return xf + yf;
			case 0xd : return -xf + yf;
			case 0xe : return xf - yf;
			case 0xf : return -xf - yf;

			default : return 0.0f;
		}
	}
}