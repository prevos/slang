/**
 * Copyright (c) 2019 Stuart Carnie
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#import <Foundation/Foundation.h>
#import "SlangShader.h"
#import <file/config_file.h>
#import "video_shader_parse.h"
#import "drivers_shader/slang_process.h"

static ShaderPassWrap ShaderPassWrapFromString(NSString *wrapMode) {
   if ([wrapMode isEqualToString:@"clamp_to_border"])
      return ShaderPassWrapBorder;
   else if ([wrapMode isEqualToString:@"clamp_to_edge"])
      return ShaderPassWrapEdge;
   else if ([wrapMode isEqualToString:@"repeat"])
      return ShaderPassWrapRepeat;
   else if ([wrapMode isEqualToString:@"mirrored_repeat"])
      return ShaderPassWrapMirroredRepeat;

   NSLog(@"invalid wrap mode %@, specify (clamp_to_border, clamp_to_edge, repeat and mirrored_repeat.", wrapMode);

   return ShaderPassWrapDefault;
}

static ShaderPassWrap ShaderPassWrapFrom_gfx_wrap_type(enum gfx_wrap_type v) {
   switch (v) {
   case RARCH_WRAP_BORDER:
      return ShaderPassWrapBorder;
   case RARCH_WRAP_EDGE:
      return ShaderPassWrapEdge;
   case RARCH_WRAP_REPEAT:
      return ShaderPassWrapRepeat;
   case RARCH_WRAP_MIRRORED_REPEAT:
      return ShaderPassWrapMirroredRepeat;
      /* case RARCH_WRAP_DEFAULT: */
   default:
      return ShaderPassWrapBorder;
   }
}

static ShaderPassScale ShaderPassScaleFrom_gfx_scale_type(enum gfx_scale_type v) {
   switch (v) {
   case RARCH_SCALE_INPUT:
      return ShaderPassScaleInput;
   case RARCH_SCALE_ABSOLUTE:
      return ShaderPassScaleAbsolute;
   case RARCH_SCALE_VIEWPORT:
      return ShaderPassScaleViewport;
   default:
      return ShaderPassScaleInput;
   }
}

static ShaderPassFilter ShaderPassFilterFrom_gfx_filter(int v) {
   switch (v) {
   case RARCH_FILTER_LINEAR:
      return ShaderPassFilterLinear;
   case RARCH_FILTER_NEAREST:
      return ShaderPassFilterNearest;
   case RARCH_FILTER_UNSPEC:
   default:
      return ShaderPassFilterUnspecified;
   }
}

@implementation ShaderPass {
   struct video_shader      *_shader;
   struct video_shader_pass *_pass;
   NSUInteger               _index;
   NSString                 *_path;
}

- (instancetype)initWithShader:(struct video_shader *)shader
                         index:(NSUInteger)index {
   if (self = [super init]) {
      _shader = shader;
      _pass   = &shader->pass[index];
      _index  = index;
   }
   return self;
}

- (BOOL)buildMetalVersion:(NSUInteger)version
                semantics:(semantics_map_t *)semantics
            passSemantics:(pass_semantics_t *)passSemantics
                   vertex:(NSString **)vs
                 fragment:(NSString **)fs {
   bool res = slang_process(_shader, (unsigned int) _index, RARCH_SHADER_METAL, (unsigned int) version, semantics,
                            passSemantics);
   if (!res) {
      return NO;
   }

   *vs = [[NSString alloc] initWithBytesNoCopy:_pass->source.string.vertex
                                        length:strlen(_pass->source.string.vertex)
                                      encoding:NSUTF8StringEncoding
                                  freeWhenDone:YES];
   _pass->source.string.vertex = nil;

   *fs = [[NSString alloc] initWithBytesNoCopy:_pass->source.string.fragment
                                        length:strlen(_pass->source.string.fragment)
                                      encoding:NSUTF8StringEncoding
                                  freeWhenDone:YES];
   _pass->source.string.fragment = nil;

   return YES;
}

- (NSString *)path {
   if (_path == nil) {
      _path = [NSString stringWithUTF8String:_pass->source.path];
   }
   return _path;
}

- (NSUInteger)frameCountMod {
   return _pass->frame_count_mod;
}

- (ShaderPassScale)scaleX {
   return ShaderPassScaleFrom_gfx_scale_type(_pass->fbo.type_x);
}

- (ShaderPassScale)scaleY {
   return ShaderPassScaleFrom_gfx_scale_type(_pass->fbo.type_y);
}

- (ShaderPassFilter)filter {
   return ShaderPassFilterFrom_gfx_filter(_pass->filter);
}

- (ShaderPassWrap)wrapMode {
   return ShaderPassWrapFrom_gfx_wrap_type(_pass->wrap);
}

