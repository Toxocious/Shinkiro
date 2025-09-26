#include <Renderer/_Common.h>

#include <Renderer/Map/MapRenderer.h>

#include <Core/IApplication.h>
#include <Core/Util/FileSystem.h>

#include <Platform/GL.h>
#include <Platform/Modules/Window.h>

#include <Asset/AssetBundleManager.h>

#include <Log/Log.h>

namespace Shinkiro::Renderer
{
    MapRenderer::MapRenderer()
    {
    }

    MapRenderer::~MapRenderer()
    {
    }

    bool MapRenderer::InitializeMapTileShader()
    {
        m_MapTileShader = std::make_unique<Shader>(
            "Shaders/MapTile/maptile.vertex.glsl",
            "Shaders/MapTile/maptile.fragment.glsl"
        );

        if ( m_MapTileShader->GetID() == 0 )
        {
            SHNK_CORE_ERROR( "Renderer failed to initialize MapTile shader!" );
            return false;
        }

        m_MapTileShader->Use();
        m_MapTileShader->SetMat4( "model", glm::mat4( 1.0f ) );
        m_MapTileShader->SetMat4( "view", glm::mat4( 1.0f ) );
        m_MapTileShader->SetMat4( "projection", glm::mat4( 1.0f ) );
        m_MapTileShader->SetVec2( "uvOffset", glm::vec2( 0.0f ) );
        m_MapTileShader->SetVec2( "uvScale", glm::vec2( 1.0f ) );

        return true;
    }

    bool MapRenderer::InitializeMapTileHighlighterShader()
    {
        m_MapTileHighlighterShader = std::make_unique<Shader>(
            "Shaders/MapTileHighlight/maptilehighlight.vertex.glsl",
            "Shaders/MapTileHighlight/maptilehighlight.fragment.glsl"
        );

        if ( m_MapTileHighlighterShader->GetID() == 0 )
        {
            SHNK_CORE_ERROR( "Renderer failed to initialize MapTileHighlight shader!" );
            return false;
        }

        m_MapTileHighlighterShader->Use();
        m_MapTileHighlighterShader->SetMat4( "model", glm::mat4( 1.0f ) );
        m_MapTileHighlighterShader->SetMat4( "view", glm::mat4( 1.0f ) );
        m_MapTileHighlighterShader->SetMat4( "projection", glm::mat4( 1.0f ) );
        m_MapTileHighlighterShader->SetVec4( "highlightColor", glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

        // Setup vertex data
        Shinkiro::Platform::OpenGL::glGenVertexArrays( 1, &m_CubeVAO );
        Shinkiro::Platform::OpenGL::glGenBuffers( 1, &m_CubeVBO );
        Shinkiro::Platform::OpenGL::glBindVertexArray( m_CubeVAO );
        Shinkiro::Platform::OpenGL::glBindBuffer( GL_ARRAY_BUFFER, m_CubeVBO );
        Shinkiro::Platform::OpenGL::glBufferData( GL_ARRAY_BUFFER, sizeof( m_TileVertices ), m_TileVertices, GL_STATIC_DRAW );

        // Position attribute
        Shinkiro::Platform::OpenGL::glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) 0 );
        Shinkiro::Platform::OpenGL::glEnableVertexAttribArray( 0 );

