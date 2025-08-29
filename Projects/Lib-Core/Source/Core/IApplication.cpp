#include <Core/IApplication.h>

namespace Shinkiro::Core
{
    // Define the global application pointer. It will be assigned by the
    // concrete Application class in the main executable.
    CORE_API IApplication * App = nullptr;
}
