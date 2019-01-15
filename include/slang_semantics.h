//
// Created by Stuart Carnie on 2019-01-17.
//

#ifndef SLANG_SLANG_SEMANTICS_H
#define SLANG_SLANG_SEMANTICS_H

#include "glslang_util.h"

// Textures with built-in meaning.
enum slang_texture_semantic {
   // The input texture to the filter chain.
   // Canonical name: "Original".
      SLANG_TEXTURE_SEMANTIC_ORIGINAL = 0,

   // The output from pass N - 1 if executing pass N, or ORIGINAL
   // if pass #0 is executed.
   // Canonical name: "Source".
      SLANG_TEXTURE_SEMANTIC_SOURCE = 1,

   // The original inputs with a history back in time.
   // Canonical name: "OriginalHistory#", e.g. "OriginalHistory2" <- Two frames back.
   // "OriginalHistory0" is an alias for SEMANTIC_ORIGINAL.
   // Size name: "OriginalHistorySize#".
      SLANG_TEXTURE_SEMANTIC_ORIGINAL_HISTORY = 2,

   // The output from pass #N, where pass #0 is the first pass.
   // Canonical name: "PassOutput#", e.g. "PassOutput3".
   // Size name: "PassOutputSize#".
      SLANG_TEXTURE_SEMANTIC_PASS_OUTPUT = 3,

   // The output from pass #N, one frame ago where pass #0 is the first pass.
   // It is not valid to use the pass feedback from a pass which is not offscreen.
   // Canonical name: "PassFeedback#", e.g. "PassFeedback2".
      SLANG_TEXTURE_SEMANTIC_PASS_FEEDBACK = 4,

   // Inputs from static textures, defined by the user.
   // There is no canonical name, and the only way to use these semantics are by
   // remapping.
      SLANG_TEXTURE_SEMANTIC_USER = 5,

   SLANG_NUM_TEXTURE_SEMANTICS,
   SLANG_INVALID_TEXTURE_SEMANTIC = -1
};

enum slang_semantic {
   // mat4, MVP
      SLANG_SEMANTIC_MVP            = 0,
   // vec4, viewport size of current pass
      SLANG_SEMANTIC_OUTPUT         = 1,
   // vec4, viewport size of final pass
      SLANG_SEMANTIC_FINAL_VIEWPORT = 2,
   // uint, frame count with modulo
      SLANG_SEMANTIC_FRAME_COUNT    = 3,
      SLANG_NUM_BASE_SEMANTICS,

   // float, user defined parameter, arrayed
      SLANG_SEMANTIC_FLOAT_PARAMETER = 4,

   SLANG_NUM_SEMANTICS,
   SLANG_INVALID_SEMANTIC            = -1
};

enum slang_stage {
   SLANG_STAGE_VERTEX_MASK   = 1 << 0,
   SLANG_STAGE_FRAGMENT_MASK = 1 << 1
};

enum slang_constant_buffer {
   SLANG_CBUFFER_UBO = 0,
   SLANG_CBUFFER_PC,
   SLANG_CBUFFER_MAX,
};

typedef enum slang_wrap_type {
   SLANG_WRAP_BORDER  = 0,
   SLANG_WRAP_DEFAULT = SLANG_WRAP_BORDER,
   SLANG_WRAP_EDGE,
   SLANG_WRAP_REPEAT,
   SLANG_WRAP_MIRRORED_REPEAT,
   SLANG_WRAP_MAX
} slang_wrap_type_t;

typedef enum slang_filter {
   SLANG_FILTER_UNSPEC = 0,
   SLANG_FILTER_LINEAR,
   SLANG_FILTER_NEAREST,
   SLANG_FILTER_MAX
} slang_filter_t;

/* Vulkan minimum limit. */
#define SLANG_NUM_BINDINGS 16

typedef struct {
   void *ptr;
   size_t stride;
} data_map_t;

typedef struct {
   void *image;
   size_t image_stride;
   void *size;
   size_t size_stride;
} texture_map_t;

typedef struct {
   texture_map_t textures[SLANG_NUM_TEXTURE_SEMANTICS];
   void *uniforms[SLANG_NUM_BASE_SEMANTICS];
} semantics_map_t;

typedef struct {
   void *data;
   unsigned size;
   unsigned offset;
   char     id[64];
} uniform_sem_t;

typedef struct {
   void *texture_data;
   slang_wrap_type_t wrap;
   slang_filter_t    filter;
   unsigned          stage_mask;
   unsigned          binding;
   char              id[64];
} texture_sem_t;

typedef struct {
   unsigned stage_mask;
   unsigned binding;
   unsigned size;
   int      uniform_count;
   uniform_sem_t *uniforms;
} cbuffer_sem_t;

typedef struct {
   int texture_count;
   texture_sem_t *textures;
   cbuffer_sem_t  cbuffers[SLANG_CBUFFER_MAX];
   glslang_format format;
} pass_semantics_t;

//#define SLANG_STAGE_VERTEX_MASK (1 << 0)
//#define SLANG_STAGE_FRAGMENT_MASK (1 << 1)

#endif //SLANG_SLANG_SEMANTICS_H
