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

#import "slang_semantics.h"

#define kMaxShaderPasses 26
#define kMaxTextures     8
#define kMaxParameters   128
#define kMaxFrameHistory 128

typedef NS_ENUM(NSUInteger, ShaderType) {
   ShaderTypeNone = 0,
   ShaderTypeSlang,
   ShaderTypeMetal,
};

typedef NS_ENUM(NSUInteger, ShaderPassScale) {
   ShaderPassScaleInput = 0,
   ShaderPassScaleAbsolute,
   ShaderPassScaleViewport,
};

typedef NS_ENUM(NSUInteger, ShaderPassFilter) {
   ShaderPassFilterUnspecified = 0,
   ShaderPassFilterLinear,
   ShaderPassFilterNearest,
   ShaderPassFilterCount,
};

typedef NS_ENUM(NSUInteger, ShaderPassWrap) {
   ShaderPassWrapBorder  = 0,
   ShaderPassWrapDefault = ShaderPassWrapBorder,
   ShaderPassWrapEdge,
   ShaderPassWrapRepeat,
   ShaderPassWrapMirroredRepeat,
   ShaderPassWrapCount,
};

@interface ShaderPass : NSObject

@property (nonatomic, readonly) NSString         *path;
@property (nonatomic, readonly) NSUInteger       frameCountMod;
@property (nonatomic, readonly) ShaderPassScale  scaleX;
@property (nonatomic, readonly) ShaderPassScale  scaleY;
@property (nonatomic, readonly) ShaderPassFilter filter;
@property (nonatomic, readonly) ShaderPassWrap   wrapMode;
@property (nonatomic, readonly) CGSize           scale;
@property (nonatomic, readonly) CGSize           size;
@property (nonatomic, readonly) BOOL             valid;
@property (nonatomic, readonly) BOOL             isFloat;
@property (nonatomic, readonly) BOOL             issRGB;
@property (nonatomic, readonly) BOOL             isMipmap;
@property (nonatomic, readonly) BOOL             isFeedback;

- (BOOL)buildMetalVersion:(NSUInteger)version
                semantics:(semantics_map_t *)semantics
            passSemantics:(pass_semantics_t *)passSemantics
                   vertex:(NSString **)vs
                 fragment:(NSString **)fs;

@end

@interface ShaderLUT : NSObject

@property (nonatomic, readonly) NSString         *path;
@property (nonatomic, readonly) ShaderPassWrap   wrapMode;
@property (nonatomic, readonly) BOOL             isMipmap;
@property (nonatomic, readonly) ShaderPassFilter filter;

@end

@interface ShaderParameter : NSObject

@property (nonatomic, readonly) NSString   *name;
@property (nonatomic, readonly) NSString   *desc;
@property (nonatomic, readonly) NSUInteger index;
@property (nonatomic, readonly) float      *value;
@property (nonatomic, readonly) float      minimum;
@property (nonatomic, readonly) float      initial;
@property (nonatomic, readonly) float      maximum;
@property (nonatomic, readonly) float      step;
@property (nonatomic, readonly) int        pass;
@end

@interface SlangShader : NSObject

- (instancetype)initFromPath:(NSString *)path;

@property (nonatomic, readonly) NSArray<ShaderPass *>      *passes;
@property (nonatomic, readonly) NSArray<ShaderParameter *> *parameters;
@property (nonatomic, readonly) NSArray<ShaderLUT *>       *luts;
@property (nonatomic, readonly) NSUInteger                 historySize;

@end
