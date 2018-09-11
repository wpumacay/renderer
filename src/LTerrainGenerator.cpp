
#include "LTerrainGenerator.h"
#include "LAssetsManager.h"

using namespace std;


namespace engine
{

    LTerrainGenerator::LTerrainGenerator( LScene* pScene )
    {
        m_scene = pScene;

        m_heightmapGenerator = new LHeightmapGenerator();

        string _textureNames[] = { "water", /* region 0 */
                                   "grass_sandy", /* region 1 */
                                   "grass", /* region 2 variation 0 */
                                   "ground_stony", /* region 2 variation 1 */
                                   "rocks_1", /* region 2 variation 2 */
                                   "rocks_2", /* region 2 variation 3 */
                                   "snow" };/* region 3 */

        for ( int q = 0; q < 7; q++ )
        {
            auto _textureData = LAssetsManager::INSTANCE->texturesData[ _textureNames[ q ] ];
            auto _texture = new LTexture();
            _texture->setData( ( u8* ) _textureData->data,
                               GL_RGB, 
                               _textureData->width, _textureData->height, q );

            m_textures.push_back( _texture );
        }

        LVec3 _vWhite( 1.0f, 1.0f, 1.0f );
        
        for ( int q = 0; q < 7; q++ )
        {
            m_materials.push_back( new LMaterial( _vWhite, _vWhite, _vWhite,
                                                  120.0f ) );
        }


        for ( int i = -TERRAIN_INIT_PATCHS_WINDOW; i <= TERRAIN_INIT_PATCHS_WINDOW; i++ )
        {
            for ( int j = -TERRAIN_INIT_PATCHS_WINDOW; j <= TERRAIN_INIT_PATCHS_WINDOW; j++ )
            {
                float _xp = j * TERRAIN_PATCH_AREA;
                float _zp = i * TERRAIN_PATCH_AREA;

                auto _terrainPatch = new LTerrainPatch( _xp, _zp,
                                                        TERRAIN_PATCH_AREA, TERRAIN_PATCH_AREA, 
                                                        TERRAIN_PATCH_BASE_DIV, TERRAIN_PATCH_BASE_DIV,
                                                        m_heightmapGenerator );

                m_terrainPatches.push_back( _terrainPatch );
            }
        }

        m_lastPosition = LVec3( 0.0f, 0.0f, 0.0f );
    }

    LTerrainGenerator::~LTerrainGenerator()
    {
        m_scene = NULL;

        for ( LTerrainPatch* _terrainPatch : m_terrainPatches )
        {
            delete _terrainPatch;
        }
        m_terrainPatches.clear();

        while ( !m_terrainPatchesPool.empty() )
        {
            auto _terrainPatch = m_terrainPatchesPool.front();
            delete _terrainPatch;
            m_terrainPatchesPool.pop();
        }

        if ( m_heightmapGenerator != NULL )
        {
            delete m_heightmapGenerator;
            m_heightmapGenerator = NULL;
        }
    }

    bool LTerrainGenerator::_isTherePatchInPosition( float x, float z )
    {
        // Naive checking, just check all the positions of the current patches 
        // in the terrainpatches vector

        for ( LTerrainPatch* _terrainPatch : m_terrainPatches )
        {
            float _dx = x - _terrainPatch->getPosition().x;
            float _dz = z - _terrainPatch->getPosition().z;

            float _dist = sqrt( _dx * _dx + _dz * _dz );

            if ( _dist < 0.1 )
            {
                return true;
            }
        }

        return false;
    }

    void LTerrainGenerator::update( float dt )
    {
        // std::cout << "size ( terrainpatches, pool ): "
        //           << "( " << m_terrainPatches.size() << ", " << m_terrainPatchesPool.size() << " )" << std::endl;

        // return;

        // Get the camera position
        auto _camera = m_scene->getCurrentCamera();
        if ( _camera == NULL )
        {
            return;
        }
        auto _cpos = _camera->getPosition();

        float _cx = _cpos.x;
        float _cz = _cpos.z;

        float _dist = sqrt( ( _cx - m_lastPosition.x ) * ( _cx - m_lastPosition.x ) + 
                            ( _cz - m_lastPosition.z ) * ( _cz - m_lastPosition.z ) );

        if ( _dist < TERRAIN_RANGE_FOR_RECALCULATION )
        {
            return;
        }

        /* 
        * outside of bounds, need to recalculate :D
        */

        // calculate the center tile where we currently are and ...
        // some other necessary stuff

        float _dx = TERRAIN_PATCH_AREA;
        float _dz = TERRAIN_PATCH_AREA;

        // calculate current center tile indx
        int _xi = floor( _cx / _dx + 0.5 ); int _zi = floor( _cz / _dz + 0.5 );

        // and also the world coordinates of that point
        float _xCenter = _xi * _dx; float _zCenter = _zi * _dz;

        // calculate the bounds of the new visible area
        float _xmin = _xCenter - ( TERRAIN_INIT_PATCHS_WINDOW + 0.5 ) * TERRAIN_PATCH_AREA;
        float _xmax = _xCenter + ( TERRAIN_INIT_PATCHS_WINDOW + 0.5 ) * TERRAIN_PATCH_AREA;
        float _zmin = _zCenter - ( TERRAIN_INIT_PATCHS_WINDOW + 0.5 ) * TERRAIN_PATCH_AREA;
        float _zmax = _zCenter + ( TERRAIN_INIT_PATCHS_WINDOW + 0.5 ) * TERRAIN_PATCH_AREA;

        // recycle the patches that are no longer necessary

        vector<LTerrainPatch*> _keepPatches;

        for ( LTerrainPatch* _patch : m_terrainPatches )
        {
            float _px = _patch->getPosition().x;
            float _pz = _patch->getPosition().z;

            if ( _px < _xmin || _px > _xmax ||
                 _pz < _zmin || _pz > _zmax )
            {
                // recycle this patch
                m_terrainPatchesPool.push( _patch );
            }
            else
            {
                // keep this patch
                _keepPatches.push_back( _patch );
            }
        }

        m_terrainPatches.clear();
        m_terrainPatches = _keepPatches;

        // craete new necessary patches

        for ( int i = -TERRAIN_INIT_PATCHS_WINDOW; i <= TERRAIN_INIT_PATCHS_WINDOW; i++ )
        {
            for ( int j = -TERRAIN_INIT_PATCHS_WINDOW; j <= TERRAIN_INIT_PATCHS_WINDOW; j++ )
            {
                float _xp = j * TERRAIN_PATCH_AREA + _xCenter;
                float _zp = i * TERRAIN_PATCH_AREA + _zCenter;

                if ( !_isTherePatchInPosition( _xp, _zp ) )
                {
                    // Sample a new terrain here, from the pool or a new one
                    if ( m_terrainPatchesPool.empty() )
                    {
                        // create a new one
                        auto _terrainPatch = new LTerrainPatch( _xp, _zp,
                                                                TERRAIN_PATCH_AREA, TERRAIN_PATCH_AREA, 
                                                                TERRAIN_PATCH_BASE_DIV, TERRAIN_PATCH_BASE_DIV,
                                                                m_heightmapGenerator );

                        m_terrainPatches.push_back( _terrainPatch );
                    }
                    else
                    {
                        auto _terrainPatch = m_terrainPatchesPool.front();
                        _terrainPatch->resample( _xp, _zp );
                        m_terrainPatches.push_back( _terrainPatch );
                        m_terrainPatchesPool.pop();
                    }
                }

                
            }
        }

        m_lastPosition.x = _cx;
        m_lastPosition.z = _cz;
    }

}