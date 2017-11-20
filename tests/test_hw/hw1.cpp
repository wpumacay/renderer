

#include <core/LBaseApp.h>
#include <cmath>

#include <vector>
#include <string>
#include <fstream>

using namespace std;

#define NUMVTX 18

GLfloat g_mat_specular[] = { 1.0, 1.0, 1.0 };
GLfloat g_mat_shininess = 20.0f;
GLfloat g_mat_emmision[] = { 0.0f, 0.0f, 0.0f };

GLfloat g_ambientLight[] = { 0.2f, 0.2f, 0.2f };
GLfloat g_lightColor[] = { 1.0f, 1.0f, 1.0f };

template<class T>
struct Hw1Vec3T
{
    T x;
    T y;
    T z;

    Hw1Vec3T()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;        
    }

    Hw1Vec3T( T x, T y, T z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

typedef Hw1Vec3T<GLfloat> Hw1Vec3;
typedef Hw1Vec3T<GLuint> Hw1Tri;

struct Hw1TriFace
{
    Hw1Tri verts;

    Hw1Vec3 n1;
    Hw1Vec3 n2;
    Hw1Vec3 n3;

    bool contains( GLuint v )
    {
        if ( v != verts.x && v != verts.y && v != verts.z )
        {
            return false;
        }

        return true;
    }
};

void normalize( Hw1Vec3& vect )
{
    GLfloat _len = sqrt( vect.x * vect.x + 
                         vect.y * vect.y +
                         vect.z * vect.z );

    vect.x = vect.x / _len;
    vect.y = vect.y / _len;
    vect.z = vect.z / _len;
}



class Hw1Cube
{
    protected :

    GLfloat m_l;
    vector<Hw1Vec3> m_cubeVertices;
    vector<Hw1Tri> m_tris;

    vector<Hw1TriFace> m_triFaces;

    Hw1Vec3 m_color;
    GLfloat m_colorArr[4];

    Hw1Vec3 computeFaceNormal( const Hw1Tri& pFaceIDs )
    {
        Hw1Vec3 _normal;

        Hw1Vec3 _a = m_cubeVertices[pFaceIDs.x];
        Hw1Vec3 _b = m_cubeVertices[pFaceIDs.y];
        Hw1Vec3 _c = m_cubeVertices[pFaceIDs.z];

        Hw1Vec3 _ab = Hw1Vec3( _b.x - _a.x, _b.y - _a.y, _b.z - _a.z );
        Hw1Vec3 _ac = Hw1Vec3( _c.x - _a.x, _c.y - _a.y, _c.z - _a.z );

        _normal.x = _ab.y * _ac.z - _ac.y * _ab.z;
        _normal.y = -_ab.x * _ac.z + _ac.x * _ab.z;
        _normal.z = _ab.x * _ac.y - _ac.x * _ab.y;

        normalize( _normal );

        return _normal;
    }

    void computeNormalsNaive()
    {
        // Just assign each vertex normal ...
        // the same normal of its face

        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            Hw1Vec3 _n = computeFaceNormal( m_triFaces[q].verts );
            m_triFaces[q].n1 = _n;
            m_triFaces[q].n2 = _n;
            m_triFaces[q].n3 = _n;
        }
    }

    Hw1Vec3 computeAvgNormalForVertex( GLuint vId )
    {
        Hw1Vec3 _n;
        int _count = 0;
        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            if ( m_triFaces[q].contains( vId ) )
            {
                _count++;
                Hw1Vec3 _np = computeFaceNormal( m_triFaces[q].verts );

                _n.x += _np.x;
                _n.y += _np.y;
                _n.z += _np.z;
            }
        }
        _n.x = _n.x / _count;
        _n.y = _n.y / _count;
        _n.z = _n.z / _count;

        normalize( _n );

        return _n;
    }

    void computeNormalAvg()
    {
        // Assign each vertex normal as the ...
        // average of the normals to the faces it belongs to

        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            // Naive check, check every vertex in the face
            m_triFaces[q].n1 = computeAvgNormalForVertex( m_triFaces[q].verts.x );
            m_triFaces[q].n2 = computeAvgNormalForVertex( m_triFaces[q].verts.y );
            m_triFaces[q].n3 = computeAvgNormalForVertex( m_triFaces[q].verts.z );
        }
    }

    void computeTriFaces()
    {
        for ( int q = 0; q < m_tris.size(); q++ )
        {
            Hw1TriFace _face;
            _face.verts.x = m_tris[q].x;
            _face.verts.y = m_tris[q].y;
            _face.verts.z = m_tris[q].z;

            m_triFaces.push_back( _face );
        }
    }

    public :

    Hw1Vec3 scale;
    Hw1Vec3 rot;
    Hw1Vec3 pos;

    Hw1Cube()
    {
        m_l = 1.0f;

        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;

        m_cubeVertices.push_back( Hw1Vec3( -m_l,  m_l, m_l ) );// 0
        m_cubeVertices.push_back( Hw1Vec3( -m_l, -m_l, m_l ) );// 1
        m_cubeVertices.push_back( Hw1Vec3(  m_l, -m_l, m_l ) );// 2
        m_cubeVertices.push_back( Hw1Vec3(  m_l,  m_l, m_l ) );// 3

        m_cubeVertices.push_back( Hw1Vec3( -m_l,  m_l, -m_l ) );// 4
        m_cubeVertices.push_back( Hw1Vec3( -m_l, -m_l, -m_l ) );// 5
        m_cubeVertices.push_back( Hw1Vec3(  m_l, -m_l, -m_l ) );// 6
        m_cubeVertices.push_back( Hw1Vec3(  m_l,  m_l, -m_l ) );// 7

        m_tris.push_back( Hw1Tri( 0, 1, 2 ) ); m_tris.push_back( Hw1Tri( 2, 3, 0 ) );// face 1
        m_tris.push_back( Hw1Tri( 0, 1, 5 ) ); m_tris.push_back( Hw1Tri( 5, 4, 0 ) );// face 2
        m_tris.push_back( Hw1Tri( 4, 5, 6 ) ); m_tris.push_back( Hw1Tri( 6, 7, 4 ) );// face 3
        m_tris.push_back( Hw1Tri( 3, 2, 6 ) ); m_tris.push_back( Hw1Tri( 6, 7, 3 ) );// face 4
        m_tris.push_back( Hw1Tri( 4, 0, 3 ) ); m_tris.push_back( Hw1Tri( 3, 7, 4 ) );// face 5
        m_tris.push_back( Hw1Tri( 5, 1, 2 ) ); m_tris.push_back( Hw1Tri( 2, 6, 5 ) );// face 6

        computeTriFaces();

        //computeNormalAvg();
        computeNormalsNaive();
    }

    void setColor( GLfloat r, GLfloat g, GLfloat b )
    {
        m_color.x = r;
        m_color.y = g;
        m_color.z = b;

        m_colorArr[0] = r;
        m_colorArr[1] = g;
        m_colorArr[2] = b;
        m_colorArr[3] = 1.0f;
    }

    void render()
    {
        glPushMatrix();

        glTranslatef( this->pos.x, this->pos.y, this->pos.z );
        glRotatef( this->rot.x, 1.0f, 0.0f, 0.0f );
        glRotatef( this->rot.y, 0.0f, 1.0f, 0.0f );
        glRotatef( this->rot.z, 0.0f, 0.0f, 1.0f );
        glScalef( this->scale.x, this->scale.y, this->scale.z );


        glBegin( GL_TRIANGLES );

        //glColor3f( m_color.x, m_color.y, m_color.z );
        //glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_colorArr );
        glMaterialfv( GL_FRONT, GL_AMBIENT, m_colorArr );
        glMaterialfv( GL_FRONT, GL_DIFFUSE, m_colorArr );
        glMaterialfv( GL_FRONT, GL_SPECULAR, g_mat_specular );
        //glMaterialfv( GL_FRONT, GL_EMISSION, g_mat_emmision );
        glMaterialf( GL_FRONT, GL_SHININESS, g_mat_shininess );

        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            glNormal3f( m_triFaces[q].n1.x,
                        m_triFaces[q].n1.y,
                        m_triFaces[q].n1.z );
            glVertex3f( m_cubeVertices[m_triFaces[q].verts.x].x,
                        m_cubeVertices[m_triFaces[q].verts.x].y,
                        m_cubeVertices[m_triFaces[q].verts.x].z );
            glNormal3f( m_triFaces[q].n2.x,
                        m_triFaces[q].n2.y,
                        m_triFaces[q].n2.z );
            glVertex3f( m_cubeVertices[m_triFaces[q].verts.y].x,
                        m_cubeVertices[m_triFaces[q].verts.y].y,
                        m_cubeVertices[m_triFaces[q].verts.y].z );
            glNormal3f( m_triFaces[q].n3.x,
                        m_triFaces[q].n3.y,
                        m_triFaces[q].n3.z );
            glVertex3f( m_cubeVertices[m_triFaces[q].verts.z].x,
                        m_cubeVertices[m_triFaces[q].verts.z].y,
                        m_cubeVertices[m_triFaces[q].verts.z].z );
        }

        glEnd();

        glPopMatrix();
    }

    void dump()
    {
        cout << "showing normals" << endl;
        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            cout << "n1.x: " << m_triFaces[q].n1.x << " - "
                 << "n1.y: " << m_triFaces[q].n1.y << " - "
                 << "n1.z: " << m_triFaces[q].n1.z << endl;
            cout << "n2.x: " << m_triFaces[q].n2.x << " - "
                 << "n2.y: " << m_triFaces[q].n2.y << " - "
                 << "n2.z: " << m_triFaces[q].n2.z << endl;
            cout << "n3.x: " << m_triFaces[q].n3.x << " - "
                 << "n3.y: " << m_triFaces[q].n3.y << " - "
                 << "n3.z: " << m_triFaces[q].n3.z << endl;
        }
        cout << "done" << endl;
    }

};

