#pragma once

#include <gui/CImGuiLayer.h>

#include <renderers/CMainRenderer.h>
#include <graphics/CScene.h>

namespace engine
{

    class CImGuiSceneLayer : public CImGuiLayer
    {

    public :

        CImGuiSceneLayer( const std::string& name )
            : CImGuiLayer( name ) {}
        ~CImGuiSceneLayer() {}

        void update() override {}
        void render() override {}
        bool onEvent( const CInputEvent& event ) override { return false; }

    };

}
