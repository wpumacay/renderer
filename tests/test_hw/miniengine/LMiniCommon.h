
#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#define USE_MODERN_OPENGL 1

using namespace std;

namespace miniengine
{

    template<class T>
    struct LVec3T
    {
        T x;
        T y;
        T z;

        LVec3T()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;        
        }

        LVec3T( T x, T y, T z )
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        LVec3T<T> operator+( const LVec3T<T>& other )
        {
            return LVec3T<T>( this->x + other.x,
                              this->y + other.y,
                              this->z + other.z );
        }

        LVec3T<T> operator-( const LVec3T<T>& other )
        {
            return LVec3T<T>( this->x - other.x,
                              this->y - other.y,
                              this->z - other.z );
        }

        void normalize()
        {
            T _len = sqrt( x * x + y * y + z * z );
            x = x / _len;
            y = y / _len;
            z = z / _len;
        }

        LVec3T<T> operator*( float s )
        {
            return LVec3T<T>( this->x * s,
                              this->y * s,
                              this->z * s );
        }

        void scale( float sx, float sy, float sz )
        {
            x *= sx; y *= sy; z *= sz;
        }

        static LVec3T<T> cross( const LVec3T<T>& v1,
                                const LVec3T<T>& v2 )
        {
            LVec3T<T> _res;

            _res.x = v1.y * v2.z - v2.y * v1.z;
            _res.y = -v1.x * v2.z + v2.x * v1.z;
            _res.z = v1.x * v2.y - v2.x * v1.y;

            return _res;
        }

    };

    template<class T>
    struct LTriT
    {
        T i1;
        T i2;
        T i3;
    };

    template<class T>
    union LInd3T
    {
        T buff[3];
        LTriT<T> tri;

        LInd3T()
        {

        }

        LInd3T( T v1, T v2, T v3 )
        {
            buff[0] = v1;
            buff[1] = v2;
            buff[2] = v3;
        }

        void dump()
        {
            cout << "v1: " << buff[0] << " - v2: " << buff[1] << " - v3: " << buff[2] << endl;
        }
    };


    typedef LVec3T<GLfloat> LVec3;
    typedef LInd3T<GLuint> LInd3;

    struct LLightProps
    {
        LVec3 ambient;
        LVec3 diffuse;
        LVec3 specular;

        LVec3 pos;
        LVec3 dir;
    };

    struct LRenderInfo
    {
        glm::mat4 viewMat;
        glm::mat4 projMat;

        vector<LLightProps> lights;
    };

    // Stuff used for ear clipping triangulation

    struct LTri
    {
        GLuint i1;
        GLuint i2;
        GLuint i3;
    };

    struct LVec2
    {

        GLfloat x;
        GLfloat y;

        LVec2()
        {
            x = 0.0f;
            y = 0.0f;
        }

        LVec2( GLfloat x, GLfloat y )
        {
            this->x = x;
            this->y = y;
        }

        LVec2 operator+( const LVec2& other )
        {
            return LVec2( this->x + other.x,
                          this->y + other.y );
        }

        LVec2 operator-( const LVec2& other )
        {
            return LVec2( this->x - other.x,
                          this->y - other.y );
        }
    };

    struct LNode
    {

        LVec2 data;

        LNode* prev;
        LNode* next;

        GLint id;

        LNode()
        {
            prev = NULL;
            next = NULL;
            id = -1;
        }

    };


    struct LDoubleLinkList
    {
        LNode* head;
        vector<LNode*> nodes;

        LDoubleLinkList( vector<LVec2> pathPoints )
        {
            cout << "making llist" << endl;
            // make nodes
            vector<LNode*> _vNodes;

            for ( int q = 0; q < pathPoints.size(); q++ )
            {
                LNode* _node = new LNode;
                _node->data = pathPoints[q];
                _node->id = q;

                _vNodes.push_back( _node );
            }

            head = _vNodes[0];

            for ( int q = 0; q < _vNodes.size(); q++ )
            {
                //cout << "_vNodes.size(): " << _vNodes.size() << endl;

                int _i_next = ( q + 1 ) % _vNodes.size();
                int _i_prev = ( q - 1 );

                if ( _i_prev < 0 )
                {
                    _i_prev += _vNodes.size();
                }
                else
                {
                    _i_prev %= _vNodes.size();
                }

                //cout << "_i_next: " << _i_next << endl;
                //cout << "_i_prev: " << _i_prev << endl;
                //cout << "_vNodes.size(): " << _vNodes.size() << endl;
                //while( 1 );

                _vNodes[q]->next = _vNodes[_i_next];
                _vNodes[q]->prev = _vNodes[_i_prev];
            }

            nodes = _vNodes;

            cout << "making llist - DONE" << endl;
        }

        void remove( LNode* pNode )
        {
            int _indx = -1;

            for ( int q = 0; q < nodes.size(); q++ )
            {
                if ( nodes[q] == pNode )
                {
                    _indx = q;
                }
            }

            if ( _indx != -1 )
            {
                nodes.erase( nodes.begin() + _indx, nodes.begin() + _indx + 1 );
            }
                
            LNode* _prev = pNode->prev;
            LNode* _next = pNode->next;

            _prev->next = _next;
            _next->prev = _prev;

            delete pNode;
        }

    };

    class LPolygon
    {
        protected :

        vector<LVec2> m_points;
        vector<LTri> m_tris;

        GLfloat _cross2d( LVec2 v1, LVec2 v2 )
        {
            return v1.x * v2.y - v2.x * v1.y;
        }

        bool _isConvex( LNode* pNode )
        {
            LVec2 _a = pNode->prev->data;
            LVec2 _b = pNode->data;
            LVec2 _c = pNode->next->data;

            LVec2 _ba = _a - _b;
            LVec2 _bc = _c - _b;

            GLfloat _cross = _cross2d( _bc, _ba );

            return _cross > 0;
        }

        bool _isInTriangle( LVec2 p, LVec2 a, LVec2 b, LVec2 c )
        {
            LVec2 _ab = b - a;
            LVec2 _bc = c - b;
            LVec2 _ca = a - c;

            LVec2 _ap = p - a;
            LVec2 _bp = p - b;
            LVec2 _cp = p - c;

            float _crossA = _cross2d( _ab, _ap );
            float _crossB = _cross2d( _bc, _bp );
            float _crossC = _cross2d( _ca, _cp );

            if ( _crossA >= 0 && _crossB >= 0 && _crossC >= 0 )
            {
                return true;
            }

            return false;
        }

        bool _isEar( LNode* pNode )
        {
            LNode* _checker = pNode->next->next;

            while ( _checker != pNode->prev )
            {
                if ( _isInTriangle( _checker->data, 
                                    pNode->prev->data, 
                                    pNode->data,
                                    pNode->next->data ) )
                {
                    return false;
                }

                _checker = _checker->next;
            }

            return true;
        }

        public :


        LPolygon()
        {

        }


        void addPoint( LVec2 pPoint )
        {
            m_points.push_back( pPoint );
        }

        void earClipTriangulate()
        {
            m_tris.clear();

            LDoubleLinkList _list( m_points );

            //cout << "size points: " << m_points.size() << endl;

            while ( _list.nodes.size() > 3 )
            {
                //cout << "checking v: " << _list.head->id << endl;
                if ( _isConvex( _list.head ) )
                {
                    //cout << "isconvex" << endl;
                    if ( _isEar( _list.head ) )
                    {
                        LTri _tri;
                        _tri.i1 = _list.head->prev->id;
                        _tri.i2 = _list.head->id;
                        _tri.i3 = _list.head->next->id;

                        m_tris.push_back( _tri );
                        //cout << "added tri" << endl;
                        //cout << "i1: " << _tri.i1 << " i2: " << _tri.i2 << " i3: " << _tri.i3 << endl;
                        LNode* _next = _list.head->next;
                        _list.remove( _list.head );
                        //cout << "size: " << _list.nodes.size() << endl;
                        _list.head = _next;
                    }

                }

                _list.head = _list.head->next;

            }

            // add remaining triangle
            LTri _tri;
            _tri.i1 = _list.head->prev->id;
            _tri.i2 = _list.head->id;
            _tri.i3 = _list.head->next->id;

            m_tris.push_back( _tri );
        }

        vector<LVec2> points()
        {
            return m_points;
        }

        vector<LTri> tris()
        {
            return m_tris;
        }

    };

}