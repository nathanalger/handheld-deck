#include "app.hpp"
#include "framebuffer.hpp"
#include "renderer.hpp"
#include "sdl3_display.hpp"

int main()
{
   Framebuffer framebuffer;
   Renderer renderer(framebuffer);

   SDLDisplay display;

   App app(renderer, display);
   app.run();

   return 0;
}