#pragma once

#include "framebuffer.hpp"
#include "renderer.hpp"
#include "display_driver.hpp"

class App
{
public:
   App(Renderer &renderer, DisplayDriver &display);
   void run();

private:
   Renderer &i_renderer;
   DisplayDriver &i_display;
};