
#pragma once

#include "LCommon.h"

#include "LMesh.h"

#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define OBJ_COMMENT string( "#" )
#define OBJ_VERTEX string( "v" )
#define OBJ_NORMAL string( "vn" )
#define OBJ_TEXTURE string( "vt" )
#define OBJ_FACE string( "f" )
#define OBJ_MATERIAL_LIB string( "mtllib" )
#define OBJ_OBJECT_NAME string( "o" )
#define OBJ_GROUP_NAME string( "g" )
#define OBJ_MATERIAL_ID string( "usemtl" )
#define OBJ_SMOOTH_SHADING string( "s" )


namespace engine
{

    struct LObjInfo
    {
        vector<LVec3> vertices;
        vector<LVec3> normals;
        vector<LVec2> texCoords;
        vector<LInd3> indices;

        string materialFile;
    };

    struct LObjGeometryInfo
    {
        vector<LVec3> vertices;
        vector<LVec3> normals;
        vector<LVec2> texCoords;
        vector<LInd3> indices;        


    };

    struct LMeshBuilder
    {

        private :

        static vector<string> _split( const string &txt, char separator = ';' );

        //static void _computeNormalsSingle();
        //static void _computeNormalsAvg();

        static void _parseObj( ifstream& fileHandle,
                               LObjInfo& obj );




        public :

        static LVec3 _computeFaceNormal( LVec3 v1, LVec3 v2, LVec3 v3, bool normalize = false );

        static LMesh* createSphere( GLfloat radius, int levelDivision = 10, int numLevels = 10 );
        static LMesh* createBox( GLfloat width, GLfloat height, GLfloat depth );
        static LMesh* createCylinder( GLfloat radius, GLfloat height, int sectionDivision = 10 );
        static LMesh* createCapsule( GLfloat radius, GLfloat height, int sectionDivision = 10, int capLevels = 10 );

        static LMesh* createPlane( GLfloat width, GLfloat depth );
        static LMesh* createFromFile( const char* filename );
        static LMesh* createFromObj( const char* filename );

        static LMesh* createPerlinPatch( GLfloat width, GLfloat depth, int cellDivision );
    };



}