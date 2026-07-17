#include "app.hpp"
#include "framebuffer.hpp"
#include "renderer.hpp"
#include "uc8179_display.hpp"

int main()
{
   Framebuffer framebuffer;
   Renderer renderer(framebuffer);

   UC8179Display display;

   App app(renderer, display);
   app.run();

   return 0;
}