#pragma once

#include "display_driver.hpp"

#include <SDL3/SDL.h>

class SDLDisplay : public DisplayDriver
{
public:
   SDLDisplay(uint16_t width, uint16_t height);
   SDLDisplay()
       : SDLDisplay(DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT)
   {
   }
   ~SDLDisplay();

   bool initialize() override;

   void present(const Framebuffer &framebuffer) override;

   bool processEvents() override;

private:
   SDL_Window *i_window = nullptr;
   SDL_Renderer *i_renderer = nullptr;
   SDL_Texture *i_texture = nullptr;

   uint16_t i_width;
   uint16_t i_height;
};