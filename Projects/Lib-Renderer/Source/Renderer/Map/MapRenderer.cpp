#include <Renderer/_Common.h>

#include <Renderer/Map/MapRenderer.h>

#include <Core/IApplication.h>
#include <Core/Util/FileSystem.h>

#include <Asset/AssetBundleManager.h>

#include <Log/Log.h>

namespace Shinkiro::Renderer
{
    MapRenderer::MapRenderer()
    {
    }

    MapRenderer::~MapRenderer()
    {
        glDeleteVertexArrays( 1, &m_CubeVAO );
        glDeleteBuffers( 1, &m_CubeVBO );

        glDeleteVertexArrays( 1, &m_SkyboxVAO );
        glDeleteBuffers( 1, &m_SkyboxVBO );
        glDeleteTextures( 1, &m_cubemapTexture );
    }

    bool MapRenderer::Initialize()
    {
        /**
         * Create the generic map tile shader program.
         */
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
            m_MapTileShader->SetVec3( "objectColor", glm::vec3( 1.0f, 1.0f, 1.0f ) );
            m_MapTileShader->SetVec2( "uvOffset", glm::vec2( 0.0f ) );
            m_MapTileShader->SetVec2( "uvScale", glm::vec2( 1.0f ) );
        }

        /**
         * Create the map tile highlight shader program.
         */
        {
            const auto MapTileHighlightVertexShader = Shinkiro::Core::App->GetBundleManager().GetAssetData(
                "Shaders/MapTileHighlight/maptilehighlight.vertex.glsl"
            );
            const auto MapTileFragmentShader = Shinkiro::Core::App->GetBundleManager().GetAssetData(
                "Shaders/MapTileHighlight/maptilehighlight.fragment.glsl"
            );

            std::string MapTileVertexSource(
                reinterpret_cast<const char *>( MapTileHighlightVertexShader.data() ),
                MapTileHighlightVertexShader.size()
            );
            std::string MapTileFragmentSource(
                reinterpret_cast<const char *>( MapTileFragmentShader.data() ),
                MapTileFragmentShader.size()
            );

            m_MapTileHighlighterShader = std::make_unique<Shader>(
                MapTileVertexSource,
                MapTileFragmentSource
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
            glGenVertexArrays( 1, &m_CubeVAO );
            glGenBuffers( 1, &m_CubeVBO );
            glBindVertexArray( m_CubeVAO );
            glBindBuffer( GL_ARRAY_BUFFER, m_CubeVBO );
            glBufferData( GL_ARRAY_BUFFER, sizeof( m_TileVertices ), m_TileVertices, GL_STATIC_DRAW );

            // Position attribute
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) 0 );
            glEnableVertexAttribArray( 0 );

