
#pragma once

#include "LMesh.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace engine
{

    struct LMeshBuilder
    {

        private :

        static vector<string> _split( const string &txt, char separator = ';' );

        //static void _computeNormalsSingle();
        //static void _computeNormalsAvg();


        public :

        static LVec3 _computeFaceNormal( LVec3 v1, LVec3 v2, LVec3 v3, bool normalize = false );

        static LMesh* createBox( GLfloat width, GLfloat height, GLfloat depth, 
                                 bool computeSmoothNormals = false,
                                 bool generateTexCoords = false );
        static LMesh* createPlane( GLfloat width, GLfloat depth,
                                   bool generateTexCoords = false );
        static LMesh* createFromFile( const char* filename );

    };



}