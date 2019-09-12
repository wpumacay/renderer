#pragma once

#include <utils/CImguiUi.h>

namespace engine
{

    class CImguiUiDemo : public CImguiUi
    {
    public :
        CImguiUiDemo( COpenGLContext* context );
        ~CImguiUiDemo();

    protected :

        void _initInternal() override;
        void _renderInternal() override;

    private :


    };

}