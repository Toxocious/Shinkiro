#include <Core/Util/Log.h>

#include <Core/Application.h>

#include <Core/Util/UpdateStatus.h>

int main()
{
    using namespace Shinkiro;

    const GLuint WIDTH = 1366, HEIGHT = 768;
    const char * APPLICATION_TITLE   = "Shinkiro Map Editor";
    const char * APPLICATION_VERSION = "v0.1";

    Shinkiro::Core::Log::Init();
    {
        Core::ApplicationState currentState = Core::ApplicationState::CREATION;

        bool running = true;

        while ( running )
        {
            switch ( currentState )
            {
                case Core::ApplicationState::CREATION:
                    SHNK_INFO( "Creating application" );
                    {
                        App          = new Core::Application();
                        currentState = Core::ApplicationState::INIT;
                    }
                    SHNK_INFO( "Application successfully created" );
                    break;

                case Core::ApplicationState::INIT:
                    SHNK_INFO( "Initializing application and modules" );
                    {
                        if ( App->Initialize( APPLICATION_TITLE, APPLICATION_VERSION, HEIGHT, WIDTH ) )
                        {
                            currentState = Core::ApplicationState::START;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to create application. Exiting" );
                            currentState = Core::ApplicationState::EXIT_ERROR;
                            break;
                        }
                    }
                    SHNK_INFO( "Application successfully initialized" );
                    break;

                case Core::ApplicationState::START:
                    SHNK_INFO( "Starting application modules" );
                    {
                        if ( App->Start() )
                        {
                            currentState = Core::ApplicationState::UPDATE;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to start application modules. Exiting" );
                            currentState = Core::ApplicationState::EXIT_ERROR;
                        }
                    }
                    SHNK_INFO( "Application modules successfully started" );

                    break;

                case Core::ApplicationState::UPDATE:

                    App->Update();
                    switch ( App->Update() )
                    {
                        case Shinkiro::Core::UpdateStatus::UPDATE_STOP:
                            currentState = Core::ApplicationState::FINISH;
                            break;
                        case Shinkiro::Core::UpdateStatus::UPDATE_ERROR:
                            currentState = Core::ApplicationState::EXIT_ERROR;
                            break;
                    }
                    break;

                case Core::ApplicationState::FINISH:
                    SHNK_INFO( "Cleaning up application before exiting" );
                    {
                        if ( App->CleanUp() )
                        {
                            currentState = Core::ApplicationState::EXIT;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to clean up the application. Exiting" );
                            currentState = Core::ApplicationState::EXIT_ERROR;
                        }
                    }
                    break;

                case Core::ApplicationState::EXIT:
                    SHNK_INFO( "Application has successfully exited" );
                    running = false;
                    break;

                case Core::ApplicationState::EXIT_ERROR:
                    SHNK_INFO( "Application has exited with errors" );
                    running = false;
                    break;
            }
        }
    }

    delete App;
    App = nullptr;

    return 0;
}