enum LightSourceType
{
    LIGHT_TYPE_AMBIENT,
    LIGHT_TYPE_DIFFUSE,
    LIGHT_TYPE_SPECULAR
};

class Hw1LightSource
{

    private :

    LightSourceType m_type;

    Hw1Cube m_indicator;

    Hw1Vec3 m_lightPos;
    GLfloat m_lightPosArr[4];

    GLuint m_lightId;

    public :

    Hw1LightSource()
    {
        m_type = LIGHT_TYPE_AMBIENT;
        m_indicator.setColor( 1.0f, 1.0f, 1.0f );
        m_indicator.scale.x = 0.25f;
        m_indicator.scale.y = 0.25f;
        m_indicator.scale.z = 0.25f;

        m_lightId = 0;
    }

    void setLightType( LightSourceType pType )
    {
        m_type = pType;
    }

    void setLightId( GLuint id )
    {
        m_lightId = id;
    }

    void setLightPosition( GLfloat x, GLfloat y, GLfloat z )
    {
        m_lightPos.x = x; 
        m_lightPos.y = y; 
        m_lightPos.z = z;
        m_lightPosArr[0] = x;
        m_lightPosArr[1] = y;
        m_lightPosArr[2] = z;
        m_lightPosArr[3] = 0.0;
        m_indicator.pos.x = x;
        m_indicator.pos.y = y;
        m_indicator.pos.z = z;
        //glLightfv( m_lightId, GL_POSITION, 
        //           reinterpret_cast<GLfloat*>( &m_lightPos ) );
        glLightfv( m_lightId, GL_POSITION, m_lightPosArr );
    }

