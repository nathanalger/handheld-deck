#include "app.hpp"

App::App(Renderer &renderer, DisplayDriver &display)
    : i_renderer(renderer),
      i_display(display)
{
}

void App::run()
{
   if (!i_display.initialize())
      return;

   bool running = true;

   while (running)
   {
      running = i_display.processEvents();

      i_renderer.clear();
      i_renderer.drawRect(0, 0, 500, 500, PixelColor::Black);

      i_display.present(i_renderer.framebuffer());
   }
}