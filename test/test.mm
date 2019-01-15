#include "gtest/gtest.h"
#include "video_shader_parse.h"
#include "../src/drivers_shader/slang_process.h"
#include "Settings.h"
#include <memory>
#import <Foundation/Foundation.h>
#include "formats/image.h"
#include "SlangShader.h"

#define MTLALIGN(x) __attribute__((aligned(x)))

typedef struct {
   float x;
   float y;
   float z;
   float w;
} float4_t;

typedef float4_t matrix_float4x4[4];

typedef struct texture {
   void     *view;
   float4_t size_data;
}                texture_t;

typedef struct MTLALIGN(16) {
   matrix_float4x4 mvp;

   struct {
      texture_t texture[kMaxFrameHistory + 1];
      float     viewport[4];
      float4_t  output_size;
   }               frame;

   struct {
      void             *buffers[SLANG_CBUFFER_MAX];
      texture_t        rt;
      texture_t        feedback;
      uint32_t         frame_count;
      pass_semantics_t semantics;
      float            viewport[4];
      void             *_state;
   }               pass[kMaxShaderPasses];

   texture_t luts[kMaxTextures];

} engine_t;

TEST(example, add) {
   auto                            path = GlobalTestSettings.testRoot + "/shaders/tests/in-array.slangp";
   std::shared_ptr<config_file_np> cfg(config_file_p_new(path.c_str()),
                                       [](config_file_p p) {
                                          config_file_p_free(p);
                                       });

   video_shader shader     = {};
   engine_t     engine;
   video_shader_read_conf_cgp(cfg.get(), &shader);
   video_shader_resolve_relative(&shader, path.c_str());
   video_shader_resolve_parameters(cfg.get(), &shader);

   matrix_float4x4 projectionMatrix;
   texture_t       *source = &engine.frame.texture[0];
   for (unsigned   i       = 0; i < shader.passes; source = &engine.pass[i++].rt) {
      matrix_float4x4 *mvp = (i == shader.passes - 1) ? &projectionMatrix : &engine.mvp;

      semantics_map_t semantics_map = {
         {
            /* Original */
            {&engine.frame.texture[0].view, 0,
               &engine.frame.texture[0].size_data, 0},

            /* Source */
            {&source->view, 0,
               &source->size_data, 0},

            /* OriginalHistory */
            {&engine.frame.texture[0].view, sizeof(*engine.frame.texture),
               &engine.frame.texture[0].size_data, sizeof(*engine.frame.texture)},

            /* PassOutput */
            {&engine.pass[0].rt.view, sizeof(*engine.pass),
               &engine.pass[0].rt.size_data, sizeof(*engine.pass)},

            /* PassFeedback */
            {&engine.pass[0].feedback.view, sizeof(*engine.pass),
               &engine.pass[0].feedback.size_data, sizeof(*engine.pass)},

            /* User */
            {&engine.luts[0].view, sizeof(*engine.luts),
               &engine.luts[0].size_data, sizeof(*engine.luts)},
         },
         {
            mvp,                          /* MVP */
            &engine.pass[i].rt.size_data, /* OutputSize */
            &engine.frame.output_size,    /* FinalViewportSize */
            &engine.pass[i].frame_count,  /* FrameCount */
         }
      };

      auto res = slang_process(&shader, i, RARCH_SHADER_METAL, 20000, &semantics_map, &engine.pass[i].semantics);

      free(shader.pass[i].source.string.vertex);
      shader.pass[i].source.string.vertex = nullptr;
      free(shader.pass[i].source.string.fragment);
      shader.pass[i].source.string.fragment = nullptr;

      ASSERT_TRUE(res);
   }

   for (unsigned i = 0; i < shader.luts; i++) {
      struct texture_image image = {0};
      image.supports_rgba = true;

      if (!image_texture_load(&image, shader.lut[i].path)) {
         continue;
      }

      // TODO(sgc): generate mip maps
      image_texture_free(&image);
   }

   video_shader_resolve_current_parameters(cfg.get(), &shader);

   ASSERT_TRUE(cfg != nullptr);
}

TEST(example, SlangShaderLoad) {
   auto path = GlobalTestSettings.testRoot + "/shaders/crt/crt-easymode-halation.slangp";

   SlangShader *s          = [[SlangShader alloc] initFromPath:[NSString stringWithUTF8String:path.c_str()]];

   engine_t engine;

   matrix_float4x4 projectionMatrix;
   texture_t       *source = &engine.frame.texture[0];
   unsigned        passes  = s.passes.count;
   for (unsigned   i       = 0; i < passes; source = &engine.pass[i++].rt) {
      matrix_float4x4 *mvp = (i == passes - 1) ? &projectionMatrix : &engine.mvp;

      semantics_map_t semantics_map = {
         {
            /* Original */
            {&engine.frame.texture[0].view, 0,
               &engine.frame.texture[0].size_data, 0},

            /* Source */
            {&source->view, 0,
               &source->size_data, 0},

            /* OriginalHistory */
            {&engine.frame.texture[0].view, sizeof(*engine.frame.texture),
               &engine.frame.texture[0].size_data, sizeof(*engine.frame.texture)},

            /* PassOutput */
            {&engine.pass[0].rt.view, sizeof(*engine.pass),
               &engine.pass[0].rt.size_data, sizeof(*engine.pass)},

            /* PassFeedback */
            {&engine.pass[0].feedback.view, sizeof(*engine.pass),
               &engine.pass[0].feedback.size_data, sizeof(*engine.pass)},

            /* User */
            {&engine.luts[0].view, sizeof(*engine.luts),
               &engine.luts[0].size_data, sizeof(*engine.luts)},
         },
         {
            mvp,                          /* MVP */
            &engine.pass[i].rt.size_data, /* OutputSize */
            &engine.frame.output_size,    /* FinalViewportSize */
            &engine.pass[i].frame_count,  /* FrameCount */
         }
      };

      NSString *vs, *ps;
      auto     res                  = [s.passes[i] buildMetalVersion:20000
                                                           semantics:&semantics_map
                                                       passSemantics:&engine.pass[i].semantics
                                                              vertex:&vs
                                                            fragment:&ps];

      ASSERT_TRUE(res);
   }

   unsigned      luts = s.luts.count;
   for (unsigned i    = 0; i < luts; i++) {
      struct texture_image image = {0};
      image.supports_rgba = true;

      if (!image_texture_load(&image, s.luts[i].path.UTF8String)) {
         continue;
      }

      // TODO(sgc): generate mip maps
      image_texture_free(&image);
   }

   ASSERT_TRUE(s != nil);
}
