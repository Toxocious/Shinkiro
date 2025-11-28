#include <Asset/AssetBundleManager.h>

#include <Core/IApplication.h>
#include <Core/Util/Log.h>

#include <Platform/Modules/Window.h>

#include <Renderer/Map/MapManager.h>

#include <Editor/UI/EditorUI.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <lucide/IconsLucide.h>

namespace Shinkiro::Editor
{
    EditorGui::EditorGui()
    {
        SHNK_CORE_INFO( "Editor GUI layer created." );
    }

    EditorGui::~EditorGui()
    {
        Shutdown();
    }

    bool EditorGui::Initialize( void * imguiContext )
    {
        ImGui::SetCurrentContext( ( ImGuiContext * ) imguiContext );

        SetupImGuiStyle();

        m_ImGuiInitialized = true;
        SHNK_CORE_INFO( "Editor GUI attached to shared ImGui Context." );
        return true;
    }

    void EditorGui::Shutdown()
    {
        m_ImGuiInitialized = false;
    }

    void EditorGui::BeginFrame()
    {
    }

    void EditorGui::EndFrame()
    {
    }

    void EditorGui::Render()
    {
    }

    void EditorGui::RenderLoadingScreen()
    {
        ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
        ImGui::SetNextWindowSize( ImGui::GetIO().DisplaySize );
        ImGui::Begin( "LoadingScreen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground );

        float windowWidth  = ImGui::GetWindowSize().x;
        float windowHeight = ImGui::GetWindowSize().y;

        float titleTextHeight    = ImGui::CalcTextSize( "Shinkiro Editor" ).y;
        float loadingTextHeight  = ImGui::CalcTextSize( "Loading..." ).y;
        float totalContentHeight = 100.0f + titleTextHeight + loadingTextHeight + ImGui::GetStyle().ItemSpacing.y * 2;

        ImGui::SetCursorPosY( ( windowHeight - totalContentHeight ) * 0.5f );

        float loadingTextWidth = ImGui::CalcTextSize( "Loading..." ).x;
        ImGui::SetCursorPosX( ( windowWidth - loadingTextWidth ) * 0.5f );
        ImGui::Text( "Loading..." );

        ImGui::End();
    }

    // =============================================================
    // =============================================================
    // =============================================================

    void EditorGui::RenderPanels()
    {
        RenderMainMenuBar();
        RenderTilesetPanel();
        RenderTilesetPanelButtons();
        RenderObjectsPanel();
    }

    void EditorGui::SetupImGuiStyle()
    {
        ImGuiStyle & style = ImGui::GetStyle();

        style.Alpha            = 1.0f;
        style.DisabledAlpha    = 0.60f;
        style.WindowPadding    = ImVec2( 8, 8 );
        style.WindowRounding   = 8.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize    = ImVec2( 32, 32 );
        style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );

        style.ChildRounding   = 4.0f;
        style.ChildBorderSize = 1.0f;

        style.PopupRounding   = 4.0f;
        style.PopupBorderSize = 1.0f;

        style.FramePadding    = ImVec2( 4, 3 );
        style.FrameRounding   = 4.0f;
        style.FrameBorderSize = 0.0f;

        style.ItemSpacing       = ImVec2( 8, 4 );
        style.ItemInnerSpacing  = ImVec2( 4, 4 );
        style.CellPadding       = ImVec2( 4, 2 );
        style.IndentSpacing     = 21.0f;
        style.ColumnsMinSpacing = 6.0f;

        style.ScrollbarSize     = 10.0f;
        style.ScrollbarRounding = 9.0f;

        style.GrabMinSize  = 10.0f;
        style.GrabRounding = 4.0f;

        style.TabRounding   = 4.0f;
        style.TabBorderSize = 0.0f;

        style.ButtonTextAlign     = ImVec2( 0.5f, 0.5f );
        style.SelectableTextAlign = ImVec2( 0.0f, 0.0f );

        ImVec4 * colors = style.Colors;

        const ImVec4 color_text                    = ImVec4( 0.70f, 0.70f, 0.70f, 1.00f );
        const ImVec4 color_text_disabled           = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
        const ImVec4 color_window_bg               = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );
        const ImVec4 color_child_bg                = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
        const ImVec4 color_popup_bg                = ImVec4( 0.15f, 0.15f, 0.15f, 0.94f );
        const ImVec4 color_border                  = ImVec4( 0.25f, 0.25f, 0.25f, 0.50f );
        const ImVec4 color_frame_bg                = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
        const ImVec4 color_frame_bg_hovered        = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
        const ImVec4 color_frame_bg_active         = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
        const ImVec4 color_title_bg                = ImVec4( 0.10f, 0.10f, 0.10f, 1.00f );
        const ImVec4 color_title_bg_active         = ImVec4( 0.10f, 0.10f, 0.10f, 1.00f );
        const ImVec4 color_title_bg_collapsed      = ImVec4( 0.10f, 0.10f, 0.10f, 1.00f );
        const ImVec4 color_menubar_bg              = ImVec4( 0.15f, 0.15f, 0.15f, 1.00f );
        const ImVec4 color_scrollbar_bg            = ImVec4( 0.10f, 0.10f, 0.10f, 0.00f );
        const ImVec4 color_scrollbar_grab          = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
        const ImVec4 color_scrollbar_grab_hovered  = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );
        const ImVec4 color_scrollbar_grab_active   = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
        const ImVec4 color_check_mark              = ImVec4( 0.40f, 0.70f, 1.00f, 1.00f );
        const ImVec4 color_slider_grab             = ImVec4( 0.35f, 0.65f, 0.95f, 1.00f );
        const ImVec4 color_slider_grab_active      = ImVec4( 0.40f, 0.70f, 1.00f, 1.00f );
        const ImVec4 color_button                  = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
        const ImVec4 color_button_hovered          = ImVec4( 0.35f, 0.35f, 0.35f, 1.00f );
        const ImVec4 color_button_active           = ImVec4( 0.45f, 0.45f, 0.45f, 1.00f );
        const ImVec4 color_header                  = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
        const ImVec4 color_header_hovered          = ImVec4( 0.35f, 0.35f, 0.35f, 1.00f );
        const ImVec4 color_header_active           = ImVec4( 0.45f, 0.45f, 0.45f, 1.00f );
        const ImVec4 color_separator               = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
        const ImVec4 color_separator_hovered       = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );
        const ImVec4 color_separator_active        = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
        const ImVec4 color_resize_grip             = ImVec4( 0.35f, 0.65f, 0.95f, 0.20f );
        const ImVec4 color_resize_grip_hovered     = ImVec4( 0.35f, 0.65f, 0.95f, 0.67f );
        const ImVec4 color_resize_grip_active      = ImVec4( 0.35f, 0.65f, 0.95f, 0.95f );
        const ImVec4 color_tab                     = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
        const ImVec4 color_tab_hovered             = ImVec4( 0.35f, 0.35f, 0.35f, 1.00f );
        const ImVec4 color_tab_active              = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
        const ImVec4 color_tab_unfocused           = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
        const ImVec4 color_tab_unfocused_active    = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
        const ImVec4 color_docking_preview         = ImVec4( 0.35f, 0.65f, 0.95f, 0.70f );
        const ImVec4 color_docking_empty_bg        = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
        const ImVec4 color_plot_lines              = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
        const ImVec4 color_plot_lines_hovered      = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
        const ImVec4 color_plot_histogram          = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
        const ImVec4 color_plot_histogram_hovered  = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
        const ImVec4 color_table_header_bg         = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
        const ImVec4 color_table_border_strong     = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );
        const ImVec4 color_table_border_light      = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );
        const ImVec4 color_table_row_bg            = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
        const ImVec4 color_table_row_bg_alt        = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
        const ImVec4 color_text_selected_bg        = ImVec4( 0.35f, 0.65f, 0.95f, 0.35f );
        const ImVec4 color_drag_drop_target        = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
        const ImVec4 color_nav_highlight           = ImVec4( 0.35f, 0.65f, 0.95f, 1.00f );
        const ImVec4 color_nav_windowing_highlight = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
        const ImVec4 color_nav_windowing_dim_bg    = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
        const ImVec4 color_modal_window_dim_bg     = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );

        colors[ImGuiCol_Text]                  = color_text;
        colors[ImGuiCol_TextDisabled]          = color_text_disabled;
        colors[ImGuiCol_WindowBg]              = color_window_bg;
        colors[ImGuiCol_ChildBg]               = color_child_bg;
        colors[ImGuiCol_PopupBg]               = color_popup_bg;
        colors[ImGuiCol_Border]                = color_border;
        colors[ImGuiCol_BorderShadow]          = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
        colors[ImGuiCol_FrameBg]               = color_frame_bg;
        colors[ImGuiCol_FrameBgHovered]        = color_frame_bg_hovered;
        colors[ImGuiCol_FrameBgActive]         = color_frame_bg_active;
        colors[ImGuiCol_TitleBg]               = color_title_bg;
        colors[ImGuiCol_TitleBgActive]         = color_title_bg_active;
        colors[ImGuiCol_TitleBgCollapsed]      = color_title_bg_collapsed;
        colors[ImGuiCol_MenuBarBg]             = color_menubar_bg;
        colors[ImGuiCol_ScrollbarBg]           = color_scrollbar_bg;
        colors[ImGuiCol_ScrollbarGrab]         = color_scrollbar_grab;
        colors[ImGuiCol_ScrollbarGrabHovered]  = color_scrollbar_grab_hovered;
        colors[ImGuiCol_ScrollbarGrabActive]   = color_scrollbar_grab_active;
        colors[ImGuiCol_CheckMark]             = color_check_mark;
        colors[ImGuiCol_SliderGrab]            = color_slider_grab;
        colors[ImGuiCol_SliderGrabActive]      = color_slider_grab_active;
        colors[ImGuiCol_Button]                = color_button;
        colors[ImGuiCol_ButtonHovered]         = color_button_hovered;
        colors[ImGuiCol_ButtonActive]          = color_button_active;
        colors[ImGuiCol_Header]                = color_header;
        colors[ImGuiCol_HeaderHovered]         = color_header_hovered;
        colors[ImGuiCol_HeaderActive]          = color_header_active;
        colors[ImGuiCol_Separator]             = color_separator;
        colors[ImGuiCol_SeparatorHovered]      = color_separator_hovered;
        colors[ImGuiCol_SeparatorActive]       = color_separator_active;
        colors[ImGuiCol_ResizeGrip]            = color_resize_grip;
        colors[ImGuiCol_ResizeGripHovered]     = color_resize_grip_hovered;
        colors[ImGuiCol_ResizeGripActive]      = color_resize_grip_active;
        colors[ImGuiCol_Tab]                   = color_tab;
        colors[ImGuiCol_TabHovered]            = color_tab_hovered;
        colors[ImGuiCol_TabActive]             = color_tab_active;
        colors[ImGuiCol_TabUnfocused]          = color_tab_unfocused;
        colors[ImGuiCol_TabUnfocusedActive]    = color_tab_unfocused_active;
        colors[ImGuiCol_PlotLines]             = color_plot_lines;
        colors[ImGuiCol_PlotLinesHovered]      = color_plot_lines_hovered;
        colors[ImGuiCol_PlotHistogram]         = color_plot_histogram;
        colors[ImGuiCol_PlotHistogramHovered]  = color_plot_histogram_hovered;
        colors[ImGuiCol_TableHeaderBg]         = color_table_header_bg;
        colors[ImGuiCol_TableBorderStrong]     = color_table_border_strong;
        colors[ImGuiCol_TableBorderLight]      = color_table_border_light;
        colors[ImGuiCol_TableRowBg]            = color_table_row_bg;
        colors[ImGuiCol_TableRowBgAlt]         = color_table_row_bg_alt;
        colors[ImGuiCol_TextSelectedBg]        = color_text_selected_bg;
        colors[ImGuiCol_DragDropTarget]        = color_drag_drop_target;
        colors[ImGuiCol_NavHighlight]          = color_nav_highlight;
        colors[ImGuiCol_NavWindowingHighlight] = color_nav_windowing_highlight;
        colors[ImGuiCol_NavWindowingDimBg]     = color_nav_windowing_dim_bg;
        colors[ImGuiCol_ModalWindowDimBg]      = color_modal_window_dim_bg;
    }

    void EditorGui::RenderButtonDropdown( const char * label, const std::function<void()> & content )
    {
        {
            ImGuiStyle & style      = ImGui::GetStyle();
            ImVec2       textSize   = ImGui::CalcTextSize( label );
            ImVec2       padding    = style.FramePadding;
            ImVec2       buttonSize = ImVec2( textSize.x + padding.x * 2.0f, textSize.y + padding.y * 2.0f );

            ImVec2 pMin = ImGui::GetCursorScreenPos();
            ImVec2 pMax = ImVec2( pMin.x + buttonSize.x, pMin.y + buttonSize.y );

            bool isHovered = ImGui::IsMouseHoveringRect( pMin, pMax );
            bool isOpen    = ImGui::IsPopupOpen( label );

            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );

            if ( isHovered || isOpen )
            {
                ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            }

            if ( ImGui::Button( label ) )
            {
                ImGui::OpenPopup( label );
            }

            if ( isHovered || isOpen )
            {
                ImGui::PopStyleColor( 1 );
            }

            ImGui::PopStyleColor( 3 );
        }

        // If the popup is open, position it directly below the button
        if ( ImGui::IsPopupOpen( label ) )
        {
            ImVec2 rect_min = ImGui::GetItemRectMin();
            ImVec2 rect_max = ImGui::GetItemRectMax();
            ImGui::SetNextWindowPos( ImVec2( rect_min.x, rect_max.y + 5.0f ) );
        }

        {
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10.0f, 10.0f ) );
            {
                if ( ImGui::BeginPopup( label ) )
                {
                    {
                        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 4.0f, 3.0f ) );
                        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 6.0f );

                        content();

                        ImGui::PopStyleVar( 2 );
                        ImGui::EndPopup();
                    }
                }
            }
            ImGui::PopStyleVar( 1 );
        }
    }

    void EditorGui::RenderMainMenuBarMapName()
    {
        const char * mapName     = "Untitled Map";
        float        windowWidth = ImGui::GetWindowSize().x;
        float        textWidth   = ImGui::CalcTextSize( mapName ).x;

        float posX = ( windowWidth - textWidth ) * 0.5f;

        if ( posX < ImGui::GetCursorPosX() )
        {
            posX = ImGui::GetCursorPosX();
        }

        ImGui::SetCursorPosX( posX );

        ImGui::AlignTextToFramePadding();
        ImGui::Text( "%s", mapName );
    }

    void EditorGui::RenderMainMenuBar()
    {
        ImGuiViewport * viewport = ImGui::GetMainViewport();
        float           padding  = 12.0f;

        ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x + padding, viewport->Pos.y + padding ) );
        ImGui::SetNextWindowSize( ImVec2( viewport->Size.x - ( padding * 2 ), 0 ) );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 8.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10.0f, 0.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 10.0f, 10.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 0.0f );

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if ( ImGui::Begin( "CustomMainMenuBar", nullptr, window_flags ) )
        {
            EditorGui::RenderButtonDropdown( "Map", []()
                                             {
                                                 if ( ImGui::MenuItem( "New Map" ) )
                                                 {
                                                     // Handle save
                                                 }

                                                 ImGui::Spacing();
                                                 ImGui::Separator();
                                                 ImGui::Spacing();

                                                 if ( ImGui::MenuItem( "Save Map (Local)" ) )
                                                 {
                                                     // Handle load
                                                 }
                                                 if ( ImGui::MenuItem( "Load Map (Local)" ) )
                                                 {
                                                     // Handle load
                                                 }

                                                 ImGui::Spacing();
                                                 ImGui::Separator();
                                                 ImGui::Spacing();

                                                 if ( ImGui::MenuItem( "Save Map (Server)" ) )
                                                 {
                                                     // Handle load
                                                 }
                                                 if ( ImGui::MenuItem( "Load Map (Server)" ) )
                                                 {
                                                     // Handle load
                                                 }

                                                 //  ImGui::Separator();

                                                 //  static bool enabled = false;
                                                 //  if ( ImGui::Checkbox( "Enable Feature", &enabled ) )
                                                 //  {
                                                 //      // Handle toggle
                                                 //  }
                                             } );

            ImGui::SameLine();

            EditorGui::RenderButtonDropdown( "Edit", []()
                                             {
                                                 if ( ImGui::MenuItem( "Undo" ) )
                                                 {
                                                     // Handle save
                                                 }

                                                 if ( ImGui::MenuItem( "Redo" ) )
                                                 {
                                                     // Handle load
                                                 }
                                             } );

            ImGui::SameLine();

            EditorGui::RenderButtonDropdown( "Camera", []()
                                             {
                                                 if ( ImGui::MenuItem( "Default View" ) )
                                                 {
                                                     auto   ActiveMapData = Shinkiro::Core::App->GetMapManager().GetActiveMap();
                                                     auto & ActiveCamera  = Shinkiro::Core::App->GetCamera();

                                                     float     targetX = ActiveMapData->width / 2.0f;
                                                     float     targetZ = ActiveMapData->height / 2.0f;
                                                     glm::vec3 targetPosition( targetX, 0.0f, targetZ );
                                                     glm::vec3 cameraPosition( targetX, 30.0f, targetZ + 30.0f );
                                                     glm::vec3 direction = glm::normalize( targetPosition - cameraPosition );

                                                     ActiveCamera.SetPosition( cameraPosition );
                                                     ActiveCamera.SetPitch( glm::degrees( asin( direction.y ) ) );
                                                     ActiveCamera.SetYaw( glm::degrees( atan2( direction.z, direction.x ) ) );
                                                     ActiveCamera.updateCameraVectors();
                                                 }

                                                 if ( ImGui::MenuItem( "Top-Down View" ) )
                                                 {
                                                     auto   ActiveMapData = Shinkiro::Core::App->GetMapManager().GetActiveMap();
                                                     auto & ActiveCamera  = Shinkiro::Core::App->GetCamera();

                                                     float targetX = ActiveMapData->width / 2.0f;
                                                     float targetZ = ActiveMapData->height / 2.0f;

                                                     ActiveCamera.SetPosition( glm::vec3( targetX, 40.0f, targetZ ) );
                                                     ActiveCamera.SetPitch( -89.9f );
                                                     ActiveCamera.SetYaw( -90.0f );
                                                     ActiveCamera.updateCameraVectors();
                                                 }
                                             } );

            ImGui::SameLine();
            RenderMainMenuBarMapName();
        }

        ImGui::PopStyleVar( 5 );

        ImGui::End();
    }

    void EditorGui::RenderTilesetPanelButtons()
    {
        ImGuiViewport * viewport = ImGui::GetMainViewport();

        float padding = 4.0f;
        float spacing = 12.0f;

        float tileSize       = 16.0f;
        float tilesPerRow    = 24.0f;
        float tilesPerColumn = 24.0f;
        float tilesetHeight  = tileSize * tilesPerColumn;

        ImVec2 anchorPos = ImVec2(
            viewport->Pos.x + padding,
            viewport->Pos.y + viewport->Size.y - tilesetHeight - padding - spacing
        );

        ImGui::SetNextWindowPos( anchorPos, ImGuiCond_Always, ImVec2( 0.0f, 1.0f ) );

        ImVec4 buttonColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

        ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
        ImGui::PushStyleColor( ImGuiCol_Button, buttonColor );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 8.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10.0f, 0.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 4.0f, 4.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 8.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign, ImVec2( 0.5f, 1.0f ) );

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if ( ImGui::Begin( "TilesetPanelButtons", nullptr, window_flags ) )
        {
            ImVec2 btnSize( 36.0f, 36.0f );

            if ( ImGui::Button( ICON_LC_PAINTBRUSH, btnSize ) )
            {
            }

            ImGui::SameLine();

            if ( ImGui::Button( ICON_LC_PAINT_BUCKET, btnSize ) )
            {
            }

            ImGui::SameLine();

            if ( ImGui::Button( ICON_LC_MOVE, btnSize ) )
            {
            }

            ImGui::SameLine();

            if ( ImGui::Button( ICON_LC_ERASER, btnSize ) )
            {
            }
        }

        ImGui::PopStyleVar( 6 );
        ImGui::PopStyleColor( 2 );

        ImGui::End();
    }

    void EditorGui::RenderTilesetPanel()
    {
        ImGuiViewport * viewport = ImGui::GetMainViewport();
        float           padding  = 12.0f;

        float tileSize       = 16.0f;
        float tilesPerRow    = 24.0f;
        float tilesPerColumn = 24.0f;

        ImVec2 windowSize = ImVec2( tileSize * tilesPerRow, tileSize * tilesPerColumn );

        ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x + padding, viewport->Pos.y + viewport->Size.y - windowSize.y - padding ) );
        ImGui::SetNextWindowSize( windowSize );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 8.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10.0f, 0.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 10.0f, 10.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 0.0f );

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if ( ImGui::Begin( "TilesetPanel", nullptr, window_flags ) )
        {
        }

        ImGui::PopStyleVar( 5 );

        ImGui::End();
    }

    void EditorGui::RenderObjectsPanel()
    {
        ImGuiViewport * viewport = ImGui::GetMainViewport();
        float           padding  = 12.0f;

        float tileSize       = 16.0f;
        float tilesPerRow    = 24.0f;
        float tilesPerColumn = 24.0f;

        ImVec2 windowSize = ImVec2( tileSize * tilesPerRow, tileSize * tilesPerColumn );

        ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x + viewport->Size.x - windowSize.x - padding, viewport->Pos.y + viewport->Size.y - windowSize.y - padding ) );
        ImGui::SetNextWindowSize( windowSize );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 8.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10.0f, 0.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 10.0f, 10.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 0.0f );

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if ( ImGui::Begin( "ObjectsPanel", nullptr, window_flags ) )
        {
        }

        ImGui::PopStyleVar( 5 );

        ImGui::End();
    }
}
