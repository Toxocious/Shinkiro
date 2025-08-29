#include <Log/Log.h>

#include <Editor/Application.h>

#include <Core/Util/UpdateStatus.h>

int main()
{
    const GLuint WIDTH = 1366, HEIGHT = 768;
    const char * APPLICATION_TITLE   = "Shinkiro Map Editor";
    const char * APPLICATION_VERSION = SHINKIRO_VERSION;

    Shinkiro::Logger::Log::Initialize();
    {
        Shinkiro::ApplicationState currentState = Shinkiro::ApplicationState::CREATION;

        bool running = true;

        while ( running )
        {
            switch ( currentState )
            {
                case Shinkiro::ApplicationState::CREATION:
                    SHNK_INFO( "Creating application" );
                    {
                        App          = new Shinkiro::Application();
                        currentState = Shinkiro::ApplicationState::INIT;
                    }
                    SHNK_INFO( "Application successfully created" );
                    break;

                case Shinkiro::ApplicationState::INIT:
                    SHNK_INFO( "Initializing application and modules" );
                    {
                        if ( App->Initialize( APPLICATION_TITLE, APPLICATION_VERSION, HEIGHT, WIDTH ) )
                        {
                            currentState = Shinkiro::ApplicationState::START;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to create application. Exiting" );
                            currentState = Shinkiro::ApplicationState::EXIT_ERROR;
                            break;
                        }
                    }
                    SHNK_INFO( "Application successfully initialized" );
                    break;

                case Shinkiro::ApplicationState::START:
                    SHNK_INFO( "Starting application modules" );
                    {
                        if ( App->Start() )
                        {
                            currentState = Shinkiro::ApplicationState::UPDATE;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to start application modules. Exiting" );
                            currentState = Shinkiro::ApplicationState::EXIT_ERROR;
                        }
                    }
                    SHNK_INFO( "Application modules successfully started" );

                    break;

                case Shinkiro::ApplicationState::UPDATE:

                    App->Update();
                    switch ( App->Update() )
                    {
                        case Shinkiro::Core::UpdateStatus::UPDATE_STOP:
                            currentState = Shinkiro::ApplicationState::FINISH;
                            break;
                        case Shinkiro::Core::UpdateStatus::UPDATE_ERROR:
                            currentState = Shinkiro::ApplicationState::EXIT_ERROR;
                            break;
                    }
                    break;

                case Shinkiro::ApplicationState::FINISH:
                    SHNK_INFO( "Cleaning up application before exiting" );
                    {
                        if ( App->CleanUp() )
                        {
                            currentState = Shinkiro::ApplicationState::EXIT;
                        }
                        else
                        {
                            SHNK_ERROR( "Failed to clean up the application. Exiting" );
                            currentState = Shinkiro::ApplicationState::EXIT_ERROR;
                        }
                    }
                    break;

                case Shinkiro::ApplicationState::EXIT:
                    SHNK_INFO( "Application has successfully exited" );
                    running = false;
                    break;

                case Shinkiro::ApplicationState::EXIT_ERROR:
                    SHNK_INFO( "Application has exited with errors" );
                    running = false;
                    break;
            }
        }
    }

    delete App;

    return 0;
}
