

#include <core/LBaseApp.h>
#include <cmath>

#include <vector>
#include <string>
#include <fstream>

using namespace std;

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

struct LVec3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct LInd3
{
    GLuint i1;
    GLuint i2;
    GLuint i3;
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


struct LPolygon
{
    //protected :

    vector<LVec2> m_points;
    vector<LInd3> m_tris;

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

        //cout << "crosA: " << _crossA << endl;
        //cout << "crosB: " << _crossB << endl;
        //cout << "crosC: " << _crossC << endl;

        if ( _crossA > 0 && _crossB > 0 && _crossC > 0 )
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

        cout << "size points: " << m_points.size() << endl;

        while ( _list.nodes.size() > 3 )
        {
            cout << "checking v: " << _list.head->id << endl;
            if ( _isConvex( _list.head ) )
            {
                cout << "isconvex" << endl;
                if ( _isEar( _list.head ) )
                {
                    LInd3 _tri;
                    _tri.i1 = _list.head->prev->id;
                    _tri.i2 = _list.head->id;
                    _tri.i3 = _list.head->next->id;

                    m_tris.push_back( _tri );
                    cout << "added tri" << endl;
                    cout << "i1: " << _tri.i1 << " i2: " << _tri.i2 << " i3: " << _tri.i3 << endl;
                    LNode* _next = _list.head->next;
                    _list.remove( _list.head );
                    cout << "size: " << _list.nodes.size() << endl;
                    _list.head = _next;
                }

            }

            _list.head = _list.head->next;

        }

        // add remaining triangle
        LInd3 _tri;
        _tri.i1 = _list.head->prev->id;
        _tri.i2 = _list.head->id;
        _tri.i3 = _list.head->next->id;

        m_tris.push_back( _tri );

        // dump tris
        cout << "dumping tris" << endl;
        for ( int q = 0; q < m_tris.size(); q++ )
        {
            LInd3 _tri = m_tris[q];
            cout << "i1: " << _tri.i1 << " i2: " << _tri.i2 << " i3: " << _tri.i3 << endl;
        }
        cout << "dumping tris - DONE" << endl;

        cout << "ntris: " << m_tris.size() << endl;
        cout << "remaining v: " << _list.nodes.size() << endl;
    }

    void reset()
    {
        m_points.clear();
        m_tris.clear();
    }

    void render()
    {

        glBegin( GL_LINES );

        for ( int q = 0; q < m_points.size(); q++ )
        {
            int _i = q;
            int _j = ( q + 1 ) % m_points.size();

            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( m_points[_i].x, m_points[_i].y, 0.0f );
            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( m_points[_j].x, m_points[_j].y, 0.0f );
        }

        glEnd();

        glBegin( GL_TRIANGLES );

        for ( int q = 0; q < m_tris.size(); q++ )
        {
            LInd3 _tri = m_tris[q];

            glColor3f( 1.0f, 1.0f, 1.0f );
            glVertex3f( m_points[_tri.i1].x,
                        m_points[_tri.i1].y, 0.0f );

            glColor3f( 1.0f, 1.0f, 1.0f );
            glVertex3f( m_points[_tri.i2].x,
                        m_points[_tri.i2].y, 0.0f );

            glColor3f( 1.0f, 1.0f, 1.0f );
            glVertex3f( m_points[_tri.i3].x,
                        m_points[_tri.i3].y, 0.0f );
        }

        glEnd();
    }

    vector<LVec2> points()
    {
        return m_points;
    }


    void d_test()
    {
        glBegin( GL_TRIANGLES );

        glVertex3f(  0.0f,  0.5f, 0.0f );
        glVertex3f( -0.5f, -0.5f, 0.0f );
        glVertex3f(  0.5f, -0.5f, 0.0f );

        glEnd();
    }

    void d_test_funct( LVec2 v )
    {
        //cout << "v.x " << v.x << " - " << v.y << endl;
        if ( _isInTriangle( v, 
                            LVec2(  0.0f,  0.5f ),
                            LVec2( -0.5f, -0.5f ),
                            LVec2(  0.5f, -0.5f ) ) )
        {
            cout << "inside triangle :D" << endl;
        }
        else
        {
            cout << "not inside triangle :(" << endl;
        }
    }

};

class Hw1App : public engine::core::LBaseApp
{
    private :

    LPolygon m_path;

    vector<string> split( const string &txt )
    {
        vector<string> _res;
                
        int pos = txt.find( ',' );
        if ( pos == std::string::npos )
        {
            _res.push_back( txt );
            return _res;
        }

        int initpos = 0;

        while ( pos != std::string::npos )
        {
            _res.push_back( txt.substr( initpos, pos - initpos + 1 ) );
            initpos = pos + 1;

            pos = txt.find( ',', initpos );
        }

        _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos + 1 ) );
                
        return _res;
    }

    protected :

    Hw1App() : engine::core::LBaseApp()
    {

    }

    public :

    static void create()
    {
        if ( Hw1App::instance != NULL )
        {
            delete Hw1App::instance;
        }

        Hw1App::instance = new Hw1App();
        Hw1App::instance->init();
    }

    void onStartup() override
    {
        ENGINE_LOG( "Hw1App::onStartup> initializing some custom stuff" );

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    void render() override
    {

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        m_path.render();

        //m_path.d_test();

    }

    void onMouseButtonCallback( int pButton, 
                                int pAction, 
                                int pMods ) override
    {
        if ( pAction != GLFW_PRESS )
        {
            return;
        }

        if ( pButton == GLFW_MOUSE_BUTTON_2 )
        {
            m_path.reset();
            return;
        }
        else if ( pButton == GLFW_MOUSE_BUTTON_3 )
        {
            cout << "earClipTriangulate"<< endl;
            m_path.earClipTriangulate();
            return;
        }

        double evx, evy;

        glfwGetCursorPos( m_window, &evx, &evy );

        LVec2 _v;

        _v.x = 2 * ( evx / ENGINE_APP_WIDTH - 0.5 );
        _v.y = 2 * ( 0.5 - evy / ENGINE_APP_HEIGHT );
    
        //m_path.d_test_funct( _v );        
        m_path.addPoint( _v );
    }

};




int main()
{

    Hw1App::create();
    Hw1App::instance->loop();
    Hw1App::destroy();

    return 0;
}

