#include <Renderer/_Common.h>

#include <Renderer/Map/MapManager.h>

#include <Core/IApplication.h>
#include <Core/Util/FileSystem.h>

#include <Asset/AssetBundleManager.h>

#include <Log/Log.h>

namespace Shinkiro::Renderer
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
        glDeleteVertexArrays( 1, &m_cubeVAO );
        glDeleteBuffers( 1, &m_cubeVBO );

        glDeleteVertexArrays( 1, &m_skyboxVAO );
        glDeleteBuffers( 1, &m_skyboxVBO );
        glDeleteTextures( 1, &m_cubemapTexture );
    }

    bool Renderer::init()
    {
        /**
         * Create the generic map tile shader program.
         */
        {
            m_MapTileShader = std::make_unique<Shader>(
                "Shaders/MapTile/maptile.vertex.glsl",
                "Shaders/MapTile/maptile.fragment.glsl"
            );

            if ( m_MapTileShader->getID() == 0 )
            {
                SHNK_CORE_ERROR( "Renderer failed to initialize MapTile shader!" );
                return false;
            }

            m_MapTileShader->use();
            m_MapTileShader->setMat4( "model", glm::mat4( 1.0f ) );
            m_MapTileShader->setMat4( "view", glm::mat4( 1.0f ) );
            m_MapTileShader->setMat4( "projection", glm::mat4( 1.0f ) );
            m_MapTileShader->setVec3( "objectColor", glm::vec3( 1.0f, 1.0f, 1.0f ) );
            m_MapTileShader->setVec2( "uvOffset", glm::vec2( 0.0f ) );
            m_MapTileShader->setVec2( "uvScale", glm::vec2( 1.0f ) );
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

            // Use your Shader class instead of raw program creation
            m_MapTileHighlighterShader = std::make_unique<Shader>(
                MapTileVertexSource,
                MapTileFragmentSource
            );

            if ( m_MapTileHighlighterShader->getID() == 0 )
            {
                SHNK_CORE_ERROR( "Renderer failed to initialize MapTileHighlight shader!" );
                return false;
            }

            // Example: set some default uniforms (no need for cached locations anymore)
            m_MapTileHighlighterShader->use();
            m_MapTileHighlighterShader->setMat4( "model", glm::mat4( 1.0f ) );
            m_MapTileHighlighterShader->setMat4( "view", glm::mat4( 1.0f ) );
            m_MapTileHighlighterShader->setMat4( "projection", glm::mat4( 1.0f ) );
            m_MapTileHighlighterShader->setVec4( "highlightColor", glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

            // Setup vertex data for a unit cube
            glGenVertexArrays( 1, &m_cubeVAO );
            glGenBuffers( 1, &m_cubeVBO );
            glBindVertexArray( m_cubeVAO );
            glBindBuffer( GL_ARRAY_BUFFER, m_cubeVBO );
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

            // Use the Shader class
            m_MapSkyboxShader = std::make_unique<Shader>(
                MapSkyBoxVertexSource,
                MapSkyBoxFragmentSource
            );

            if ( m_MapSkyboxShader->getID() == 0 )
            {
                SHNK_CORE_ERROR( "Renderer failed to initialize MapSkybox shader!" );
                return false;
            }

            glGenVertexArrays( 1, &m_skyboxVAO );
            glGenBuffers( 1, &m_skyboxVBO );
            glBindVertexArray( m_skyboxVAO );
            glBindBuffer( GL_ARRAY_BUFFER, m_skyboxVBO );
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
            m_cubemapTexture = loadCubemap( faces );
        }

        glBindVertexArray( 0 );

        return true;
    }

    void Renderer::DrawHighlight( int x, int y, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection )
    {
        // Use the highlight shader
        m_MapTileHighlighterShader->use();

        // Set matrices
        m_MapTileHighlighterShader->setMat4( "view", view );
        m_MapTileHighlighterShader->setMat4( "projection", projection );

        // Set highlight color
        m_MapTileHighlighterShader->setVec4( "highlightColor", glm::vec4( 0.2f, 0.2f, 1.0f, 0.5f ) );

        // Compute model matrix
        glm::mat4 model = glm::mat4( 1.0f );
        model           = glm::translate( model, glm::vec3( static_cast<float>( x ), layer_y_offset + 0.51f, static_cast<float>( y ) ) );
        model           = glm::scale( model, glm::vec3( 1.0f, 0.1f, 1.0f ) );

        m_MapTileHighlighterShader->setMat4( "model", model );

        // Enable blending for transparency
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // Disable writing to the depth buffer for transparency
        glDepthMask( GL_FALSE );

        // Draw the plane
        glBindVertexArray( m_cubeVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        // Reset state
        glDepthMask( GL_TRUE );
        glBindVertexArray( 0 );
        glDisable( GL_BLEND ); // optional: reset blend state
    }

    void Renderer::loadTilesetTextures( MapData & mapData )
    {
        stbi_set_flip_vertically_on_load( true ); // Match OpenGL's coordinate system

        for ( auto & tileset : mapData.tilesets )
        {
            glGenTextures( 1, &tileset.textureID );
            glBindTexture( GL_TEXTURE_2D, tileset.textureID );

            // Set texture wrapping and filtering options
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

            // Use nearest for pixel art
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            int width, height, nrChannels;

            // IMPORTANT: Tiled may use relative paths. You might need to prepend a base path.
            unsigned char * data = stbi_load( tileset.imageSource.c_str(), &width, &height, &nrChannels, 0 );
            if ( data )
            {
                GLenum format = ( nrChannels == 4 ) ? GL_RGBA : GL_RGB;
                glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
                glGenerateMipmap( GL_TEXTURE_2D );
                std::cout << "Loaded texture: " << tileset.imageSource << std::endl;
            }
            else
            {
                std::cerr << "Failed to load texture: " << tileset.imageSource << std::endl;
            }
            stbi_image_free( data );
        }

        stbi_set_flip_vertically_on_load( false );
    }

    const Tileset * Renderer::findTilesetForGid( int gid, const std::vector<Tileset> & tilesets ) const
    {
        for ( int i = tilesets.size() - 1; i >= 0; --i )
        {
            if ( gid >= tilesets[i].firstGid )
            {
                // Return a pointer to the found tileset
                return &tilesets[i];
            }
        }

        SHNK_CORE_WARN( "GID {} has no corresponding tileset. Skipping.", gid );
        return nullptr;
    }

    void Renderer::drawMap( MapData & mapData, const Camera & camera, const glm::mat4 & view, const glm::mat4 & projection )
    {
        m_renderedTileCount = 0;

        // Activate the shader
        m_MapTileShader->use();

        // Set the view and projection matrices once for all tiles
        m_MapTileShader->setMat4( "view", view );
        m_MapTileShader->setMat4( "projection", projection );

        // Bind the cube VAO
        glBindVertexArray( m_cubeVAO );

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
                        continue; // Skip empty tiles
                    }

                    glm::vec3 tileCenter = { float( x ), layer_y_offset, float( y ) };
                    if ( !camera.m_Frustum.IsSphereInFrustum( tileCenter, tileBoundingRadius ) )
                    {
                        continue; // Cull this tile
                    }

                    // Find the tileset
                    const Tileset * currentTileset = nullptr;
                    for ( int i = mapData.tilesets.size() - 1; i >= 0; --i )
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

                    // Bind the texture
                    glActiveTexture( GL_TEXTURE0 );
                    glBindTexture( GL_TEXTURE_2D, currentTileset->textureID );
                    m_MapTileShader->setInt( "texture_diffuse1", 0 );

                    // Calculate UV scale and offset
                    int localTileId = gid - currentTileset->firstGid;
                    int tileX       = localTileId % currentTileset->columns;
                    int tileY       = localTileId / currentTileset->columns;

                    float uv_scale_x = float( currentTileset->tileWidth ) / currentTileset->imageWidth;
                    float uv_scale_y = float( currentTileset->tileHeight ) / currentTileset->imageHeight;

                    float uv_offset_x = float( tileX ) * uv_scale_x;
                    float uv_offset_y = 1.0f - ( float( tileY ) * uv_scale_y ) - uv_scale_y;

                    m_MapTileShader->setVec2( "uvScale", glm::vec2( uv_scale_x, uv_scale_y ) );
                    m_MapTileShader->setVec2( "uvOffset", glm::vec2( uv_offset_x, uv_offset_y ) );

                    // Calculate model matrix and draw
                    glm::mat4 model = glm::mat4( 1.0f );
                    model           = glm::translate( model, glm::vec3( float( x ), layer_y_offset, float( y ) ) );
                    m_MapTileShader->setMat4( "model", model );

                    glDrawArrays( GL_TRIANGLES, 0, 36 );
                    m_renderedTileCount++;
                }
            }

            layer_y_offset += 1.0f;
        }

        glBindVertexArray( 0 );
    }

    void Renderer::drawSkybox( const glm::mat4 & view, const glm::mat4 & projection )
    {
        // Change depth function so skybox passes depth test when values are equal to depth buffer
        glDepthFunc( GL_LEQUAL );

        // Use the skybox shader
        m_MapSkyboxShader->use();
        m_MapSkyboxShader->setMat4( "view", view );
        m_MapSkyboxShader->setMat4( "projection", projection );
        m_MapSkyboxShader->setInt( "skybox", 0 ); // Bind sampler to texture unit 0

        // Bind skybox VAO and cubemap texture
        glBindVertexArray( m_skyboxVAO );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapTexture );

        // Draw the skybox cube
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glBindVertexArray( 0 );

        // Restore default depth function
        glDepthFunc( GL_LESS );
    }

    void Renderer::drawGrid( int mapWidth, int mapHeight, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection )
    {
        // Use the highlight shader
        m_MapTileHighlighterShader->use();

        // Set view and projection matrices
        m_MapTileHighlighterShader->setMat4( "view", view );
        m_MapTileHighlighterShader->setMat4( "projection", projection );

        // Set grid color
        m_MapTileHighlighterShader->setVec4( "highlightColor", glm::vec4( 0.5f, 0.5f, 0.5f, 0.95f ) );

        glBindVertexArray( m_cubeVAO );

        // Calculate Y position for the current layer
        float y_pos     = layer_y_offset - 0.49f;
        float thickness = 0.05f;

        // Draw horizontal lines
        for ( int z = 0; z <= mapHeight; ++z )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( mapWidth ) / 2.0f - 0.5f, y_pos, float( z ) - 0.5f ) );
            model           = glm::scale( model, glm::vec3( float( mapWidth ), thickness, thickness ) );
            m_MapTileHighlighterShader->setMat4( "model", model );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        // Draw vertical lines
        for ( int x = 0; x <= mapWidth; ++x )
        {
            glm::mat4 model = glm::mat4( 1.0f );
            model           = glm::translate( model, glm::vec3( float( x ) - 0.5f, y_pos, float( mapHeight ) / 2.0f - 0.5f ) );
            model           = glm::scale( model, glm::vec3( thickness, thickness, float( mapHeight ) ) );
            m_MapTileHighlighterShader->setMat4( "model", model );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        glBindVertexArray( 0 );
    }

    unsigned int Renderer::loadCubemap( std::vector<std::string> faces )
    {
        unsigned int textureID;
        glGenTextures( 1, &textureID );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

        int width, height, nrChannels;
        for ( unsigned int i = 0; i < faces.size(); i++ )
        {
            // Force loading the image as 3-channel RGB, regardless of its original format.
            unsigned char * data = stbi_load( faces[i].c_str(), &width, &height, &nrChannels, 3 );
            if ( data )
            {
                // Now that we've forced 3 channels, the format is always GL_RGB.
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