            // Texture coord attribute
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) ( 3 * sizeof( float ) ) );
            glEnableVertexAttribArray( 1 );
        }

        /**
         * Create the skybox shader program.
         */
        {
            const auto MapSkyBoxVertexShader   = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Shaders/MapSkyBox/maptilehighlight.vertex.glsl" );
            const auto MapSkyboxFragmentShader = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Shaders/MapSkyBox/maptilehighlight.fragment.glsl" );

            std::string MapSkyBoxVertexSource(
                reinterpret_cast<const char *>( MapSkyBoxVertexShader.data() ),
                MapSkyBoxVertexShader.size()
            );
            std::string MapSkyBoxFragmentSource(
                reinterpret_cast<const char *>( MapSkyboxFragmentShader.data() ),
                MapSkyboxFragmentShader.size()
            );

            m_MapSkyboxShader = std::make_unique<Shader>(
                MapSkyBoxVertexSource,
                MapSkyBoxFragmentSource
            );

            if ( m_MapSkyboxShader->GetID() == 0 )
            {
                SHNK_CORE_ERROR( "Renderer failed to initialize MapSkybox shader!" );
                return false;
            }

            glGenVertexArrays( 1, &m_SkyboxVAO );
            glGenBuffers( 1, &m_SkyboxVBO );
            glBindVertexArray( m_SkyboxVAO );
            glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBO );
            glBufferData( GL_ARRAY_BUFFER, sizeof( m_SkyboxVertices ), &m_SkyboxVertices, GL_STATIC_DRAW );

            glEnableVertexAttribArray( 0 );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void * ) 0 );

            std::filesystem::path exeDir    = Shinkiro::Core::FileSystem::GetExecutableDirectory();
            std::filesystem::path skyboxDir = exeDir / "Assets" / "Textures" / "Skybox";

            // The order must be: Right, Left, Top, Bottom, Front, Back.
            std::vector<std::string> faces = {
                ( skyboxDir / "Skybox_Right.png" ).string(), // Right
                ( skyboxDir / "Skybox_Left.png" ).string(),  // Left
                ( skyboxDir / "Skybox_Up.png" ).string(),    // Top
                ( skyboxDir / "Skybox_Down.png" ).string(),  // Bottom
                ( skyboxDir / "Skybox_Front.png" ).string(), // Front
                ( skyboxDir / "Skybox_Back.png" ).string()   // Back
            };
            m_cubemapTexture = LoadCubemap( faces );
        }

        glBindVertexArray( 0 );

        return true;
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

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glDepthMask( GL_FALSE );

        glBindVertexArray( m_CubeVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glDepthMask( GL_TRUE );
        glBindVertexArray( 0 );
        glDisable( GL_BLEND );
    }

    void MapRenderer::LoadTilesetTextures( MapData & mapData )
    {
        stbi_set_flip_vertically_on_load( true );

        for ( auto & tileset : mapData.tilesets )
        {
            glGenTextures( 1, &tileset.textureID );
            glBindTexture( GL_TEXTURE_2D, tileset.textureID );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            int width, height, nrChannels;

            unsigned char * data = stbi_load( tileset.imageSource.c_str(), &width, &height, &nrChannels, 0 );
            if ( data )
            {
                GLenum format = ( nrChannels == 4 ) ? GL_RGBA : GL_RGB;
                glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
                glGenerateMipmap( GL_TEXTURE_2D );

                SHNK_CORE_INFO( "Loaded texture: {}", tileset.imageSource );
            }
            else
            {
                SHNK_CORE_ERROR( "Failed to load texture: {}", tileset.imageSource );
            }

            stbi_image_free( data );
        }

        stbi_set_flip_vertically_on_load( false );
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

        glBindVertexArray( m_CubeVAO );

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
                    if ( !camera.m_Frustum.IsSphereInFrustum( tileCenter, tileBoundingRadius ) )
                    {
                        continue;
                    }

                    const Tileset * currentTileset = nullptr;
                    for ( size_t i = mapData.tilesets.size() - 1; i >= 0; --i )
                    {
                        if ( gid >= mapData.tilesets[i].firstGid )
                        {
                            currentTileset = &mapData.tilesets[i];
                            break;
                        }
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

                    glActiveTexture( GL_TEXTURE0 );
                    glBindTexture( GL_TEXTURE_2D, currentTileset->textureID );
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

                    glDrawArrays( GL_TRIANGLES, 0, 36 );

                    m_RenderedTileCount++;
                }
            }

            layer_y_offset += 1.0f;
        }

        glBindVertexArray( 0 );
    }

    void MapRenderer::DrawSkybox( const glm::mat4 & view, const glm::mat4 & projection )
    {
        glDepthFunc( GL_LEQUAL );

        m_MapSkyboxShader->Use();
        m_MapSkyboxShader->SetMat4( "view", view );
        m_MapSkyboxShader->SetMat4( "projection", projection );
        m_MapSkyboxShader->SetInt( "skybox", 0 );

        glBindVertexArray( m_SkyboxVAO );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapTexture );

        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glBindVertexArray( 0 );

        glDepthFunc( GL_LESS );
    }

    void MapRenderer::DrawGrid( int mapWidth, int mapHeight, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection )
    {
        m_MapTileHighlighterShader->Use();

        m_MapTileHighlighterShader->SetMat4( "view", view );
        m_MapTileHighlighterShader->SetMat4( "projection", projection );

        m_MapTileHighlighterShader->SetVec4( "highlightColor", glm::vec4( 0.5f, 0.5f, 0.5f, 0.95f ) );

        glBindVertexArray( m_CubeVAO );

        float y_pos     = layer_y_offset - 0.49f;
        float thickness = 0.05f;

        for ( int z = 0; z <= mapHeight; ++z )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( mapWidth ) / 2.0f - 0.5f, y_pos, float( z ) - 0.5f ) );
            model           = glm::scale( model, glm::vec3( float( mapWidth ), thickness, thickness ) );
            m_MapTileHighlighterShader->SetMat4( "model", model );

            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        for ( int x = 0; x <= mapWidth; ++x )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( x ) - 0.5f, y_pos, float( mapHeight ) / 2.0f - 0.5f ) );
            model           = glm::scale( model, glm::vec3( thickness, thickness, float( mapHeight ) ) );
            m_MapTileHighlighterShader->SetMat4( "model", model );

            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        glBindVertexArray( 0 );
    }

    unsigned int MapRenderer::LoadCubemap( std::vector<std::string> faces )
    {
        unsigned int textureID;
        glGenTextures( 1, &textureID );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

        int width, height, nrChannels;
        for ( unsigned int i = 0; i < faces.size(); i++ )
        {
            unsigned char * data = stbi_load( faces[i].c_str(), &width, &height, &nrChannels, 3 );
            if ( data )
            {
                glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
                stbi_image_free( data );

                SHNK_CORE_INFO( "Loaded cubemap texture: {}", faces[i] );
            }
            else
            {
                SHNK_CORE_ERROR( "Cubemap texture failed to load at path: {}", faces[i] );
            }
        }

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        return textureID;
    }
}
