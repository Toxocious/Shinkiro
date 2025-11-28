#pragma once
#ifndef SHINKIRO_EDITOR_EDITORUI_H
#    define SHINKIRO_EDITOR_EDITORUI_H

#    include <Core/Interfaces/GuiLayerInterface.h>

#    include <GLFW/glfw3.h>

#    include <functional>
#    include <string>

namespace Shinkiro::Editor
{
    class EditorGui : public Shinkiro::Core::IGuiLayer
    {
    public:
        EditorGui();
        ~EditorGui() override;

        bool Initialize( void * imguiContext ) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;

        void Render() override;
        void RenderLoadingScreen();
        void RenderPanels();

    private:
        void SetupImGuiStyle();

        void RenderButtonDropdown( const char * label, const std::function<void()> & content );

        void RenderMainMenuBar();
        void RenderMainMenuBarMapName();

        void RenderTilesetPanel();
        void RenderTilesetPanelButtons();

        void RenderObjectsPanel();
        void RenderObjectsPanelButtons();

    private:
        bool m_ImGuiInitialized = false;
    };
}

#endif