        // Texture coord attribute
        Shinkiro::Platform::OpenGL::glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) ( 3 * sizeof( float ) ) );
        Shinkiro::Platform::OpenGL::glEnableVertexAttribArray( 1 );

        return true;
    }

    bool MapRenderer::InitializeMapSkyboxShader()
    {
        m_MapSkyboxShader = std::make_unique<Shader>(
            "Shaders/MapSkybox/mapskybox.vertex.glsl",
            "Shaders/MapSkybox/mapskybox.fragment.glsl"
        );

        if ( m_MapSkyboxShader->GetID() == 0 )
        {
            SHNK_CORE_ERROR( "Renderer failed to initialize MapSkybox shader!" );
            return false;
        }
        else
        {
            SHNK_CORE_INFO( "Renderer initialized MapSkybox shader successfully. (ID: {})", m_MapSkyboxShader->GetID() );
        }

        Shinkiro::Platform::OpenGL::glGenVertexArrays( 1, &m_SkyboxVAO );
        Shinkiro::Platform::OpenGL::glGenBuffers( 1, &m_SkyboxVBO );
        Shinkiro::Platform::OpenGL::glBindVertexArray( m_SkyboxVAO );
        Shinkiro::Platform::OpenGL::glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBO );
        Shinkiro::Platform::OpenGL::glBufferData( GL_ARRAY_BUFFER, sizeof( m_SkyboxVertices ), &m_SkyboxVertices, GL_STATIC_DRAW );

        Shinkiro::Platform::OpenGL::glEnableVertexAttribArray( 0 );
        Shinkiro::Platform::OpenGL::glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void * ) 0 );

        // The order must be: Right, Left, Top, Bottom, Front, Back.
        std::vector<std::string> faces = {
            ( "Textures/Skybox/Skybox_Right.png" ), // Right
            ( "Textures/Skybox/Skybox_Left.png" ),  // Left
            ( "Textures/Skybox/Skybox_Up.png" ),    // Top
            ( "Textures/Skybox/Skybox_Down.png" ),  // Bottom
            ( "Textures/Skybox/Skybox_Front.png" ), // Front
            ( "Textures/Skybox/Skybox_Back.png" )   // Back
        };
        m_cubemapTexture = LoadCubemap( faces );

        return true;
    }

    bool MapRenderer::Initialize()
    {
        Shinkiro::Platform::OpenGL::Initialize( Shinkiro::Core::App->GetWindow()->GetGLFWWindow() );

        InitializeMapTileShader();
        InitializeMapTileHighlighterShader();
        InitializeMapSkyboxShader();

        Shinkiro::Platform::OpenGL::glBindVertexArray( 0 );

        return true;
    }

    void MapRenderer::Shutdown()
    {
        Shinkiro::Platform::OpenGL::glDeleteVertexArrays( 1, &m_CubeVAO );
        Shinkiro::Platform::OpenGL::glDeleteBuffers( 1, &m_CubeVBO );

        Shinkiro::Platform::OpenGL::glDeleteVertexArrays( 1, &m_SkyboxVAO );
        Shinkiro::Platform::OpenGL::glDeleteBuffers( 1, &m_SkyboxVBO );
        Shinkiro::Platform::OpenGL::glDeleteTextures( 1, &m_cubemapTexture );
    }

    void MapRenderer::DrawHighlight( int x, int y, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection )
    {
        m_MapTileHighlighterShader->Use();

        m_MapTileHighlighterShader->SetMat4( "view", view );
        m_MapTileHighlighterShader->SetMat4( "projection", projection );

        m_MapTileHighlighterShader->SetVec4( "highlightColor", glm::vec4( 0.2f, 0.2f, 1.0f, 0.5f ) );

        glm::mat4 model = glm::mat4( 1.0f );
        model           = glm::translate( model, glm::vec3( static_cast<float>( x ), layer_y_offset + 0.51f, static_cast<float>( y ) ) );
        model           = glm::scale( model, glm::vec3( 1.0f, 0.1f, 1.0f ) );

        m_MapTileHighlighterShader->SetMat4( "model", model );

        Shinkiro::Platform::OpenGL::glEnable( GL_BLEND );
        Shinkiro::Platform::OpenGL::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        Shinkiro::Platform::OpenGL::glDepthMask( GL_FALSE );

        Shinkiro::Platform::OpenGL::glBindVertexArray( m_CubeVAO );
        Shinkiro::Platform::OpenGL::glDrawArrays( GL_TRIANGLES, 0, 36 );

        Shinkiro::Platform::OpenGL::glDepthMask( GL_TRUE );
        Shinkiro::Platform::OpenGL::glBindVertexArray( 0 );
        Shinkiro::Platform::OpenGL::glDisable( GL_BLEND );
    }

    void MapRenderer::InitializeAndSetCamera()
    {
        float     targetX = m_MapData->width / 2.0f;
        float     targetZ = m_MapData->height / 2.0f;
        glm::vec3 targetPosition( targetX, 0.0f, targetZ );
        glm::vec3 cameraPosition( targetX, 30.0f, targetZ + 30.0f );
        glm::vec3 direction = glm::normalize( targetPosition - cameraPosition );
        float     pitch     = glm::degrees( asin( direction.y ) );
        float     yaw       = glm::degrees( atan2( direction.z, direction.x ) );

        auto & camera = Shinkiro::Core::App->GetCamera();
        camera.SetPosition( cameraPosition );
        camera.SetUp( glm::vec3( 0.0f, 1.0f, 0.0f ) );
        camera.SetPitch( pitch );
        camera.SetYaw( yaw );
        camera.SetProjection( glm::perspective( glm::radians( camera.GetZoom() ), ( float ) 1920 / ( float ) 1080, 0.1f, 1000.0f ) );
        camera.updateCameraVectors();
    }

    void MapRenderer::LoadTilesetTextures( MapData & mapData )
    {
        stbi_set_flip_vertically_on_load( true );

        for ( auto & tileset : mapData.tilesets )
        {
            Shinkiro::Platform::OpenGL::glGenTextures( 1, &tileset.textureID );
            Shinkiro::Platform::OpenGL::glBindTexture( GL_TEXTURE_2D, tileset.textureID );

            Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

            Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            int width, height, nrChannels;

            const std::string tilesetImageSource = "Maps/" + tileset.imageSource;

            const auto textureData = Shinkiro::Core::App->GetBundleManager().GetAssetData( tilesetImageSource );
            if ( !textureData.empty() )
            {
                unsigned char * data = stbi_load_from_memory(
                    textureData.data(),
                    static_cast<int>( textureData.size() ),
                    &width,
                    &height,
                    &nrChannels,
                    0
                );

                if ( data )
                {
                    GLenum format = ( nrChannels == 4 ) ? GL_RGBA : GL_RGB;
                    Shinkiro::Platform::OpenGL::glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
                    Shinkiro::Platform::OpenGL::glGenerateMipmap( GL_TEXTURE_2D );

                    SHNK_CORE_INFO( "Loaded texture: {}", tilesetImageSource );
                }
                else
                {
                    SHNK_CORE_ERROR( "Failed to load texture from memory: {}", tilesetImageSource );
                }

                stbi_image_free( data );
            }
            else
            {
                SHNK_CORE_ERROR( "Failed to get asset data for texture: {}", tilesetImageSource );
            }
        }

        stbi_set_flip_vertically_on_load( false );
    }

    void MapRenderer::UnloadTilesetTextures( const MapData & mapData )
    {
        for ( const auto & tileset : mapData.tilesets )
        {
            if ( tileset.textureID != 0 )
            {
                Shinkiro::Platform::OpenGL::glDeleteTextures( 1, &tileset.textureID );
            }
        }

        SHNK_CORE_INFO( "Unloaded textures for map: {}", mapData.name );
    }

    const Tileset * MapRenderer::FindTilesetForGid( int gid, const std::vector<Tileset> & tilesets ) const
    {
        for ( size_t i = tilesets.size() - 1; i >= 0; --i )
        {
            if ( gid >= tilesets[i].firstGid )
            {
                return &tilesets[i];
            }
        }

        SHNK_CORE_WARN( "GID {} has no corresponding tileset. Skipping.", gid );

        return nullptr;
    }

    void MapRenderer::DrawMap( MapData & mapData, const Camera & camera, const glm::mat4 & view, const glm::mat4 & projection )
    {
        m_RenderedTileCount = 0;

        m_MapTileShader->Use();

        m_MapTileShader->SetMat4( "view", view );
        m_MapTileShader->SetMat4( "projection", projection );

        Shinkiro::Platform::OpenGL::glBindVertexArray( m_CubeVAO );

        float layer_y_offset = 0.0f;
        for ( const auto & layer : mapData.layers )
        {
            const float tileBoundingRadius = 1.0f;

            for ( int y = 0; y < layer.height; ++y )
            {
                for ( int x = 0; x < layer.width; ++x )
                {
                    int gid = layer.data[y * layer.width + x];
                    if ( gid == 0 )
                    {
                        continue;
                    }

                    glm::vec3 tileCenter = { float( x ), layer_y_offset, float( y ) };
                    if ( !camera.GetFrustum().IsSphereInFrustum( tileCenter, tileBoundingRadius ) )
                    {
                        continue;
                    }

                    const Tileset * currentTileset = FindTilesetForGid( gid, mapData.tilesets );
                    if ( !currentTileset || currentTileset->columns == 0 || currentTileset->imageWidth == 0 || currentTileset->imageHeight == 0 )
                    {
                        if ( currentTileset )
                        {
                            SHNK_CORE_WARN( "Tileset with firstGid {} has invalid data. Skipping tile.", currentTileset->firstGid );
                        }
                        continue;
                    }

                    if ( !currentTileset || currentTileset->columns == 0 || currentTileset->imageWidth == 0 || currentTileset->imageHeight == 0 )
                    {
                        if ( !currentTileset )
                        {
                            SHNK_CORE_WARN( "GID {} has no corresponding tileset. Skipping.", gid );
                        }
                        else
                        {
                            SHNK_CORE_WARN( "Tileset with firstGid {} has invalid data. Skipping tile.", currentTileset->firstGid );
                        }
                        continue;
                    }

                    Shinkiro::Platform::OpenGL::glActiveTexture( GL_TEXTURE0 );
                    Shinkiro::Platform::OpenGL::glBindTexture( GL_TEXTURE_2D, currentTileset->textureID );
                    m_MapTileShader->SetInt( "texture_diffuse1", 0 );

                    int localTileId = gid - currentTileset->firstGid;
                    int tileX       = localTileId % currentTileset->columns;
                    int tileY       = localTileId / currentTileset->columns;

                    float uv_scale_x = float( currentTileset->tileWidth ) / currentTileset->imageWidth;
                    float uv_scale_y = float( currentTileset->tileHeight ) / currentTileset->imageHeight;

                    float uv_offset_x = float( tileX ) * uv_scale_x;
                    float uv_offset_y = 1.0f - ( float( tileY ) * uv_scale_y ) - uv_scale_y;

                    m_MapTileShader->SetVec2( "uvScale", glm::vec2( uv_scale_x, uv_scale_y ) );
                    m_MapTileShader->SetVec2( "uvOffset", glm::vec2( uv_offset_x, uv_offset_y ) );

                    glm::mat4 model = glm::mat4( 1.0f );
                    model           = glm::translate( model, glm::vec3( float( x ), layer_y_offset, float( y ) ) );
                    m_MapTileShader->SetMat4( "model", model );

                    Shinkiro::Platform::OpenGL::glDrawArrays( GL_TRIANGLES, 0, 36 );

                    m_RenderedTileCount++;
                }
            }

            layer_y_offset += 1.0f;
        }

        Shinkiro::Platform::OpenGL::glBindVertexArray( 0 );
    }

    void MapRenderer::DrawSkybox( const glm::mat4 & view, const glm::mat4 & projection )
    {
        if ( m_MapSkyboxShader->GetID() == 0 )
        {
            SHNK_CORE_WARN( "Skybox shader not initialized, skipping skybox rendering." );
            return;
        }

        Shinkiro::Platform::OpenGL::glDepthFunc( GL_LEQUAL );

        m_MapSkyboxShader->Use();
        m_MapSkyboxShader->SetMat4( "view", view );
        m_MapSkyboxShader->SetMat4( "projection", projection );
        m_MapSkyboxShader->SetInt( "skybox", 0 );

        Shinkiro::Platform::OpenGL::glBindVertexArray( m_SkyboxVAO );
        Shinkiro::Platform::OpenGL::glActiveTexture( GL_TEXTURE0 );
        Shinkiro::Platform::OpenGL::glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapTexture );

        Shinkiro::Platform::OpenGL::glDrawArrays( GL_TRIANGLES, 0, 36 );

        Shinkiro::Platform::OpenGL::glBindVertexArray( 0 );

        Shinkiro::Platform::OpenGL::glDepthFunc( GL_LESS );
    }

    void MapRenderer::DrawGrid( int mapWidth, int mapHeight, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection )
    {
        m_MapTileHighlighterShader->Use();

        m_MapTileHighlighterShader->SetMat4( "view", view );
        m_MapTileHighlighterShader->SetMat4( "projection", projection );

        m_MapTileHighlighterShader->SetVec4( "highlightColor", glm::vec4( 0.5f, 0.5f, 0.5f, 0.95f ) );

        Shinkiro::Platform::OpenGL::glBindVertexArray( m_CubeVAO );

        float y_pos     = layer_y_offset - 0.49f;
        float thickness = 0.05f;

        for ( int z = 0; z <= mapHeight; ++z )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( mapWidth ) / 2.0f - 0.5f, y_pos, float( z ) - 0.5f ) );
            model           = glm::scale( model, glm::vec3( float( mapWidth ), thickness, thickness ) );
            m_MapTileHighlighterShader->SetMat4( "model", model );

            Shinkiro::Platform::OpenGL::glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        for ( int x = 0; x <= mapWidth; ++x )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( x ) - 0.5f, y_pos, float( mapHeight ) / 2.0f - 0.5f ) );
            model           = glm::scale( model, glm::vec3( thickness, thickness, float( mapHeight ) ) );
            m_MapTileHighlighterShader->SetMat4( "model", model );

            Shinkiro::Platform::OpenGL::glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        Shinkiro::Platform::OpenGL::glBindVertexArray( 0 );
    }

    unsigned int MapRenderer::LoadCubemap( std::vector<std::string> faces )
    {
        unsigned int textureID;
        Shinkiro::Platform::OpenGL::glGenTextures( 1, &textureID );
        Shinkiro::Platform::OpenGL::glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

        int width, height, nrChannels;
        for ( unsigned int i = 0; i < faces.size(); i++ )
        {
            const auto textureData = Shinkiro::Core::App->GetBundleManager().GetAssetData( faces[i] );
            if ( !textureData.empty() )
            {
                unsigned char * data = stbi_load_from_memory(
                    textureData.data(),
                    static_cast<int>( textureData.size() ),
                    &width,
                    &height,
                    &nrChannels,
                    3
                );
                if ( data )
                {
                    Shinkiro::Platform::OpenGL::glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0,
                        GL_RGB,
                        width,
                        height,
                        0,
                        GL_RGB,
                        GL_UNSIGNED_BYTE,
                        data
                    );
                    stbi_image_free( data );

                    SHNK_CORE_INFO( "Loaded cubemap texture: {}", faces[i] );
                }
                else
                {
                    SHNK_CORE_ERROR( "Cubemap texture failed to load from memory: {}", faces[i] );
                }
            }
            else
            {
                SHNK_CORE_ERROR( "Failed to get asset data for cubemap texture: {}", faces[i] );
            }
        }

        Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        Shinkiro::Platform::OpenGL::glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        return textureID;
    }
}
