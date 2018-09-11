
#pragma once

#include "LCommon.h"
#include "LIRenderable.h"
#include "LVertexBuffer.h"
#include "LVertexArray.h"
#include "LHeightmapGenerator.h"
#include "LMaterial.h"
#include "LTexture.h"

using namespace std;

#define TERRAIN_REGION2_VARIATIONS 4

namespace engine
{


	class LTerrainPatch : public LIRenderable
	{

		protected :

		LVertexBuffer* m_terrainVertBuff;
		LVertexBuffer* m_terrainNormBuff;
		LVertexBuffer* m_terrainTexBuff;
		LVertexArray* m_terrainVao;


		vector<LVec3> m_terrainVertices;
		vector<LVec3> m_terrainNormals;
		vector<LVec2> m_terrainTexCoords;

		GLfloat m_patchWidth;
		GLfloat m_patchDepth;

		int m_patchWidthDiv;
		int m_patchDepthDiv;

		LHeightmapGenerator* m_generator;

		bool m_isWireframe;

		float _getHeight( float x, float z );

		LVec3 m_pos;

		int m_terrainVariation;

		public :

		LTerrainPatch( float xp, float zp,
					   GLfloat patchWidth, GLfloat patchDepth, 
					   int patchWidthDiv, int patchDepthDiv, LHeightmapGenerator* pGenerator = NULL );
		~LTerrainPatch();

		void resample( float patchX, float patchZ );

		void enableWireframe() { m_isWireframe = true; }
		void disableWireframe() { m_isWireframe = false; }
		bool isWireframe() { return m_isWireframe; }

		LVec3 getPosition(){ return m_pos; }

		void render() override;

		int getTerrainVariation() { return m_terrainVariation; }

	};



}