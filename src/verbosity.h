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

#ifndef __RARCH_VERBOSITY_H
#define __RARCH_VERBOSITY_H

#include <stdarg.h>

#include <boolean.h>
#include <retro_common_api.h>

RETRO_BEGIN_DECLS

bool verbosity_is_enabled(void);

void verbosity_enable(void);

void verbosity_disable(void);

bool *verbosity_get_ptr(void);

void RARCH_LOG_V(const char *tag, const char *fmt, va_list ap);
void RARCH_LOG(const char *fmt, ...);
void RARCH_LOG_BUFFER(uint8_t *buffer, size_t size);
void RARCH_LOG_OUTPUT(const char *msg, ...);
void RARCH_WARN(const char *fmt, ...);
void RARCH_ERR(const char *fmt, ...);

#define RARCH_LOG_OUTPUT_V RARCH_LOG_V
#define RARCH_WARN_V RARCH_LOG_V
#define RARCH_ERR_V RARCH_LOG_V

RETRO_END_DECLS

#endif