- (CGSize)scale {
   return CGSizeMake(_pass->fbo.scale_x, _pass->fbo.scale_y);
}

- (CGSize)size {
   return CGSizeMake(_pass->fbo.abs_x, _pass->fbo.abs_y);
}

- (BOOL)valid {
   return _pass->fbo.valid;
}

- (BOOL)isFloat {
   return _pass->fbo.fp_fbo;
}

- (BOOL)issRGB {
   return _pass->fbo.srgb_fbo;
}

- (BOOL)isMipmap {
   return _pass->mipmap;
}

- (BOOL)isFeedback {
   return _pass->feedback;
}

@end

@implementation ShaderLUT {
   struct video_shader     *_shader;
   struct video_shader_lut *_lut;
   NSString                *_path;
   NSUInteger              _index;
}

- (instancetype)initWithShader:(struct video_shader *)shader
                         index:(NSUInteger)index {
   if (self = [super init]) {
      _shader = shader;
      _lut    = &shader->lut[index];
      _path   = [[NSString alloc] initWithBytesNoCopy:_lut->path
                                               length:strlen(_lut->path)
                                             encoding:NSUTF8StringEncoding
                                         freeWhenDone:NO];
      _index  = index;
   }
   return self;
}

- (ShaderPassWrap)wrapMode {
   return ShaderPassWrapFrom_gfx_wrap_type(_lut->wrap);
}

- (BOOL)isMipmap {
   return _lut->mipmap;
}

- (ShaderPassFilter)filter {
   return ShaderPassFilterFrom_gfx_filter(_lut->filter);
}

@end

@implementation ShaderParameter {
   struct video_shader           *_shader;
   struct video_shader_parameter *_param;
   NSString                      *_name;
   NSString                      *_desc;
   NSUInteger                    _index;
}

- (instancetype)initWithShader:(struct video_shader *)shader
                         index:(NSUInteger)index {
   if (self = [super init]) {
      _shader = shader;
      _param  = &shader->parameters[index];

      _desc  = [[NSString alloc] initWithBytesNoCopy:_param->desc
                                              length:strlen(_param->desc)
                                            encoding:NSUTF8StringEncoding
                                        freeWhenDone:NO];
      _index = index;
   }
   return self;
}

- (NSString *)name {
   if (_name == nil) {
      _name = [[NSString alloc] initWithBytesNoCopy:_param->id
                                             length:strlen(_param->id)
                                           encoding:NSUTF8StringEncoding
                                       freeWhenDone:NO];
   }
   return _name;
}

- (NSString *)desc {
   if (_desc == nil) {
      _desc = [[NSString alloc] initWithBytesNoCopy:_param->desc
                                             length:strlen(_param->desc)
                                           encoding:NSUTF8StringEncoding
                                       freeWhenDone:NO];
   }
   return _desc;
}

- (float *)value {
   return &_param->current;
}

- (float)minimum {
   return _param->minimum;
}

- (float)initial {
   return _param->initial;
}

- (float)maximum {
   return _param->maximum;
}

- (float)step {
   return _param->step;
}

@end

@implementation SlangShader {
   ShaderType                        _type;
   NSString                          *_path;
   struct video_shader               _shader;
   NSMutableArray<ShaderPass *>      *_passes;
   NSMutableArray<ShaderLUT *>       *_luts;
   NSMutableArray<ShaderParameter *> *_parameters;
}

- (instancetype)initFromPath:(NSString *)path {
   if (self = [super init]) {
      _path = path;

      config_file_t *conf = config_file_new(path.UTF8String);

      if (!video_shader_read_conf_cgp(conf, &_shader)) {
         return nil;
      }

      video_shader_resolve_relative(&_shader, path.UTF8String);
      video_shader_resolve_parameters(conf, &_shader);

      _passes = [[NSMutableArray alloc] initWithCapacity:_shader.passes];
      for (unsigned i = 0; i < _shader.passes; i++) {
         _passes[i] = [[ShaderPass alloc] initWithShader:&_shader index:i];
      }

      if (_shader.luts > 0) {
         _luts = [[NSMutableArray alloc] initWithCapacity:_shader.luts];
         for (unsigned i = 0; i < _shader.luts; i++) {
            _luts[i] = [[ShaderLUT alloc] initWithShader:&_shader index:i];
         }
      }

      if (_shader.num_parameters > 0) {
         _parameters = [[NSMutableArray alloc] initWithCapacity:_shader.num_parameters];
         for (unsigned i = 0; i < _shader.num_parameters; i++) {
            _parameters[i] = [[ShaderParameter alloc] initWithShader:&_shader index:i];
         }
      }

      config_file_free(conf);
   }
   return self;
}

- (NSUInteger)historySize {
   return (NSUInteger) _shader.history_size;
}


@end
