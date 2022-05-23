#include "kronic_app.h"

#include "platform/window_glfw.h"

KronicApplication::KronicApplication()
    : Application(MakeRef<WindowGLFW>(640, 480))
{
}

KronicApplication::~KronicApplication()
{
}
