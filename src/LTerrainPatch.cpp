
#include "LTerrainPatch.h"
#include "LMeshBuilder.h"

#include <cstdlib>

namespace engine
{


	LTerrainPatch::LTerrainPatch( float xp, float zp,
								  GLfloat patchWidth, GLfloat patchDepth,
								  int patchWidthDiv, int patchDepthDiv,
								  LHeightmapGenerator* pGenerator )
	{
		// std::cout << "created new patch" << std::endl;

		m_pos.x = xp;
		m_pos.z = zp;

		m_patchWidth = patchWidth;
		m_patchDepth = patchDepth;

		m_patchWidthDiv = patchWidthDiv;
		m_patchDepthDiv = patchDepthDiv;

		m_generator = pGenerator;

		// create the buffers according to the dimensions required
		// Every subgrid has a total of 6 vertices ( repeating vertices for normals )

		m_terrainVertices = vector<LVec3>( 6 * m_patchWidthDiv * m_patchDepthDiv );
		m_terrainNormals = vector<LVec3>( 6 * m_patchWidthDiv * m_patchDepthDiv );
		m_terrainTexCoords = vector<LVec2>( 6 * m_patchWidthDiv * m_patchDepthDiv );

		m_terrainVertBuff = new LVertexBuffer();
		m_terrainNormBuff = new LVertexBuffer();
		m_terrainTexBuff  = new LVertexBuffer();

		m_terrainVao = new LVertexArray();

		resample( m_pos.x, m_pos.z );

		m_terrainVao->addBuffer( m_terrainVertBuff, 0 );
		m_terrainVao->addBuffer( m_terrainNormBuff, 1 );
		m_terrainVao->addBuffer( m_terrainTexBuff, 2 );

		m_isWireframe = true;
	}

	LTerrainPatch::~LTerrainPatch()
	{
		m_terrainVertBuff = NULL;
		m_terrainNormBuff = NULL;
		m_terrainTexBuff = NULL;

		delete m_terrainVao;

		m_terrainVertices.clear();
		m_terrainNormals.clear();
		m_terrainTexCoords.clear();
	}