    void initLighting()
    {
        glEnable( m_lightId );
        glLightfv( m_lightId, GL_DIFFUSE, g_lightColor );
        glLightfv( m_lightId, GL_SPECULAR, g_lightColor );
        //glLightfv( m_lightId, GL_POSITION, 
        //           reinterpret_cast<GLfloat*>( &m_lightPos ) );
        glLightfv( m_lightId, GL_POSITION, m_lightPosArr );
    }


    Hw1Cube& indicator()
    {
        return m_indicator;
    }


};

class Hw1Letter
{

    private :

    vector<Hw1Vec3> m_frontVertices;
    Hw1Vec3 m_color;
    GLfloat m_colorArr[4];
    GLfloat m_depth;

    vector<Hw1Vec3> m_vertices;
    vector<Hw1Tri> m_tris;
    vector<Hw1TriFace> m_triFaces;

    Hw1Vec3 computeFaceNormal( const Hw1Tri& pFaceIDs )
    {
        Hw1Vec3 _normal;

        Hw1Vec3 _a = m_vertices[pFaceIDs.x];
        Hw1Vec3 _b = m_vertices[pFaceIDs.y];
        Hw1Vec3 _c = m_vertices[pFaceIDs.z];

        Hw1Vec3 _ab = Hw1Vec3( _b.x - _a.x, _b.y - _a.y, _b.z - _a.z );
        Hw1Vec3 _ac = Hw1Vec3( _c.x - _a.x, _c.y - _a.y, _c.z - _a.z );

        _normal.x = _ab.y * _ac.z - _ac.y * _ab.z;
        _normal.y = -_ab.x * _ac.z + _ac.x * _ab.z;
        _normal.z = _ab.x * _ac.y - _ac.x * _ab.y;

        //_normal.x *= -1;
        //_normal.y *= -1;
        //_normal.z *= -1;

        normalize( _normal );

        return _normal;
    }

