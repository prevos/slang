/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdarg.h>

#include <string/stdstring.h>
#include <streams/file_stream.h>
#include <compat/fopen_utf8.h>

#include "verbosity.h"

static bool main_verbosity       = false;

void verbosity_enable(void)
{
   main_verbosity = true;
}

void verbosity_disable(void)
{
   main_verbosity = false;
}

bool verbosity_is_enabled(void)
{
   return main_verbosity;
}

bool *verbosity_get_ptr(void)
{
   return &main_verbosity;
}

void RARCH_LOG_V(const char *tag, const char *fmt, va_list ap)
{

      FILE *fp = stderr;

      if (fp)
      {
         fprintf(fp, "%s ",
               tag ? tag : "[INFO]");
         vfprintf(fp, fmt, ap);
         fflush(fp);
      }
}

void RARCH_LOG_BUFFER(uint8_t *data, size_t size)
{
   unsigned i, offset;
   int padding     = size % 16;
   uint8_t buf[16] = {0};

   RARCH_LOG("== %d-byte buffer ==================\n", size);

   for(i = 0, offset = 0; i < size; i++)
   {
      buf[offset] = data[i];
      offset++;

      if (offset == 16)
      {
         offset = 0;
         RARCH_LOG("%02x%02x%02x%02x%02x%02x%02x%02x  %02x%02x%02x%02x%02x%02x%02x%02x\n",
            buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
            buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
      }
   }
   if(padding)
   {
      for(i = padding; i < 16; i++)
         buf[i] = 0xff;
      RARCH_LOG("%02x%02x%02x%02x%02x%02x%02x%02x  %02x%02x%02x%02x%02x%02x%02x%02x\n",
         buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
         buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
   }
   RARCH_LOG("==================================\n");
}

void RARCH_LOG(const char *fmt, ...)
{
   va_list ap;

   if (!verbosity_is_enabled())
      return;

   va_start(ap, fmt);
   RARCH_LOG_V("[INFO]", fmt, ap);
   va_end(ap);
}

void RARCH_LOG_OUTPUT(const char *msg, ...)
{
   va_list ap;
   va_start(ap, msg);
   RARCH_LOG_OUTPUT_V("[INFO]", msg, ap);
   va_end(ap);
}

void RARCH_WARN(const char *fmt, ...)
{
   va_list ap;

   if (!verbosity_is_enabled())
      return;

   va_start(ap, fmt);
   RARCH_WARN_V("[WARN]", fmt, ap);
   va_end(ap);
}

void RARCH_ERR(const char *fmt, ...)
{
   va_list ap;

   if (!verbosity_is_enabled())
      return;

   va_start(ap, fmt);
   RARCH_ERR_V("[ERROR]", fmt, ap);
   va_end(ap);
}

