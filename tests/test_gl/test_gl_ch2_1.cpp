

#include <core/LBaseApp.h>


class SampleApp : public engine::core::LBaseApp
{
    protected :

    SampleApp() : engine::core::LBaseApp()
    {

    }

    public :

    static void create()
    {
        if ( SampleApp::instance != NULL )
        {
            delete SampleApp::instance;
        }

        SampleApp::instance = new SampleApp();
        SampleApp::instance->init();
    }

    void render() override
    {
        static const GLfloat _red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv( GL_COLOR, 0, _red );
    }

};




int main()
{

    SampleApp::create();
    SampleApp::instance->loop();
    SampleApp::destroy();

    return 0;
}