    void computeNormalsNaive()
    {
        // Just assign each vertex normal ...
        // the same normal of its face

        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            Hw1Vec3 _n = computeFaceNormal( m_triFaces[q].verts );
            m_triFaces[q].n1 = _n;
            m_triFaces[q].n2 = _n;
            m_triFaces[q].n3 = _n;
        }
    }

    void computeTriFaces()
    {
        for ( int q = 0; q < m_tris.size(); q++ )
        {
            Hw1TriFace _face;
            _face.verts.x = m_tris[q].x;
            _face.verts.y = m_tris[q].y;
            _face.verts.z = m_tris[q].z;

            m_triFaces.push_back( _face );
        }
    }

    public :

    Hw1Vec3 scale;
    Hw1Vec3 rot;
    Hw1Vec3 pos;

    Hw1Letter()
    {
        m_depth = 2.0f;

        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
    }

    void addVertex( GLfloat x, GLfloat y, GLfloat z )
    {
        m_frontVertices.push_back( Hw1Vec3( x, y, z ) );
    }

    void generateGeometry()
    {
        for ( int q = 0; q < m_frontVertices.size(); q++ )
        {
            Hw1Vec3 _vfront = m_frontVertices[q];

            m_vertices.push_back( _vfront );
        }
        for ( int q = 0; q < m_frontVertices.size(); q++ )
        {
            Hw1Vec3 _vBack  = m_frontVertices[q];
            _vBack.z -= m_depth;

            m_vertices.push_back( _vBack );
        }

        int _nVertFront = m_frontVertices.size();

        for ( int q = 0; q < _nVertFront - 1; q++ )
        {
            GLuint _a = q;
            GLuint _b = q + 1;
            GLuint _ap = _a + _nVertFront;
            GLuint _bp = _b + _nVertFront;

            m_tris.push_back( Hw1Tri( _a, _b, _ap ) );
            m_tris.push_back( Hw1Tri( _ap, _b, _bp ) );
        }

        computeTriFaces();

        computeNormalsNaive();
    }

    void setColor( GLfloat r, GLfloat g, GLfloat b )
    {
        m_color.x = r;
        m_color.y = g;
        m_color.z = b;

        m_colorArr[0] = r;
        m_colorArr[1] = g;
        m_colorArr[2] = b;
        m_colorArr[3] = 1.0f;
    }

    void render()
    {
        glPushMatrix();

        glTranslatef( this->pos.x, this->pos.y, this->pos.z );
        glRotatef( this->rot.x, 1.0f, 0.0f, 0.0f );
        glRotatef( this->rot.y, 0.0f, 1.0f, 0.0f );
        glRotatef( this->rot.z, 0.0f, 0.0f, 1.0f );
        glScalef( this->scale.x, this->scale.y, this->scale.z );


        glBegin( GL_TRIANGLES );

        glColor3f( m_color.x, m_color.y, m_color.z );
        glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_colorArr );
        glMaterialfv( GL_FRONT, GL_SPECULAR, g_mat_specular );
        glMaterialfv( GL_FRONT, GL_EMISSION, g_mat_emmision );
        glMaterialf( GL_FRONT, GL_SHININESS, g_mat_shininess );

        /*
        for ( int q = 0; q < m_frontVertices.size() - 1; q++ )
        {
            Hw1Vec3 _va = m_frontVertices[q];
            Hw1Vec3 _vb = m_frontVertices[q + 1];
            Hw1Vec3 _vap = _va; _vap.z -= m_depth;
            Hw1Vec3 _vbp = _vb; _vbp.z -= m_depth;

            glVertex3f( _va.x, _va.y, _va.z );
            glVertex3f( _vap.x, _vap.y, _vap.z );
            glVertex3f( _vb.x, _vb.y, _vb.z );

            glVertex3f( _vap.x, _vap.y, _vap.z );
            glVertex3f( _vb.x, _vb.y, _vb.z );
            glVertex3f( _vbp.x, _vbp.y, _vbp.z );
        }
        */

        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            glNormal3f( m_triFaces[q].n1.x,
                        m_triFaces[q].n1.y,
                        m_triFaces[q].n1.z );
            glVertex3f( m_vertices[m_triFaces[q].verts.x].x,
                        m_vertices[m_triFaces[q].verts.x].y,
                        m_vertices[m_triFaces[q].verts.x].z );
            glNormal3f( m_triFaces[q].n2.x,
                        m_triFaces[q].n2.y,
                        m_triFaces[q].n2.z );
            glVertex3f( m_vertices[m_triFaces[q].verts.y].x,
                        m_vertices[m_triFaces[q].verts.y].y,
                        m_vertices[m_triFaces[q].verts.y].z );
            glNormal3f( m_triFaces[q].n3.x,
                        m_triFaces[q].n3.y,
                        m_triFaces[q].n3.z );
            glVertex3f( m_vertices[m_triFaces[q].verts.z].x,
                        m_vertices[m_triFaces[q].verts.z].y,
                        m_vertices[m_triFaces[q].verts.z].z );
        }

        glEnd();

        glPopMatrix();
    }

    void dump()
    {
        cout << "showing normals" << endl;
        for ( int q = 0; q < m_triFaces.size(); q++ )
        {
            cout << "n1.x: " << m_triFaces[q].n1.x << " - "
                 << "n1.y: " << m_triFaces[q].n1.y << " - "
                 << "n1.z: " << m_triFaces[q].n1.z << endl;
            cout << "n2.x: " << m_triFaces[q].n2.x << " - "
                 << "n2.y: " << m_triFaces[q].n2.y << " - "
                 << "n2.z: " << m_triFaces[q].n2.z << endl;
            cout << "n3.x: " << m_triFaces[q].n3.x << " - "
                 << "n3.y: " << m_triFaces[q].n3.y << " - "
                 << "n3.z: " << m_triFaces[q].n3.z << endl;
        }
        cout << "done" << endl;
    }

};

