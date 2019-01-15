/*  RetroArch - A frontend fror libretro.
 *  Copyright (C) 2014-2018 - Ali Bouhlel
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

#ifndef __GLSLANG_PROCESS_H__
#define __GLSLANG_PROCESS_H__

#include <stdint.h>
#include <stdbool.h>
#include "retro_common_api.h"

#include "video_shader_parse.h"
#include "slang_semantics.h"
#include "slang_reflection.h"

RETRO_BEGIN_DECLS

bool slang_process(
      struct video_shader*   shader_info,
      unsigned               pass_number,
      enum rarch_shader_type dst_type,
      unsigned               version,
      const semantics_map_t* semantics_map,
      pass_semantics_t*      out);

RETRO_END_DECLS

#endif