	void LTerrainPatch::resample( float patchX, float patchZ )
	{
		m_pos.x = patchX;
		m_pos.z = patchZ;

		m_terrainVariation = rand() % TERRAIN_REGION2_VARIATIONS; // choose one of 4 possible variations

		// Create traingle grid again

		float _dw = m_patchWidth / m_patchWidthDiv;
		float _dd = m_patchDepth / m_patchDepthDiv;

		// vector<LInd2> _cMask;
		// _cMask.push_back( LInd2( -1, -1 ) ); 
		// _cMask.push_back( LInd2(  0,  0 ) );
		// _cMask.push_back( LInd2( -1,  0 ) ); 

		// _cMask.push_back( LInd2( -1,  0 ) ); 
		// _cMask.push_back( LInd2(  0,  0 ) ); 
		// _cMask.push_back( LInd2(  0,  1 ) );

		// _cMask.push_back( LInd2(  0,  0 ) ); 
		// _cMask.push_back( LInd2(  1,  1 ) ); 
		// _cMask.push_back( LInd2(  0,  1 ) );

		// _cMask.push_back( LInd2(  0,  0 ) ); 
		// _cMask.push_back( LInd2(  1,  0 ) ); 
		// _cMask.push_back( LInd2(  1,  1 ) );

		// _cMask.push_back( LInd2(  0, -1 ) ); 
		// _cMask.push_back( LInd2(  1,  0 ) ); 
		// _cMask.push_back( LInd2(  0,  0 ) );

		// _cMask.push_back( LInd2( -1, -1 ) ); 
		// _cMask.push_back( LInd2(  0, -1 ) ); 
		// _cMask.push_back( LInd2(  0,  0 ) );

		// const int _nTriShared = 6;
		// const int _nVertexInQuad = 6;

		for ( int i = 0; i < m_patchDepthDiv; i++ )
		{
			for ( int j = 0; j < m_patchWidthDiv; j++ )
			{
				int _indx = j + i * m_patchWidthDiv;

				// Fill the vertices, normal

				float _xg = m_pos.x + j * _dw - 0.5 * m_patchWidth;
				float _zg = m_pos.z + i * _dd - 0.5 * m_patchDepth;

				LVec3 _p0( _xg +   0, 10.0 * _getHeight( _xg +   0, _zg +   0 ), _zg +   0 );
				LVec3 _p1( _xg +   0, 10.0 * _getHeight( _xg +   0, _zg + _dd ), _zg + _dd );
				LVec3 _p2( _xg + _dw, 10.0 * _getHeight( _xg + _dw, _zg + _dd ), _zg + _dd );
				LVec3 _p3( _xg + _dw, 10.0 * _getHeight( _xg + _dw, _zg +   0 ), _zg +   0 );

				LVec2 _t0( 0, 0 );
				LVec2 _t1( 0, 1 );
				LVec2 _t2( 1, 1 );
				LVec2 _t3( 1, 0 );

				m_terrainVertices[ 6 * _indx + 0 ] = _p0;
				m_terrainVertices[ 6 * _indx + 1 ] = _p1;
				m_terrainVertices[ 6 * _indx + 2 ] = _p2;
				m_terrainVertices[ 6 * _indx + 3 ] = _p0;
				m_terrainVertices[ 6 * _indx + 4 ] = _p2;
				m_terrainVertices[ 6 * _indx + 5 ] = _p3;

				m_terrainTexCoords[ 6 * _indx + 0 ] = _t0;
				m_terrainTexCoords[ 6 * _indx + 1 ] = _t1;
				m_terrainTexCoords[ 6 * _indx + 2 ] = _t2;
				m_terrainTexCoords[ 6 * _indx + 3 ] = _t0;
				m_terrainTexCoords[ 6 * _indx + 4 ] = _t2;
				m_terrainTexCoords[ 6 * _indx + 5 ] = _t3;

				// for ( int q = 0; q < _nVertexInQuad; q++ )
				// {
				// 	LVec3 _v = m_terrainVertices[ 6 * _indx + q ];

				// 	LVec3 _n;

				// 	for ( int p = 0; p < _nTriShared; p++ )
				// 	{
				// 		LVec3 _vtri[3];

				// 		for ( int s = 0; s < 3; s++ )
				// 		{
				// 			LInd2 _ij = _cMask[ 3 * p + s ];

				// 			float _x = m_pos.x + ( j + _ij.j ) * _dw - 0.5 * m_patchWidth;
				// 			float _z = m_pos.z + ( i + _ij.i ) * _dd - 0.5 * m_patchDepth;

				// 			_vtri[s] = LVec3( _x, 10.0 * _getHeight( _x, _z ), _z );
				// 		}

				// 		_n = _n + LVec3::normalize( LVec3::cross( _vtri[1] - _vtri[0], _vtri[2] - _vtri[0] ) );
				// 	}
				// 	_n.normalize();

				// 	m_terrainNormals[ 6 * _indx + q ] = _n;
				// }

				LVec3 _n0 = LVec3::cross( _p1 - _p0, _p2 - _p1 );
				LVec3 _n1 = LVec3::cross( _p2 - _p0, _p3 - _p2 );

				// if ( _n0.y < 0 ) { std::cout << "????" << std::endl; }
				// if ( _n1.y < 0 ) { std::cout << "????" << std::endl; }

				// if ( _n0.y < 0 ) { _n0.scale( -1, -1, -1 ); }
				// if ( _n1.y < 0 ) { _n1.scale( -1, -1, -1 ); }

				m_terrainNormals[ 6 * _indx + 0 ] = _n0;
				m_terrainNormals[ 6 * _indx + 1 ] = _n0;
				m_terrainNormals[ 6 * _indx + 2 ] = _n0;
				m_terrainNormals[ 6 * _indx + 3 ] = _n1;
				m_terrainNormals[ 6 * _indx + 4 ] = _n1;
				m_terrainNormals[ 6 * _indx + 5 ] = _n1;
			}
		}

		m_terrainVertBuff->setData( sizeof( LVec3 ) * m_terrainVertices.size(),
									3, ( GLfloat* ) m_terrainVertices.data() );
		m_terrainNormBuff->setData( sizeof( LVec3 ) * m_terrainNormals.size(),
									3, ( GLfloat* ) m_terrainNormals.data() );
		m_terrainTexBuff->setData( sizeof( LVec2 ) * m_terrainTexCoords.size(),
								   2, ( GLfloat* ) m_terrainTexCoords.data() );

	}

	void LTerrainPatch::render()
	{
		// if ( m_isWireframe )
		// {
		// 	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		// }

		m_terrainVao->bind();

		glDrawArrays( GL_TRIANGLES, 0, m_terrainVertices.size() );

		m_terrainVao->unbind();

/*	 	if ( m_isWireframe )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}*/
	}


	float LTerrainPatch::_getHeight( float x, float z )
	{
		if ( m_generator == NULL )
		{
			return 0.0f;
		}

		float _val = m_generator->getHeight( x, z );

		return ( _val < 0.0f ) ? 0.0f : _val;
	}

}