class Hw1App : public engine::core::LBaseApp
{
    private :

    vector<Hw1Letter> m_letters;
    Hw1LightSource m_light;
    Hw1Cube m_testCube;

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

    void loadLetters()
    {
        ENGINE_LOG( "Loading letters from file" );

        ifstream _filehandle( "res/letters.txt" );

        string _line;
        vector<string> _params;

        getline( _filehandle, _line );

        _params = split( _line );

        int _nLetters = stoi( _params[1] );

        cout << "_nLetters: " << _nLetters << endl;

        for ( int q = 0; q < _nLetters; q++ )
        {
            getline( _filehandle, _line );
            _params = split( _line );

            int _nVertInLetter = stoi( _params[1] );

            getline( _filehandle, _line );
            _params = split( _line );

            GLfloat _posX = stof( _params[1] );
            GLfloat _posY = stof( _params[2] );
            GLfloat _posZ = stof( _params[3] );

            getline( _filehandle, _line );
            _params = split( _line );

            int _colorR = stof( _params[1] );
            int _colorG = stof( _params[2] );
            int _colorB = stof( _params[3] );

            Hw1Letter _letter;

            for ( int p = 0; p < _nVertInLetter; p++ )
            {
                getline( _filehandle, _line );
                _params = split( _line );

                GLfloat _x = stof( _params[0] );
                GLfloat _y = stof( _params[1] );
                GLfloat _z = 1.0f;

                _letter.addVertex( _x, _y, _z );
                _letter.setColor( _colorR, _colorG, _colorB );
                _letter.pos.x = _posX;
                _letter.pos.y = _posY;
                _letter.pos.z = _posZ;
            }

            _letter.generateGeometry();
            //_letter.dump();

            m_letters.push_back( _letter );
        }

        ENGINE_LOG( "Finished loading letters from file" );
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

        glShadeModel( GL_SMOOTH );
        glEnable( GL_LIGHTING );

        glEnable( GL_DEPTH_TEST );
        glDisable( GL_CULL_FACE );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 60.0, ( float ) ENGINE_APP_WIDTH / ENGINE_APP_HEIGHT, 1.0, 10.0 );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt( 4.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
        //gluLookAt( 0.0f, 2.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

        loadLetters();

        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, g_ambientLight );

