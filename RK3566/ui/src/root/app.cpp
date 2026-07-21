#include "app.hpp"
#include "fonts.hpp"

App::App(Renderer &renderer, DisplayDriver &display)
    : i_renderer(renderer),
      i_display(display)
{
}

void App::run()
{
   if (!i_display.initialize())
      return;

   BitmapFont font(1);

   bool running = true;

   while (running)
   {
      running = i_display.processEvents();
      i_renderer.clear();

      i_renderer.drawRect(0, 0, 1000, 40, PixelColor::Black);
      i_renderer.drawRect(10, 10, 20, 20, PixelColor::White);

      i_renderer.drawText(200, 200, "HELLO", font, PixelColor::Black, 1);

      i_display.present(i_renderer.framebuffer());
   }
}