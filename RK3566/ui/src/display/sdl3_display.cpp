#include "sdl3_display.hpp"

SDLDisplay::SDLDisplay(uint16_t width, uint16_t height)
    : i_width(width),
      i_height(height)
{
}

SDLDisplay::~SDLDisplay()
{
   if (i_texture)
      SDL_DestroyTexture(i_texture);

   if (i_renderer)
      SDL_DestroyRenderer(i_renderer);

   if (i_window)
      SDL_DestroyWindow(i_window);

   SDL_Quit();
}

bool SDLDisplay::initialize()
{
   if (!SDL_Init(SDL_INIT_VIDEO))
      return false;

   i_window = SDL_CreateWindow(
       "SaDOS",
       i_width,
       i_height,
       0);

   if (!i_window)
      return false;

   i_renderer = SDL_CreateRenderer(
       i_window,
       nullptr);

   if (!i_renderer)
      return false;

   i_texture = SDL_CreateTexture(
       i_renderer,
       SDL_PIXELFORMAT_RGBA8888,
       SDL_TEXTUREACCESS_STREAMING,
       i_width,
       i_height);

   if (!i_texture)
      return false;

   return true;
}

void SDLDisplay::present(const Framebuffer &framebuffer)
{
   uint32_t *pixels = nullptr;
   int pitch = 0;

   if (!SDL_LockTexture(
           i_texture,
           nullptr,
           reinterpret_cast<void **>(&pixels),
           &pitch))
   {
      return;
   }

   for (uint32_t y = 0; y < framebuffer.height(); y++)
   {
      auto *row = reinterpret_cast<uint8_t *>(pixels) +
                  y * pitch;

      for (uint32_t x = 0; x < framebuffer.width(); x++)
      {
         PixelColor color = framebuffer.getPixel(
             x,
             y);

         uint32_t pixel =
             color == PixelColor::Black
                 ? 0xFF000000
                 : 0xFFFFFFFF;

         reinterpret_cast<uint32_t *>(row)[x] = pixel;
      }
   }

   SDL_UnlockTexture(i_texture);

   SDL_RenderClear(i_renderer);

   SDL_RenderTexture(
       i_renderer,
       i_texture,
       nullptr,
       nullptr);

   SDL_RenderPresent(i_renderer);
}

bool SDLDisplay::processEvents()
{
   SDL_Event event;

   while (SDL_PollEvent(&event))
   {
      if (event.type == SDL_EVENT_QUIT)
         return false;
   }

   return true;
}