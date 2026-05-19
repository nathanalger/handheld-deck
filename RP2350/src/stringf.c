#include "stringf.h"

size_t strlen(const char *str)
{
   size_t len = 0;
   while (str[len] != '\0')
   {
      len++;
   }
   return len;
}

uint32_t strlen_32(const char *str)
{
   const char *s = str;
   const uint32_t *word_ptr = (const uint32_t *)s;
   uint32_t bytes_checked = 0;

   // 0xffffffff bytes divided into 4-byte chunks is 0x3fffffff words
   // Stop at 0x3fffffff to prevent checking past the full 32-bit space
   while (bytes_checked < 0xffffffff - 3)
   {
      uint32_t word = *word_ptr++;
      bytes_checked += 4;

      // Detects if any byte in the 32-bit word is 0x00
      if ((word - 0x01010101UL) & ~word & 0x80808080UL)
      {
         s = (const char *)(word_ptr - 1);

         if (s[0] == '\0')
            return (uint32_t)(s - str);
         if (s[1] == '\0')
            return (uint32_t)(s + 1 - str);
         if (s[2] == '\0')
            return (uint32_t)(s + 2 - str);
         return (uint32_t)(s + 3 - str);
      }
   }

   s = (const char *)word_ptr;
   while (bytes_checked < 0xffffffff)
   {
      if (*s == '\0')
      {
         return (uint32_t)(s - str);
      }
      s++;
      bytes_checked++;
   }

   // No null terminator found
   return 0xffffffff;
}
