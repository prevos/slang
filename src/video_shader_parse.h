/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
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

#ifndef __VIDEO_SHADER_PARSE_H
#define __VIDEO_SHADER_PARSE_H

#include <stdbool.h>
#include "retro_common_api.h"

#define PATH_MAX_LENGTH 4096

typedef void config_file_np;
typedef config_file_np *config_file_p;

RETRO_BEGIN_DECLS

#ifndef GFX_MAX_SHADERS
#define GFX_MAX_SHADERS 26
#endif

#ifndef GFX_MAX_TEXTURES
#define GFX_MAX_TEXTURES 8
#endif

#ifndef GFX_MAX_PARAMETERS
#define GFX_MAX_PARAMETERS 128
#endif

#ifndef GFX_MAX_FRAME_HISTORY
#define GFX_MAX_FRAME_HISTORY 128
#endif

enum rarch_shader_type
{
   RARCH_SHADER_NONE = 0,
   RARCH_SHADER_CG,
   RARCH_SHADER_GLSL,
   RARCH_SHADER_SLANG,
   RARCH_SHADER_METAL
};

enum gfx_scale_type
{
   RARCH_SCALE_INPUT = 0,
   RARCH_SCALE_ABSOLUTE,
   RARCH_SCALE_VIEWPORT
};

enum
{
   RARCH_FILTER_UNSPEC = 0,
   RARCH_FILTER_LINEAR,
   RARCH_FILTER_NEAREST,
   RARCH_FILTER_MAX
};

enum gfx_wrap_type
{
   RARCH_WRAP_BORDER = 0, /* Kinda deprecated, but keep as default.
                             Will be translated to EDGE in GLES. */
   RARCH_WRAP_DEFAULT = RARCH_WRAP_BORDER,
   RARCH_WRAP_EDGE,
   RARCH_WRAP_REPEAT,
   RARCH_WRAP_MIRRORED_REPEAT,
   RARCH_WRAP_MAX
};

struct gfx_fbo_scale
{
   enum gfx_scale_type type_x;
   enum gfx_scale_type type_y;
   float scale_x;
   float scale_y;
   bool fp_fbo;
   bool srgb_fbo;
   bool valid;
   unsigned abs_x;
   unsigned abs_y;
};

struct video_shader_parameter
{
   char id[64];
   char desc[64];
   float current;
   float minimum;
   float initial;
   float maximum;
   float step;
   int pass;
};

struct video_shader_pass
{
   struct
   {
      char path[PATH_MAX_LENGTH];
      struct
      {
         char *vertex; /* Dynamically allocated. Must be free'd. */
         char *fragment; /* Dynamically allocated. Must be free'd. */
      } string;
   } source;

   char alias[64];
   struct gfx_fbo_scale fbo;
   enum gfx_wrap_type wrap;
   bool mipmap;
   unsigned filter;
   unsigned frame_count_mod;
   bool feedback;
};

struct video_shader_lut
{
   char id[64];
   char path[PATH_MAX_LENGTH];
   enum gfx_wrap_type wrap;
   bool mipmap;
   unsigned filter;
};

/* This is pretty big, shouldn't be put on the stack.
 * Avoid lots of allocation for convenience. */
struct video_shader
{
   enum rarch_shader_type type;

   char prefix[64];
   char script_path[PATH_MAX_LENGTH];
   char path[PATH_MAX_LENGTH];

   unsigned passes;
   unsigned luts;
   unsigned num_parameters;
   unsigned variables;
   /* If < 0, no feedback pass is used. Otherwise,
    * the FBO after pass #N is passed a texture to next frame. */
   int feedback_pass;
   int history_size;

   struct video_shader_pass pass[GFX_MAX_SHADERS];

   struct video_shader_lut lut[GFX_MAX_TEXTURES];

   struct video_shader_parameter parameters[GFX_MAX_PARAMETERS];
};

config_file_p config_file_p_new(const char *path);
void config_file_p_free(config_file_p conf);

/**
 * video_shader_read_conf_cgp:
 * @conf              : Preset file to read from.
 * @shader            : Shader passes handle.
 *
 * Loads preset file and all associated state (passes,
 * textures, imports, etc).
 *
 * Returns: true (1) if successful, otherwise false (0).
 **/
bool video_shader_read_conf_cgp(config_file_p conf,
                                struct video_shader *shader);

/**
 * video_shader_write_conf_cgp:
 * @conf              : Preset file to read from.
 * @shader            : Shader passes handle.
 *
 * Saves preset and all associated state (passes,
 * textures, imports, etc) to disk.
 **/
void video_shader_write_conf_cgp(config_file_p conf,
                                 struct video_shader *shader);

/**
 * video_shader_resolve_relative:
 * @shader            : Shader pass handle.
 * @ref_path          : Relative shader path.
 *
 * Resolves relative shader path (@ref_path) into absolute
 * shader paths.
 **/
void video_shader_resolve_relative(struct video_shader *shader,
                                   const char *ref_path);

/**
 * video_shader_resolve_parameters:
 * @conf              : Preset file to read from.
 * @shader            : Shader passes handle.
 *
 * Reads the current value for all parameters from config file.
 *
 * Returns: true (1) if successful, otherwise false (0).
 **/
bool video_shader_resolve_current_parameters(config_file_p conf,
                                             struct video_shader *shader);

/**
 * video_shader_resolve_parameters:
 * @param conf      Preset file to read from.
 * @param shader    Shader passes handle.
 *
 * Resolves all shader parameters belonging to shaders.
 *
 * @return true (1) if successful, otherwise false (0).
 **/
bool video_shader_resolve_parameters(config_file_p conf,
                                     struct video_shader *shader);

RETRO_END_DECLS

#endif
