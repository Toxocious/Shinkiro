#include <Log/Log.h>

#include <Platform/Application.h>

#include <Core/Util/UpdateStatus.h>

int main()
{
    const GLuint WIDTH = 1366, HEIGHT = 768;
    const char * APPLICATION_TITLE   = "Shinkiro Map Editor";
    const char * APPLICATION_VERSION = "v0.1";

    Shinkiro::Logger::Log::Init();
    {
        Shinkiro::Platform::ApplicationState currentState = Shinkiro::Platform::ApplicationState::CREATION;

        bool running = true;

        while ( running )
        {
            switch ( currentState )
            {
                case Shinkiro::Platform::ApplicationState::CREATION:
                    SHNK_INFO( "Creating application" );
                    {
                        App          = new Shinkiro::Platform::Application();
                        currentState = Shinkiro::Platform::ApplicationState::INIT;
                    }
                    SHNK_INFO( "Application successfully created" );
                    break;

                case Shinkiro::Platform::ApplicationState::INIT:
                    SHNK_INFO( "Initializing application and modules" );
                    {
                        if ( App->Initialize( APPLICATION_TITLE, APPLICATION_VERSION, HEIGHT, WIDTH ) )
                        {
                            currentState = Shinkiro::Platform::ApplicationState::START;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to create application. Exiting" );
                            currentState = Shinkiro::Platform::ApplicationState::EXIT_ERROR;
                            break;
                        }
                    }
                    SHNK_INFO( "Application successfully initialized" );
                    break;

                case Shinkiro::Platform::ApplicationState::START:
                    SHNK_INFO( "Starting application modules" );
                    {
                        if ( App->Start() )
                        {
                            currentState = Shinkiro::Platform::ApplicationState::UPDATE;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to start application modules. Exiting" );
                            currentState = Shinkiro::Platform::ApplicationState::EXIT_ERROR;
                        }
                    }
                    SHNK_INFO( "Application modules successfully started" );

                    break;

                case Shinkiro::Platform::ApplicationState::UPDATE:

                    App->Update();
                    switch ( App->Update() )
                    {
                        case Shinkiro::Platform::UpdateStatus::UPDATE_STOP:
                            currentState = Shinkiro::Platform::ApplicationState::FINISH;
                            break;
                        case Shinkiro::Platform::UpdateStatus::UPDATE_ERROR:
                            currentState = Shinkiro::Platform::ApplicationState::EXIT_ERROR;
                            break;
                    }
                    break;

                case Shinkiro::Platform::ApplicationState::FINISH:
                    SHNK_INFO( "Cleaning up application before exiting" );
                    {
                        if ( App->CleanUp() )
                        {
                            currentState = Shinkiro::Platform::ApplicationState::EXIT;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to clean up the application. Exiting" );
                            currentState = Shinkiro::Platform::ApplicationState::EXIT_ERROR;
                        }
                    }
                    break;

                case Shinkiro::Platform::ApplicationState::EXIT:
                    SHNK_INFO( "Application has successfully exited" );
                    running = false;
                    break;

                case Shinkiro::Platform::ApplicationState::EXIT_ERROR:
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
