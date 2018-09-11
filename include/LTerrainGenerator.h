
#pragma once

#include "LScene.h"
#include "LTerrainPatch.h"
#include "LHeightmapGenerator.h"
#include "LMaterial.h"
#include "LTexture.h"
#include <queue>

#define TERRAIN_INIT_PATCHS_WINDOW 5
#define TERRAIN_PATCH_AREA 10
#define TERRAIN_PATCH_MIN_DIV 10
#define TERRAIN_PATCH_BASE_DIV 40
#define TERRAIN_PATCH_MAX_DIV 40

#define TERRAIN_RANGE_FOR_RECALCULATION 5

using namespace std;

namespace engine
{

    class LScene;

	class LTerrainGenerator
	{
        private :

        vector<LTerrainPatch*> m_terrainPatches;
        LScene* m_scene;

        LHeightmapGenerator* m_heightmapGenerator;

        queue<LTerrainPatch*> m_terrainPatchesPool;

        LVec3 m_lastPosition;

        vector<LTexture*> m_textures;
        vector<LMaterial*> m_materials;

        bool _isTherePatchInPosition( float x, float z );

        public :

		LTerrainGenerator( LScene* pScene );
		~LTerrainGenerator();

		void update( float dt );

        vector<LTerrainPatch*>& getTerrainPatches() { return m_terrainPatches; }
        vector<LMaterial*> getTerrainMaterials() { return m_materials; }
        vector<LTexture*> getTerrainTextures() { return m_textures; }
	};
	
}