        m_light.setLightId( GL_LIGHT0 );
        m_light.setLightPosition( 0.0f, 2.0f, 0.0f );
        m_light.initLighting();

        m_testCube.setColor( 1.0f, 0.0f, 0.0f );

        m_testCube.dump();
    }

    void render() override
    {

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // GLfloat _rotX = 180.0f * cos( 0.5 * m_timeNow );
        // GLfloat _rotY = 180.0f * sin( 0.5 * m_timeNow );
        GLfloat _rotX = 0.0f;//2.5f;
        GLfloat _rotY = 0.0f;//2.5f;

        glEnable( GL_LIGHTING );

        GLfloat _x = 2.5 * cos( 0.5 * m_timeNow );
        GLfloat _y = 2.5;// * sin( 0.5 * m_timeNow );
        GLfloat _z = 0.0f;//2.5 * cos( 0.5 * m_timeNow );
        //m_light.setLightPosition( _x, _y, _z );

        m_testCube.rot.x = 180.0f * cos( 0.5 * m_timeNow );
        m_testCube.rot.y = 180.0f * sin( 0.5 * m_timeNow );
        m_testCube.render();

        for ( int q = 0; q < m_letters.size(); q++ )
        {
            m_letters[q].rot.x = _rotX;
            m_letters[q].rot.y = _rotY;

            //m_letters[q].render();
        }

        glDisable( GL_LIGHTING );

        m_light.indicator().rot.x = 180.0f * cos( 0.5 * m_timeNow );
        m_light.indicator().rot.y = 180.0f * sin( 0.5 * m_timeNow );
        m_light.indicator().render();

    }

};




int main()
{

    Hw1App::create();
    Hw1App::instance->loop();
    Hw1App::destroy();

    return 0;
}

