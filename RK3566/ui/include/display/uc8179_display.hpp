#pragma once

#include "display_driver.hpp"

class UC8179Display : public DisplayDriver
{
public:
   UC8179Display();

   bool initialize() override;

   void present(const Framebuffer &framebuffer) override;
};