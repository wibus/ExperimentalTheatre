/*

    This file was generated with gl3w_gen.py, part of gl3w
    (hosted at https://github.com/skaslev/gl3w)

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef __gl3w_h_
#define __gl3w_h_

#include "glcorearb.h"

#ifdef _WINDOWS
#   if defined(GL3_LIBRARY)
#       define GL3_EXPORT __declspec(dllexport)
#   else
#       define GL3_EXPORT __declspec(dllimport)
#   endif
#else
#   define GL3_EXPORT
#endif

#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GL3WglProc)(void);

/* gl3w api */
GL3_EXPORT int gl3wInit(void);
GL3_EXPORT int gl3wIsSupported(int major, int minor);
GL3_EXPORT GL3WglProc gl3wGetProcAddress(const char *proc);

/* OpenGL functions */
extern GL3_EXPORT PFNGLACTIVESHADERPROGRAMPROC                         gl3wActiveShaderProgram;
extern GL3_EXPORT PFNGLACTIVETEXTUREPROC                               gl3wActiveTexture;
extern GL3_EXPORT PFNGLATTACHSHADERPROC                                gl3wAttachShader;
extern GL3_EXPORT PFNGLBEGINCONDITIONALRENDERPROC                      gl3wBeginConditionalRender;
extern GL3_EXPORT PFNGLBEGINQUERYPROC                                  gl3wBeginQuery;
extern GL3_EXPORT PFNGLBEGINQUERYINDEXEDPROC                           gl3wBeginQueryIndexed;
extern GL3_EXPORT PFNGLBEGINTRANSFORMFEEDBACKPROC                      gl3wBeginTransformFeedback;
extern GL3_EXPORT PFNGLBINDATTRIBLOCATIONPROC                          gl3wBindAttribLocation;
extern GL3_EXPORT PFNGLBINDBUFFERPROC                                  gl3wBindBuffer;
extern GL3_EXPORT PFNGLBINDBUFFERBASEPROC                              gl3wBindBufferBase;
extern GL3_EXPORT PFNGLBINDBUFFERRANGEPROC                             gl3wBindBufferRange;
extern GL3_EXPORT PFNGLBINDBUFFERSBASEPROC                             gl3wBindBuffersBase;
extern GL3_EXPORT PFNGLBINDBUFFERSRANGEPROC                            gl3wBindBuffersRange;
extern GL3_EXPORT PFNGLBINDFRAGDATALOCATIONPROC                        gl3wBindFragDataLocation;
extern GL3_EXPORT PFNGLBINDFRAGDATALOCATIONINDEXEDPROC                 gl3wBindFragDataLocationIndexed;
extern GL3_EXPORT PFNGLBINDFRAMEBUFFERPROC                             gl3wBindFramebuffer;
extern GL3_EXPORT PFNGLBINDIMAGETEXTUREPROC                            gl3wBindImageTexture;
extern GL3_EXPORT PFNGLBINDIMAGETEXTURESPROC                           gl3wBindImageTextures;
extern GL3_EXPORT PFNGLBINDPROGRAMPIPELINEPROC                         gl3wBindProgramPipeline;
extern GL3_EXPORT PFNGLBINDRENDERBUFFERPROC                            gl3wBindRenderbuffer;
extern GL3_EXPORT PFNGLBINDSAMPLERPROC                                 gl3wBindSampler;
extern GL3_EXPORT PFNGLBINDSAMPLERSPROC                                gl3wBindSamplers;
extern GL3_EXPORT PFNGLBINDTEXTUREPROC                                 gl3wBindTexture;
extern GL3_EXPORT PFNGLBINDTEXTUREUNITPROC                             gl3wBindTextureUnit;
extern GL3_EXPORT PFNGLBINDTEXTURESPROC                                gl3wBindTextures;
extern GL3_EXPORT PFNGLBINDTRANSFORMFEEDBACKPROC                       gl3wBindTransformFeedback;
extern GL3_EXPORT PFNGLBINDVERTEXARRAYPROC                             gl3wBindVertexArray;
extern GL3_EXPORT PFNGLBINDVERTEXBUFFERPROC                            gl3wBindVertexBuffer;
extern GL3_EXPORT PFNGLBINDVERTEXBUFFERSPROC                           gl3wBindVertexBuffers;
extern GL3_EXPORT PFNGLBLENDCOLORPROC                                  gl3wBlendColor;
extern GL3_EXPORT PFNGLBLENDEQUATIONPROC                               gl3wBlendEquation;
extern GL3_EXPORT PFNGLBLENDEQUATIONSEPARATEPROC                       gl3wBlendEquationSeparate;
extern GL3_EXPORT PFNGLBLENDEQUATIONSEPARATEIPROC                      gl3wBlendEquationSeparatei;
extern GL3_EXPORT PFNGLBLENDEQUATIONSEPARATEIARBPROC                   gl3wBlendEquationSeparateiARB;
extern GL3_EXPORT PFNGLBLENDEQUATIONIPROC                              gl3wBlendEquationi;
extern GL3_EXPORT PFNGLBLENDEQUATIONIARBPROC                           gl3wBlendEquationiARB;
extern GL3_EXPORT PFNGLBLENDFUNCPROC                                   gl3wBlendFunc;
extern GL3_EXPORT PFNGLBLENDFUNCSEPARATEPROC                           gl3wBlendFuncSeparate;
extern GL3_EXPORT PFNGLBLENDFUNCSEPARATEIPROC                          gl3wBlendFuncSeparatei;
extern GL3_EXPORT PFNGLBLENDFUNCSEPARATEIARBPROC                       gl3wBlendFuncSeparateiARB;
extern GL3_EXPORT PFNGLBLENDFUNCIPROC                                  gl3wBlendFunci;
extern GL3_EXPORT PFNGLBLENDFUNCIARBPROC                               gl3wBlendFunciARB;
extern GL3_EXPORT PFNGLBLITFRAMEBUFFERPROC                             gl3wBlitFramebuffer;
extern GL3_EXPORT PFNGLBLITNAMEDFRAMEBUFFERPROC                        gl3wBlitNamedFramebuffer;
extern GL3_EXPORT PFNGLBUFFERDATAPROC                                  gl3wBufferData;
extern GL3_EXPORT PFNGLBUFFERPAGECOMMITMENTARBPROC                     gl3wBufferPageCommitmentARB;
extern GL3_EXPORT PFNGLBUFFERSTORAGEPROC                               gl3wBufferStorage;
extern GL3_EXPORT PFNGLBUFFERSUBDATAPROC                               gl3wBufferSubData;
extern GL3_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSPROC                      gl3wCheckFramebufferStatus;
extern GL3_EXPORT PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC                 gl3wCheckNamedFramebufferStatus;
extern GL3_EXPORT PFNGLCLAMPCOLORPROC                                  gl3wClampColor;
extern GL3_EXPORT PFNGLCLEARPROC                                       gl3wClear;
extern GL3_EXPORT PFNGLCLEARBUFFERDATAPROC                             gl3wClearBufferData;
extern GL3_EXPORT PFNGLCLEARBUFFERSUBDATAPROC                          gl3wClearBufferSubData;
extern GL3_EXPORT PFNGLCLEARBUFFERFIPROC                               gl3wClearBufferfi;
extern GL3_EXPORT PFNGLCLEARBUFFERFVPROC                               gl3wClearBufferfv;
extern GL3_EXPORT PFNGLCLEARBUFFERIVPROC                               gl3wClearBufferiv;
extern GL3_EXPORT PFNGLCLEARBUFFERUIVPROC                              gl3wClearBufferuiv;
extern GL3_EXPORT PFNGLCLEARCOLORPROC                                  gl3wClearColor;
extern GL3_EXPORT PFNGLCLEARDEPTHPROC                                  gl3wClearDepth;
extern GL3_EXPORT PFNGLCLEARDEPTHFPROC                                 gl3wClearDepthf;
extern GL3_EXPORT PFNGLCLEARNAMEDBUFFERDATAPROC                        gl3wClearNamedBufferData;
extern GL3_EXPORT PFNGLCLEARNAMEDBUFFERSUBDATAPROC                     gl3wClearNamedBufferSubData;
extern GL3_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                     gl3wClearNamedFramebufferfi;
extern GL3_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                     gl3wClearNamedFramebufferfv;
extern GL3_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                     gl3wClearNamedFramebufferiv;
extern GL3_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                    gl3wClearNamedFramebufferuiv;
extern GL3_EXPORT PFNGLCLEARSTENCILPROC                                gl3wClearStencil;
extern GL3_EXPORT PFNGLCLEARTEXIMAGEPROC                               gl3wClearTexImage;
extern GL3_EXPORT PFNGLCLEARTEXSUBIMAGEPROC                            gl3wClearTexSubImage;
extern GL3_EXPORT PFNGLCLIENTWAITSYNCPROC                              gl3wClientWaitSync;
extern GL3_EXPORT PFNGLCLIPCONTROLPROC                                 gl3wClipControl;
extern GL3_EXPORT PFNGLCOLORMASKPROC                                   gl3wColorMask;
extern GL3_EXPORT PFNGLCOLORMASKIPROC                                  gl3wColorMaski;
extern GL3_EXPORT PFNGLCOMPILESHADERPROC                               gl3wCompileShader;
extern GL3_EXPORT PFNGLCOMPILESHADERINCLUDEARBPROC                     gl3wCompileShaderIncludeARB;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXIMAGE1DPROC                        gl3wCompressedTexImage1D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DPROC                        gl3wCompressedTexImage2D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DPROC                        gl3wCompressedTexImage3D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                     gl3wCompressedTexSubImage1D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                     gl3wCompressedTexSubImage2D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                     gl3wCompressedTexSubImage3D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC                 gl3wCompressedTextureSubImage1D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC                 gl3wCompressedTextureSubImage2D;
extern GL3_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC                 gl3wCompressedTextureSubImage3D;
extern GL3_EXPORT PFNGLCOPYBUFFERSUBDATAPROC                           gl3wCopyBufferSubData;
extern GL3_EXPORT PFNGLCOPYIMAGESUBDATAPROC                            gl3wCopyImageSubData;
extern GL3_EXPORT PFNGLCOPYNAMEDBUFFERSUBDATAPROC                      gl3wCopyNamedBufferSubData;
extern GL3_EXPORT PFNGLCOPYTEXIMAGE1DPROC                              gl3wCopyTexImage1D;
extern GL3_EXPORT PFNGLCOPYTEXIMAGE2DPROC                              gl3wCopyTexImage2D;
extern GL3_EXPORT PFNGLCOPYTEXSUBIMAGE1DPROC                           gl3wCopyTexSubImage1D;
extern GL3_EXPORT PFNGLCOPYTEXSUBIMAGE2DPROC                           gl3wCopyTexSubImage2D;
extern GL3_EXPORT PFNGLCOPYTEXSUBIMAGE3DPROC                           gl3wCopyTexSubImage3D;
extern GL3_EXPORT PFNGLCOPYTEXTURESUBIMAGE1DPROC                       gl3wCopyTextureSubImage1D;
extern GL3_EXPORT PFNGLCOPYTEXTURESUBIMAGE2DPROC                       gl3wCopyTextureSubImage2D;
extern GL3_EXPORT PFNGLCOPYTEXTURESUBIMAGE3DPROC                       gl3wCopyTextureSubImage3D;
extern GL3_EXPORT PFNGLCREATEBUFFERSPROC                               gl3wCreateBuffers;
extern GL3_EXPORT PFNGLCREATEFRAMEBUFFERSPROC                          gl3wCreateFramebuffers;
extern GL3_EXPORT PFNGLCREATEPROGRAMPROC                               gl3wCreateProgram;
extern GL3_EXPORT PFNGLCREATEPROGRAMPIPELINESPROC                      gl3wCreateProgramPipelines;
extern GL3_EXPORT PFNGLCREATEQUERIESPROC                               gl3wCreateQueries;
extern GL3_EXPORT PFNGLCREATERENDERBUFFERSPROC                         gl3wCreateRenderbuffers;
extern GL3_EXPORT PFNGLCREATESAMPLERSPROC                              gl3wCreateSamplers;
extern GL3_EXPORT PFNGLCREATESHADERPROC                                gl3wCreateShader;
extern GL3_EXPORT PFNGLCREATESHADERPROGRAMVPROC                        gl3wCreateShaderProgramv;
extern GL3_EXPORT PFNGLCREATESYNCFROMCLEVENTARBPROC                    gl3wCreateSyncFromCLeventARB;
extern GL3_EXPORT PFNGLCREATETEXTURESPROC                              gl3wCreateTextures;
extern GL3_EXPORT PFNGLCREATETRANSFORMFEEDBACKSPROC                    gl3wCreateTransformFeedbacks;
extern GL3_EXPORT PFNGLCREATEVERTEXARRAYSPROC                          gl3wCreateVertexArrays;
extern GL3_EXPORT PFNGLCULLFACEPROC                                    gl3wCullFace;
extern GL3_EXPORT PFNGLDEBUGMESSAGECALLBACKPROC                        gl3wDebugMessageCallback;
extern GL3_EXPORT PFNGLDEBUGMESSAGECALLBACKARBPROC                     gl3wDebugMessageCallbackARB;
extern GL3_EXPORT PFNGLDEBUGMESSAGECONTROLPROC                         gl3wDebugMessageControl;
extern GL3_EXPORT PFNGLDEBUGMESSAGECONTROLARBPROC                      gl3wDebugMessageControlARB;
extern GL3_EXPORT PFNGLDEBUGMESSAGEINSERTPROC                          gl3wDebugMessageInsert;
extern GL3_EXPORT PFNGLDEBUGMESSAGEINSERTARBPROC                       gl3wDebugMessageInsertARB;
extern GL3_EXPORT PFNGLDELETEBUFFERSPROC                               gl3wDeleteBuffers;
extern GL3_EXPORT PFNGLDELETEFRAMEBUFFERSPROC                          gl3wDeleteFramebuffers;
extern GL3_EXPORT PFNGLDELETENAMEDSTRINGARBPROC                        gl3wDeleteNamedStringARB;
extern GL3_EXPORT PFNGLDELETEPROGRAMPROC                               gl3wDeleteProgram;
extern GL3_EXPORT PFNGLDELETEPROGRAMPIPELINESPROC                      gl3wDeleteProgramPipelines;
extern GL3_EXPORT PFNGLDELETEQUERIESPROC                               gl3wDeleteQueries;
extern GL3_EXPORT PFNGLDELETERENDERBUFFERSPROC                         gl3wDeleteRenderbuffers;
extern GL3_EXPORT PFNGLDELETESAMPLERSPROC                              gl3wDeleteSamplers;
extern GL3_EXPORT PFNGLDELETESHADERPROC                                gl3wDeleteShader;
extern GL3_EXPORT PFNGLDELETESYNCPROC                                  gl3wDeleteSync;
extern GL3_EXPORT PFNGLDELETETEXTURESPROC                              gl3wDeleteTextures;
extern GL3_EXPORT PFNGLDELETETRANSFORMFEEDBACKSPROC                    gl3wDeleteTransformFeedbacks;
extern GL3_EXPORT PFNGLDELETEVERTEXARRAYSPROC                          gl3wDeleteVertexArrays;
extern GL3_EXPORT PFNGLDEPTHFUNCPROC                                   gl3wDepthFunc;
extern GL3_EXPORT PFNGLDEPTHMASKPROC                                   gl3wDepthMask;
extern GL3_EXPORT PFNGLDEPTHRANGEPROC                                  gl3wDepthRange;
extern GL3_EXPORT PFNGLDEPTHRANGEARRAYVPROC                            gl3wDepthRangeArrayv;
extern GL3_EXPORT PFNGLDEPTHRANGEINDEXEDPROC                           gl3wDepthRangeIndexed;
extern GL3_EXPORT PFNGLDEPTHRANGEFPROC                                 gl3wDepthRangef;
extern GL3_EXPORT PFNGLDETACHSHADERPROC                                gl3wDetachShader;
extern GL3_EXPORT PFNGLDISABLEPROC                                     gl3wDisable;
extern GL3_EXPORT PFNGLDISABLEVERTEXARRAYATTRIBPROC                    gl3wDisableVertexArrayAttrib;
extern GL3_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYPROC                    gl3wDisableVertexAttribArray;
extern GL3_EXPORT PFNGLDISABLEIPROC                                    gl3wDisablei;
extern GL3_EXPORT PFNGLDISPATCHCOMPUTEPROC                             gl3wDispatchCompute;
extern GL3_EXPORT PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC                 gl3wDispatchComputeGroupSizeARB;
extern GL3_EXPORT PFNGLDISPATCHCOMPUTEINDIRECTPROC                     gl3wDispatchComputeIndirect;
extern GL3_EXPORT PFNGLDRAWARRAYSPROC                                  gl3wDrawArrays;
extern GL3_EXPORT PFNGLDRAWARRAYSINDIRECTPROC                          gl3wDrawArraysIndirect;
extern GL3_EXPORT PFNGLDRAWARRAYSINSTANCEDPROC                         gl3wDrawArraysInstanced;
extern GL3_EXPORT PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             gl3wDrawArraysInstancedBaseInstance;
extern GL3_EXPORT PFNGLDRAWBUFFERPROC                                  gl3wDrawBuffer;
extern GL3_EXPORT PFNGLDRAWBUFFERSPROC                                 gl3wDrawBuffers;
extern GL3_EXPORT PFNGLDRAWELEMENTSPROC                                gl3wDrawElements;
extern GL3_EXPORT PFNGLDRAWELEMENTSBASEVERTEXPROC                      gl3wDrawElementsBaseVertex;
extern GL3_EXPORT PFNGLDRAWELEMENTSINDIRECTPROC                        gl3wDrawElementsIndirect;
extern GL3_EXPORT PFNGLDRAWELEMENTSINSTANCEDPROC                       gl3wDrawElementsInstanced;
extern GL3_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           gl3wDrawElementsInstancedBaseInstance;
extern GL3_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC             gl3wDrawElementsInstancedBaseVertex;
extern GL3_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC gl3wDrawElementsInstancedBaseVertexBaseInstance;
extern GL3_EXPORT PFNGLDRAWRANGEELEMENTSPROC                           gl3wDrawRangeElements;
extern GL3_EXPORT PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                 gl3wDrawRangeElementsBaseVertex;
extern GL3_EXPORT PFNGLDRAWTRANSFORMFEEDBACKPROC                       gl3wDrawTransformFeedback;
extern GL3_EXPORT PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              gl3wDrawTransformFeedbackInstanced;
extern GL3_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                 gl3wDrawTransformFeedbackStream;
extern GL3_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        gl3wDrawTransformFeedbackStreamInstanced;
extern GL3_EXPORT PFNGLENABLEPROC                                      gl3wEnable;
extern GL3_EXPORT PFNGLENABLEVERTEXARRAYATTRIBPROC                     gl3wEnableVertexArrayAttrib;
extern GL3_EXPORT PFNGLENABLEVERTEXATTRIBARRAYPROC                     gl3wEnableVertexAttribArray;
extern GL3_EXPORT PFNGLENABLEIPROC                                     gl3wEnablei;
extern GL3_EXPORT PFNGLENDCONDITIONALRENDERPROC                        gl3wEndConditionalRender;
extern GL3_EXPORT PFNGLENDQUERYPROC                                    gl3wEndQuery;
extern GL3_EXPORT PFNGLENDQUERYINDEXEDPROC                             gl3wEndQueryIndexed;
extern GL3_EXPORT PFNGLENDTRANSFORMFEEDBACKPROC                        gl3wEndTransformFeedback;
extern GL3_EXPORT PFNGLFENCESYNCPROC                                   gl3wFenceSync;
extern GL3_EXPORT PFNGLFINISHPROC                                      gl3wFinish;
extern GL3_EXPORT PFNGLFLUSHPROC                                       gl3wFlush;
extern GL3_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEPROC                      gl3wFlushMappedBufferRange;
extern GL3_EXPORT PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC                 gl3wFlushMappedNamedBufferRange;
extern GL3_EXPORT PFNGLFRAMEBUFFERPARAMETERIPROC                       gl3wFramebufferParameteri;
extern GL3_EXPORT PFNGLFRAMEBUFFERRENDERBUFFERPROC                     gl3wFramebufferRenderbuffer;
extern GL3_EXPORT PFNGLFRAMEBUFFERTEXTUREPROC                          gl3wFramebufferTexture;
extern GL3_EXPORT PFNGLFRAMEBUFFERTEXTURE1DPROC                        gl3wFramebufferTexture1D;
extern GL3_EXPORT PFNGLFRAMEBUFFERTEXTURE2DPROC                        gl3wFramebufferTexture2D;
extern GL3_EXPORT PFNGLFRAMEBUFFERTEXTURE3DPROC                        gl3wFramebufferTexture3D;
extern GL3_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERPROC                     gl3wFramebufferTextureLayer;
extern GL3_EXPORT PFNGLFRONTFACEPROC                                   gl3wFrontFace;
extern GL3_EXPORT PFNGLGENBUFFERSPROC                                  gl3wGenBuffers;
extern GL3_EXPORT PFNGLGENFRAMEBUFFERSPROC                             gl3wGenFramebuffers;
extern GL3_EXPORT PFNGLGENPROGRAMPIPELINESPROC                         gl3wGenProgramPipelines;
extern GL3_EXPORT PFNGLGENQUERIESPROC                                  gl3wGenQueries;
extern GL3_EXPORT PFNGLGENRENDERBUFFERSPROC                            gl3wGenRenderbuffers;
extern GL3_EXPORT PFNGLGENSAMPLERSPROC                                 gl3wGenSamplers;
extern GL3_EXPORT PFNGLGENTEXTURESPROC                                 gl3wGenTextures;
extern GL3_EXPORT PFNGLGENTRANSFORMFEEDBACKSPROC                       gl3wGenTransformFeedbacks;
extern GL3_EXPORT PFNGLGENVERTEXARRAYSPROC                             gl3wGenVertexArrays;
extern GL3_EXPORT PFNGLGENERATEMIPMAPPROC                              gl3wGenerateMipmap;
extern GL3_EXPORT PFNGLGENERATETEXTUREMIPMAPPROC                       gl3wGenerateTextureMipmap;
extern GL3_EXPORT PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC              gl3wGetActiveAtomicCounterBufferiv;
extern GL3_EXPORT PFNGLGETACTIVEATTRIBPROC                             gl3wGetActiveAttrib;
extern GL3_EXPORT PFNGLGETACTIVESUBROUTINENAMEPROC                     gl3wGetActiveSubroutineName;
extern GL3_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC              gl3wGetActiveSubroutineUniformName;
extern GL3_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC                gl3wGetActiveSubroutineUniformiv;
extern GL3_EXPORT PFNGLGETACTIVEUNIFORMPROC                            gl3wGetActiveUniform;
extern GL3_EXPORT PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                   gl3wGetActiveUniformBlockName;
extern GL3_EXPORT PFNGLGETACTIVEUNIFORMBLOCKIVPROC                     gl3wGetActiveUniformBlockiv;
extern GL3_EXPORT PFNGLGETACTIVEUNIFORMNAMEPROC                        gl3wGetActiveUniformName;
extern GL3_EXPORT PFNGLGETACTIVEUNIFORMSIVPROC                         gl3wGetActiveUniformsiv;
extern GL3_EXPORT PFNGLGETATTACHEDSHADERSPROC                          gl3wGetAttachedShaders;
extern GL3_EXPORT PFNGLGETATTRIBLOCATIONPROC                           gl3wGetAttribLocation;
extern GL3_EXPORT PFNGLGETBOOLEANI_VPROC                               gl3wGetBooleani_v;
extern GL3_EXPORT PFNGLGETBOOLEANVPROC                                 gl3wGetBooleanv;
extern GL3_EXPORT PFNGLGETBUFFERPARAMETERI64VPROC                      gl3wGetBufferParameteri64v;
extern GL3_EXPORT PFNGLGETBUFFERPARAMETERIVPROC                        gl3wGetBufferParameteriv;
extern GL3_EXPORT PFNGLGETBUFFERPOINTERVPROC                           gl3wGetBufferPointerv;
extern GL3_EXPORT PFNGLGETBUFFERSUBDATAPROC                            gl3wGetBufferSubData;
extern GL3_EXPORT PFNGLGETCOMPRESSEDTEXIMAGEPROC                       gl3wGetCompressedTexImage;
extern GL3_EXPORT PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                   gl3wGetCompressedTextureImage;
extern GL3_EXPORT PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC                gl3wGetCompressedTextureSubImage;
extern GL3_EXPORT PFNGLGETDEBUGMESSAGELOGPROC                          gl3wGetDebugMessageLog;
extern GL3_EXPORT PFNGLGETDEBUGMESSAGELOGARBPROC                       gl3wGetDebugMessageLogARB;
extern GL3_EXPORT PFNGLGETDOUBLEI_VPROC                                gl3wGetDoublei_v;
extern GL3_EXPORT PFNGLGETDOUBLEVPROC                                  gl3wGetDoublev;
extern GL3_EXPORT PFNGLGETERRORPROC                                    gl3wGetError;
extern GL3_EXPORT PFNGLGETFLOATI_VPROC                                 gl3wGetFloati_v;
extern GL3_EXPORT PFNGLGETFLOATVPROC                                   gl3wGetFloatv;
extern GL3_EXPORT PFNGLGETFRAGDATAINDEXPROC                            gl3wGetFragDataIndex;
extern GL3_EXPORT PFNGLGETFRAGDATALOCATIONPROC                         gl3wGetFragDataLocation;
extern GL3_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC         gl3wGetFramebufferAttachmentParameteriv;
extern GL3_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVPROC                   gl3wGetFramebufferParameteriv;
extern GL3_EXPORT PFNGLGETGRAPHICSRESETSTATUSPROC                      gl3wGetGraphicsResetStatus;
extern GL3_EXPORT PFNGLGETGRAPHICSRESETSTATUSARBPROC                   gl3wGetGraphicsResetStatusARB;
extern GL3_EXPORT PFNGLGETIMAGEHANDLEARBPROC                           gl3wGetImageHandleARB;
extern GL3_EXPORT PFNGLGETINTEGER64I_VPROC                             gl3wGetInteger64i_v;
extern GL3_EXPORT PFNGLGETINTEGER64VPROC                               gl3wGetInteger64v;
extern GL3_EXPORT PFNGLGETINTEGERI_VPROC                               gl3wGetIntegeri_v;
extern GL3_EXPORT PFNGLGETINTEGERVPROC                                 gl3wGetIntegerv;
extern GL3_EXPORT PFNGLGETINTERNALFORMATI64VPROC                       gl3wGetInternalformati64v;
extern GL3_EXPORT PFNGLGETINTERNALFORMATIVPROC                         gl3wGetInternalformativ;
extern GL3_EXPORT PFNGLGETMULTISAMPLEFVPROC                            gl3wGetMultisamplefv;
extern GL3_EXPORT PFNGLGETNAMEDBUFFERPARAMETERI64VPROC                 gl3wGetNamedBufferParameteri64v;
extern GL3_EXPORT PFNGLGETNAMEDBUFFERPARAMETERIVPROC                   gl3wGetNamedBufferParameteriv;
extern GL3_EXPORT PFNGLGETNAMEDBUFFERPOINTERVPROC                      gl3wGetNamedBufferPointerv;
extern GL3_EXPORT PFNGLGETNAMEDBUFFERSUBDATAPROC                       gl3wGetNamedBufferSubData;
extern GL3_EXPORT PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC    gl3wGetNamedFramebufferAttachmentParameteriv;
extern GL3_EXPORT PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC              gl3wGetNamedFramebufferParameteriv;
extern GL3_EXPORT PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC             gl3wGetNamedRenderbufferParameteriv;
extern GL3_EXPORT PFNGLGETNAMEDSTRINGARBPROC                           gl3wGetNamedStringARB;
extern GL3_EXPORT PFNGLGETNAMEDSTRINGIVARBPROC                         gl3wGetNamedStringivARB;
extern GL3_EXPORT PFNGLGETOBJECTLABELPROC                              gl3wGetObjectLabel;
extern GL3_EXPORT PFNGLGETOBJECTPTRLABELPROC                           gl3wGetObjectPtrLabel;
extern GL3_EXPORT PFNGLGETPOINTERVPROC                                 gl3wGetPointerv;
extern GL3_EXPORT PFNGLGETPROGRAMBINARYPROC                            gl3wGetProgramBinary;
extern GL3_EXPORT PFNGLGETPROGRAMINFOLOGPROC                           gl3wGetProgramInfoLog;
extern GL3_EXPORT PFNGLGETPROGRAMINTERFACEIVPROC                       gl3wGetProgramInterfaceiv;
extern GL3_EXPORT PFNGLGETPROGRAMPIPELINEINFOLOGPROC                   gl3wGetProgramPipelineInfoLog;
extern GL3_EXPORT PFNGLGETPROGRAMPIPELINEIVPROC                        gl3wGetProgramPipelineiv;
extern GL3_EXPORT PFNGLGETPROGRAMRESOURCEINDEXPROC                     gl3wGetProgramResourceIndex;
extern GL3_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONPROC                  gl3wGetProgramResourceLocation;
extern GL3_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC             gl3wGetProgramResourceLocationIndex;
extern GL3_EXPORT PFNGLGETPROGRAMRESOURCENAMEPROC                      gl3wGetProgramResourceName;
extern GL3_EXPORT PFNGLGETPROGRAMRESOURCEIVPROC                        gl3wGetProgramResourceiv;
extern GL3_EXPORT PFNGLGETPROGRAMSTAGEIVPROC                           gl3wGetProgramStageiv;
extern GL3_EXPORT PFNGLGETPROGRAMIVPROC                                gl3wGetProgramiv;
extern GL3_EXPORT PFNGLGETQUERYBUFFEROBJECTI64VPROC                    gl3wGetQueryBufferObjecti64v;
extern GL3_EXPORT PFNGLGETQUERYBUFFEROBJECTIVPROC                      gl3wGetQueryBufferObjectiv;
extern GL3_EXPORT PFNGLGETQUERYBUFFEROBJECTUI64VPROC                   gl3wGetQueryBufferObjectui64v;
extern GL3_EXPORT PFNGLGETQUERYBUFFEROBJECTUIVPROC                     gl3wGetQueryBufferObjectuiv;
extern GL3_EXPORT PFNGLGETQUERYINDEXEDIVPROC                           gl3wGetQueryIndexediv;
extern GL3_EXPORT PFNGLGETQUERYOBJECTI64VPROC                          gl3wGetQueryObjecti64v;
extern GL3_EXPORT PFNGLGETQUERYOBJECTIVPROC                            gl3wGetQueryObjectiv;
extern GL3_EXPORT PFNGLGETQUERYOBJECTUI64VPROC                         gl3wGetQueryObjectui64v;
extern GL3_EXPORT PFNGLGETQUERYOBJECTUIVPROC                           gl3wGetQueryObjectuiv;
extern GL3_EXPORT PFNGLGETQUERYIVPROC                                  gl3wGetQueryiv;
extern GL3_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVPROC                  gl3wGetRenderbufferParameteriv;
extern GL3_EXPORT PFNGLGETSAMPLERPARAMETERIIVPROC                      gl3wGetSamplerParameterIiv;
extern GL3_EXPORT PFNGLGETSAMPLERPARAMETERIUIVPROC                     gl3wGetSamplerParameterIuiv;
extern GL3_EXPORT PFNGLGETSAMPLERPARAMETERFVPROC                       gl3wGetSamplerParameterfv;
extern GL3_EXPORT PFNGLGETSAMPLERPARAMETERIVPROC                       gl3wGetSamplerParameteriv;
extern GL3_EXPORT PFNGLGETSHADERINFOLOGPROC                            gl3wGetShaderInfoLog;
extern GL3_EXPORT PFNGLGETSHADERPRECISIONFORMATPROC                    gl3wGetShaderPrecisionFormat;
extern GL3_EXPORT PFNGLGETSHADERSOURCEPROC                             gl3wGetShaderSource;
extern GL3_EXPORT PFNGLGETSHADERIVPROC                                 gl3wGetShaderiv;
extern GL3_EXPORT PFNGLGETSTRINGPROC                                   gl3wGetString;
extern GL3_EXPORT PFNGLGETSTRINGIPROC                                  gl3wGetStringi;
extern GL3_EXPORT PFNGLGETSUBROUTINEINDEXPROC                          gl3wGetSubroutineIndex;
extern GL3_EXPORT PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC                gl3wGetSubroutineUniformLocation;
extern GL3_EXPORT PFNGLGETSYNCIVPROC                                   gl3wGetSynciv;
extern GL3_EXPORT PFNGLGETTEXIMAGEPROC                                 gl3wGetTexImage;
extern GL3_EXPORT PFNGLGETTEXLEVELPARAMETERFVPROC                      gl3wGetTexLevelParameterfv;
extern GL3_EXPORT PFNGLGETTEXLEVELPARAMETERIVPROC                      gl3wGetTexLevelParameteriv;
extern GL3_EXPORT PFNGLGETTEXPARAMETERIIVPROC                          gl3wGetTexParameterIiv;
extern GL3_EXPORT PFNGLGETTEXPARAMETERIUIVPROC                         gl3wGetTexParameterIuiv;
extern GL3_EXPORT PFNGLGETTEXPARAMETERFVPROC                           gl3wGetTexParameterfv;
extern GL3_EXPORT PFNGLGETTEXPARAMETERIVPROC                           gl3wGetTexParameteriv;
extern GL3_EXPORT PFNGLGETTEXTUREHANDLEARBPROC                         gl3wGetTextureHandleARB;
extern GL3_EXPORT PFNGLGETTEXTUREIMAGEPROC                             gl3wGetTextureImage;
extern GL3_EXPORT PFNGLGETTEXTURELEVELPARAMETERFVPROC                  gl3wGetTextureLevelParameterfv;
extern GL3_EXPORT PFNGLGETTEXTURELEVELPARAMETERIVPROC                  gl3wGetTextureLevelParameteriv;
extern GL3_EXPORT PFNGLGETTEXTUREPARAMETERIIVPROC                      gl3wGetTextureParameterIiv;
extern GL3_EXPORT PFNGLGETTEXTUREPARAMETERIUIVPROC                     gl3wGetTextureParameterIuiv;
extern GL3_EXPORT PFNGLGETTEXTUREPARAMETERFVPROC                       gl3wGetTextureParameterfv;
extern GL3_EXPORT PFNGLGETTEXTUREPARAMETERIVPROC                       gl3wGetTextureParameteriv;
extern GL3_EXPORT PFNGLGETTEXTURESAMPLERHANDLEARBPROC                  gl3wGetTextureSamplerHandleARB;
extern GL3_EXPORT PFNGLGETTEXTURESUBIMAGEPROC                          gl3wGetTextureSubImage;
extern GL3_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                 gl3wGetTransformFeedbackVarying;
extern GL3_EXPORT PFNGLGETTRANSFORMFEEDBACKI64_VPROC                   gl3wGetTransformFeedbacki64_v;
extern GL3_EXPORT PFNGLGETTRANSFORMFEEDBACKI_VPROC                     gl3wGetTransformFeedbacki_v;
extern GL3_EXPORT PFNGLGETTRANSFORMFEEDBACKIVPROC                      gl3wGetTransformFeedbackiv;
extern GL3_EXPORT PFNGLGETUNIFORMBLOCKINDEXPROC                        gl3wGetUniformBlockIndex;
extern GL3_EXPORT PFNGLGETUNIFORMINDICESPROC                           gl3wGetUniformIndices;
extern GL3_EXPORT PFNGLGETUNIFORMLOCATIONPROC                          gl3wGetUniformLocation;
extern GL3_EXPORT PFNGLGETUNIFORMSUBROUTINEUIVPROC                     gl3wGetUniformSubroutineuiv;
extern GL3_EXPORT PFNGLGETUNIFORMDVPROC                                gl3wGetUniformdv;
extern GL3_EXPORT PFNGLGETUNIFORMFVPROC                                gl3wGetUniformfv;
extern GL3_EXPORT PFNGLGETUNIFORMIVPROC                                gl3wGetUniformiv;
extern GL3_EXPORT PFNGLGETUNIFORMUIVPROC                               gl3wGetUniformuiv;
extern GL3_EXPORT PFNGLGETVERTEXARRAYINDEXED64IVPROC                   gl3wGetVertexArrayIndexed64iv;
extern GL3_EXPORT PFNGLGETVERTEXARRAYINDEXEDIVPROC                     gl3wGetVertexArrayIndexediv;
extern GL3_EXPORT PFNGLGETVERTEXARRAYIVPROC                            gl3wGetVertexArrayiv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBIIVPROC                          gl3wGetVertexAttribIiv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBIUIVPROC                         gl3wGetVertexAttribIuiv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBLDVPROC                          gl3wGetVertexAttribLdv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBLUI64VARBPROC                    gl3wGetVertexAttribLui64vARB;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBPOINTERVPROC                     gl3wGetVertexAttribPointerv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBDVPROC                           gl3wGetVertexAttribdv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBFVPROC                           gl3wGetVertexAttribfv;
extern GL3_EXPORT PFNGLGETVERTEXATTRIBIVPROC                           gl3wGetVertexAttribiv;
extern GL3_EXPORT PFNGLGETNCOMPRESSEDTEXIMAGEPROC                      gl3wGetnCompressedTexImage;
extern GL3_EXPORT PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC                   gl3wGetnCompressedTexImageARB;
extern GL3_EXPORT PFNGLGETNTEXIMAGEPROC                                gl3wGetnTexImage;
extern GL3_EXPORT PFNGLGETNTEXIMAGEARBPROC                             gl3wGetnTexImageARB;
extern GL3_EXPORT PFNGLGETNUNIFORMDVPROC                               gl3wGetnUniformdv;
extern GL3_EXPORT PFNGLGETNUNIFORMDVARBPROC                            gl3wGetnUniformdvARB;
extern GL3_EXPORT PFNGLGETNUNIFORMFVPROC                               gl3wGetnUniformfv;
extern GL3_EXPORT PFNGLGETNUNIFORMFVARBPROC                            gl3wGetnUniformfvARB;
extern GL3_EXPORT PFNGLGETNUNIFORMIVPROC                               gl3wGetnUniformiv;
extern GL3_EXPORT PFNGLGETNUNIFORMIVARBPROC                            gl3wGetnUniformivARB;
extern GL3_EXPORT PFNGLGETNUNIFORMUIVPROC                              gl3wGetnUniformuiv;
extern GL3_EXPORT PFNGLGETNUNIFORMUIVARBPROC                           gl3wGetnUniformuivARB;
extern GL3_EXPORT PFNGLHINTPROC                                        gl3wHint;
extern GL3_EXPORT PFNGLINVALIDATEBUFFERDATAPROC                        gl3wInvalidateBufferData;
extern GL3_EXPORT PFNGLINVALIDATEBUFFERSUBDATAPROC                     gl3wInvalidateBufferSubData;
extern GL3_EXPORT PFNGLINVALIDATEFRAMEBUFFERPROC                       gl3wInvalidateFramebuffer;
extern GL3_EXPORT PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC              gl3wInvalidateNamedFramebufferData;
extern GL3_EXPORT PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC           gl3wInvalidateNamedFramebufferSubData;
extern GL3_EXPORT PFNGLINVALIDATESUBFRAMEBUFFERPROC                    gl3wInvalidateSubFramebuffer;
extern GL3_EXPORT PFNGLINVALIDATETEXIMAGEPROC                          gl3wInvalidateTexImage;
extern GL3_EXPORT PFNGLINVALIDATETEXSUBIMAGEPROC                       gl3wInvalidateTexSubImage;
extern GL3_EXPORT PFNGLISBUFFERPROC                                    gl3wIsBuffer;
extern GL3_EXPORT PFNGLISENABLEDPROC                                   gl3wIsEnabled;
extern GL3_EXPORT PFNGLISENABLEDIPROC                                  gl3wIsEnabledi;
extern GL3_EXPORT PFNGLISFRAMEBUFFERPROC                               gl3wIsFramebuffer;
extern GL3_EXPORT PFNGLISIMAGEHANDLERESIDENTARBPROC                    gl3wIsImageHandleResidentARB;
extern GL3_EXPORT PFNGLISNAMEDSTRINGARBPROC                            gl3wIsNamedStringARB;
extern GL3_EXPORT PFNGLISPROGRAMPROC                                   gl3wIsProgram;
extern GL3_EXPORT PFNGLISPROGRAMPIPELINEPROC                           gl3wIsProgramPipeline;
extern GL3_EXPORT PFNGLISQUERYPROC                                     gl3wIsQuery;
extern GL3_EXPORT PFNGLISRENDERBUFFERPROC                              gl3wIsRenderbuffer;
extern GL3_EXPORT PFNGLISSAMPLERPROC                                   gl3wIsSampler;
extern GL3_EXPORT PFNGLISSHADERPROC                                    gl3wIsShader;
extern GL3_EXPORT PFNGLISSYNCPROC                                      gl3wIsSync;
extern GL3_EXPORT PFNGLISTEXTUREPROC                                   gl3wIsTexture;
extern GL3_EXPORT PFNGLISTEXTUREHANDLERESIDENTARBPROC                  gl3wIsTextureHandleResidentARB;
extern GL3_EXPORT PFNGLISTRANSFORMFEEDBACKPROC                         gl3wIsTransformFeedback;
extern GL3_EXPORT PFNGLISVERTEXARRAYPROC                               gl3wIsVertexArray;
extern GL3_EXPORT PFNGLLINEWIDTHPROC                                   gl3wLineWidth;
extern GL3_EXPORT PFNGLLINKPROGRAMPROC                                 gl3wLinkProgram;
extern GL3_EXPORT PFNGLLOGICOPPROC                                     gl3wLogicOp;
extern GL3_EXPORT PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC               gl3wMakeImageHandleNonResidentARB;
extern GL3_EXPORT PFNGLMAKEIMAGEHANDLERESIDENTARBPROC                  gl3wMakeImageHandleResidentARB;
extern GL3_EXPORT PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC             gl3wMakeTextureHandleNonResidentARB;
extern GL3_EXPORT PFNGLMAKETEXTUREHANDLERESIDENTARBPROC                gl3wMakeTextureHandleResidentARB;
extern GL3_EXPORT PFNGLMAPBUFFERPROC                                   gl3wMapBuffer;
extern GL3_EXPORT PFNGLMAPBUFFERRANGEPROC                              gl3wMapBufferRange;
extern GL3_EXPORT PFNGLMAPNAMEDBUFFERPROC                              gl3wMapNamedBuffer;
extern GL3_EXPORT PFNGLMAPNAMEDBUFFERRANGEPROC                         gl3wMapNamedBufferRange;
extern GL3_EXPORT PFNGLMEMORYBARRIERPROC                               gl3wMemoryBarrier;
extern GL3_EXPORT PFNGLMEMORYBARRIERBYREGIONPROC                       gl3wMemoryBarrierByRegion;
extern GL3_EXPORT PFNGLMINSAMPLESHADINGPROC                            gl3wMinSampleShading;
extern GL3_EXPORT PFNGLMINSAMPLESHADINGARBPROC                         gl3wMinSampleShadingARB;
extern GL3_EXPORT PFNGLMULTIDRAWARRAYSPROC                             gl3wMultiDrawArrays;
extern GL3_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTPROC                     gl3wMultiDrawArraysIndirect;
extern GL3_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC             gl3wMultiDrawArraysIndirectCountARB;
extern GL3_EXPORT PFNGLMULTIDRAWELEMENTSPROC                           gl3wMultiDrawElements;
extern GL3_EXPORT PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                 gl3wMultiDrawElementsBaseVertex;
extern GL3_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTPROC                   gl3wMultiDrawElementsIndirect;
extern GL3_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC           gl3wMultiDrawElementsIndirectCountARB;
extern GL3_EXPORT PFNGLNAMEDBUFFERDATAPROC                             gl3wNamedBufferData;
extern GL3_EXPORT PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC                gl3wNamedBufferPageCommitmentARB;
extern GL3_EXPORT PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC                gl3wNamedBufferPageCommitmentEXT;
extern GL3_EXPORT PFNGLNAMEDBUFFERSTORAGEPROC                          gl3wNamedBufferStorage;
extern GL3_EXPORT PFNGLNAMEDBUFFERSUBDATAPROC                          gl3wNamedBufferSubData;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC                  gl3wNamedFramebufferDrawBuffer;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC                 gl3wNamedFramebufferDrawBuffers;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC                  gl3wNamedFramebufferParameteri;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC                  gl3wNamedFramebufferReadBuffer;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC                gl3wNamedFramebufferRenderbuffer;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                     gl3wNamedFramebufferTexture;
extern GL3_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC                gl3wNamedFramebufferTextureLayer;
extern GL3_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEPROC                    gl3wNamedRenderbufferStorage;
extern GL3_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC         gl3wNamedRenderbufferStorageMultisample;
extern GL3_EXPORT PFNGLNAMEDSTRINGARBPROC                              gl3wNamedStringARB;
extern GL3_EXPORT PFNGLOBJECTLABELPROC                                 gl3wObjectLabel;
extern GL3_EXPORT PFNGLOBJECTPTRLABELPROC                              gl3wObjectPtrLabel;
extern GL3_EXPORT PFNGLPATCHPARAMETERFVPROC                            gl3wPatchParameterfv;
extern GL3_EXPORT PFNGLPATCHPARAMETERIPROC                             gl3wPatchParameteri;
extern GL3_EXPORT PFNGLPAUSETRANSFORMFEEDBACKPROC                      gl3wPauseTransformFeedback;
extern GL3_EXPORT PFNGLPIXELSTOREFPROC                                 gl3wPixelStoref;
extern GL3_EXPORT PFNGLPIXELSTOREIPROC                                 gl3wPixelStorei;
extern GL3_EXPORT PFNGLPOINTPARAMETERFPROC                             gl3wPointParameterf;
extern GL3_EXPORT PFNGLPOINTPARAMETERFVPROC                            gl3wPointParameterfv;
extern GL3_EXPORT PFNGLPOINTPARAMETERIPROC                             gl3wPointParameteri;
extern GL3_EXPORT PFNGLPOINTPARAMETERIVPROC                            gl3wPointParameteriv;
extern GL3_EXPORT PFNGLPOINTSIZEPROC                                   gl3wPointSize;
extern GL3_EXPORT PFNGLPOLYGONMODEPROC                                 gl3wPolygonMode;
extern GL3_EXPORT PFNGLPOLYGONOFFSETPROC                               gl3wPolygonOffset;
extern GL3_EXPORT PFNGLPOPDEBUGGROUPPROC                               gl3wPopDebugGroup;
extern GL3_EXPORT PFNGLPRIMITIVERESTARTINDEXPROC                       gl3wPrimitiveRestartIndex;
extern GL3_EXPORT PFNGLPROGRAMBINARYPROC                               gl3wProgramBinary;
extern GL3_EXPORT PFNGLPROGRAMPARAMETERIPROC                           gl3wProgramParameteri;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1DPROC                            gl3wProgramUniform1d;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1DVPROC                           gl3wProgramUniform1dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1FPROC                            gl3wProgramUniform1f;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1FVPROC                           gl3wProgramUniform1fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1IPROC                            gl3wProgramUniform1i;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1IVPROC                           gl3wProgramUniform1iv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1UIPROC                           gl3wProgramUniform1ui;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM1UIVPROC                          gl3wProgramUniform1uiv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2DPROC                            gl3wProgramUniform2d;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2DVPROC                           gl3wProgramUniform2dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2FPROC                            gl3wProgramUniform2f;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2FVPROC                           gl3wProgramUniform2fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2IPROC                            gl3wProgramUniform2i;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2IVPROC                           gl3wProgramUniform2iv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2UIPROC                           gl3wProgramUniform2ui;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM2UIVPROC                          gl3wProgramUniform2uiv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3DPROC                            gl3wProgramUniform3d;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3DVPROC                           gl3wProgramUniform3dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3FPROC                            gl3wProgramUniform3f;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3FVPROC                           gl3wProgramUniform3fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3IPROC                            gl3wProgramUniform3i;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3IVPROC                           gl3wProgramUniform3iv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3UIPROC                           gl3wProgramUniform3ui;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM3UIVPROC                          gl3wProgramUniform3uiv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4DPROC                            gl3wProgramUniform4d;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4DVPROC                           gl3wProgramUniform4dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4FPROC                            gl3wProgramUniform4f;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4FVPROC                           gl3wProgramUniform4fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4IPROC                            gl3wProgramUniform4i;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4IVPROC                           gl3wProgramUniform4iv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4UIPROC                           gl3wProgramUniform4ui;
extern GL3_EXPORT PFNGLPROGRAMUNIFORM4UIVPROC                          gl3wProgramUniform4uiv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC                 gl3wProgramUniformHandleui64ARB;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC                gl3wProgramUniformHandleui64vARB;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2DVPROC                     gl3wProgramUniformMatrix2dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVPROC                     gl3wProgramUniformMatrix2fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC                   gl3wProgramUniformMatrix2x3dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC                   gl3wProgramUniformMatrix2x3fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC                   gl3wProgramUniformMatrix2x4dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC                   gl3wProgramUniformMatrix2x4fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3DVPROC                     gl3wProgramUniformMatrix3dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVPROC                     gl3wProgramUniformMatrix3fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC                   gl3wProgramUniformMatrix3x2dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC                   gl3wProgramUniformMatrix3x2fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC                   gl3wProgramUniformMatrix3x4dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC                   gl3wProgramUniformMatrix3x4fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4DVPROC                     gl3wProgramUniformMatrix4dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVPROC                     gl3wProgramUniformMatrix4fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC                   gl3wProgramUniformMatrix4x2dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC                   gl3wProgramUniformMatrix4x2fv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC                   gl3wProgramUniformMatrix4x3dv;
extern GL3_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC                   gl3wProgramUniformMatrix4x3fv;
extern GL3_EXPORT PFNGLPROVOKINGVERTEXPROC                             gl3wProvokingVertex;
extern GL3_EXPORT PFNGLPUSHDEBUGGROUPPROC                              gl3wPushDebugGroup;
extern GL3_EXPORT PFNGLQUERYCOUNTERPROC                                gl3wQueryCounter;
extern GL3_EXPORT PFNGLREADBUFFERPROC                                  gl3wReadBuffer;
extern GL3_EXPORT PFNGLREADPIXELSPROC                                  gl3wReadPixels;
extern GL3_EXPORT PFNGLREADNPIXELSPROC                                 gl3wReadnPixels;
extern GL3_EXPORT PFNGLREADNPIXELSARBPROC                              gl3wReadnPixelsARB;
extern GL3_EXPORT PFNGLRELEASESHADERCOMPILERPROC                       gl3wReleaseShaderCompiler;
extern GL3_EXPORT PFNGLRENDERBUFFERSTORAGEPROC                         gl3wRenderbufferStorage;
extern GL3_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC              gl3wRenderbufferStorageMultisample;
extern GL3_EXPORT PFNGLRESUMETRANSFORMFEEDBACKPROC                     gl3wResumeTransformFeedback;
extern GL3_EXPORT PFNGLSAMPLECOVERAGEPROC                              gl3wSampleCoverage;
extern GL3_EXPORT PFNGLSAMPLEMASKIPROC                                 gl3wSampleMaski;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERIIVPROC                         gl3wSamplerParameterIiv;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERIUIVPROC                        gl3wSamplerParameterIuiv;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERFPROC                           gl3wSamplerParameterf;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERFVPROC                          gl3wSamplerParameterfv;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERIPROC                           gl3wSamplerParameteri;
extern GL3_EXPORT PFNGLSAMPLERPARAMETERIVPROC                          gl3wSamplerParameteriv;
extern GL3_EXPORT PFNGLSCISSORPROC                                     gl3wScissor;
extern GL3_EXPORT PFNGLSCISSORARRAYVPROC                               gl3wScissorArrayv;
extern GL3_EXPORT PFNGLSCISSORINDEXEDPROC                              gl3wScissorIndexed;
extern GL3_EXPORT PFNGLSCISSORINDEXEDVPROC                             gl3wScissorIndexedv;
extern GL3_EXPORT PFNGLSHADERBINARYPROC                                gl3wShaderBinary;
extern GL3_EXPORT PFNGLSHADERSOURCEPROC                                gl3wShaderSource;
extern GL3_EXPORT PFNGLSHADERSTORAGEBLOCKBINDINGPROC                   gl3wShaderStorageBlockBinding;
extern GL3_EXPORT PFNGLSTENCILFUNCPROC                                 gl3wStencilFunc;
extern GL3_EXPORT PFNGLSTENCILFUNCSEPARATEPROC                         gl3wStencilFuncSeparate;
extern GL3_EXPORT PFNGLSTENCILMASKPROC                                 gl3wStencilMask;
extern GL3_EXPORT PFNGLSTENCILMASKSEPARATEPROC                         gl3wStencilMaskSeparate;
extern GL3_EXPORT PFNGLSTENCILOPPROC                                   gl3wStencilOp;
extern GL3_EXPORT PFNGLSTENCILOPSEPARATEPROC                           gl3wStencilOpSeparate;
extern GL3_EXPORT PFNGLTEXBUFFERPROC                                   gl3wTexBuffer;
extern GL3_EXPORT PFNGLTEXBUFFERRANGEPROC                              gl3wTexBufferRange;
extern GL3_EXPORT PFNGLTEXIMAGE1DPROC                                  gl3wTexImage1D;
extern GL3_EXPORT PFNGLTEXIMAGE2DPROC                                  gl3wTexImage2D;
extern GL3_EXPORT PFNGLTEXIMAGE2DMULTISAMPLEPROC                       gl3wTexImage2DMultisample;
extern GL3_EXPORT PFNGLTEXIMAGE3DPROC                                  gl3wTexImage3D;
extern GL3_EXPORT PFNGLTEXIMAGE3DMULTISAMPLEPROC                       gl3wTexImage3DMultisample;
extern GL3_EXPORT PFNGLTEXPAGECOMMITMENTARBPROC                        gl3wTexPageCommitmentARB;
extern GL3_EXPORT PFNGLTEXPARAMETERIIVPROC                             gl3wTexParameterIiv;
extern GL3_EXPORT PFNGLTEXPARAMETERIUIVPROC                            gl3wTexParameterIuiv;
extern GL3_EXPORT PFNGLTEXPARAMETERFPROC                               gl3wTexParameterf;
extern GL3_EXPORT PFNGLTEXPARAMETERFVPROC                              gl3wTexParameterfv;
extern GL3_EXPORT PFNGLTEXPARAMETERIPROC                               gl3wTexParameteri;
extern GL3_EXPORT PFNGLTEXPARAMETERIVPROC                              gl3wTexParameteriv;
extern GL3_EXPORT PFNGLTEXSTORAGE1DPROC                                gl3wTexStorage1D;
extern GL3_EXPORT PFNGLTEXSTORAGE2DPROC                                gl3wTexStorage2D;
extern GL3_EXPORT PFNGLTEXSTORAGE2DMULTISAMPLEPROC                     gl3wTexStorage2DMultisample;
extern GL3_EXPORT PFNGLTEXSTORAGE3DPROC                                gl3wTexStorage3D;
extern GL3_EXPORT PFNGLTEXSTORAGE3DMULTISAMPLEPROC                     gl3wTexStorage3DMultisample;
extern GL3_EXPORT PFNGLTEXSUBIMAGE1DPROC                               gl3wTexSubImage1D;
extern GL3_EXPORT PFNGLTEXSUBIMAGE2DPROC                               gl3wTexSubImage2D;
extern GL3_EXPORT PFNGLTEXSUBIMAGE3DPROC                               gl3wTexSubImage3D;
extern GL3_EXPORT PFNGLTEXTUREBARRIERPROC                              gl3wTextureBarrier;
extern GL3_EXPORT PFNGLTEXTUREBUFFERPROC                               gl3wTextureBuffer;
extern GL3_EXPORT PFNGLTEXTUREBUFFERRANGEPROC                          gl3wTextureBufferRange;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERIIVPROC                         gl3wTextureParameterIiv;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERIUIVPROC                        gl3wTextureParameterIuiv;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERFPROC                           gl3wTextureParameterf;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERFVPROC                          gl3wTextureParameterfv;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERIPROC                           gl3wTextureParameteri;
extern GL3_EXPORT PFNGLTEXTUREPARAMETERIVPROC                          gl3wTextureParameteriv;
extern GL3_EXPORT PFNGLTEXTURESTORAGE1DPROC                            gl3wTextureStorage1D;
extern GL3_EXPORT PFNGLTEXTURESTORAGE2DPROC                            gl3wTextureStorage2D;
extern GL3_EXPORT PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC                 gl3wTextureStorage2DMultisample;
extern GL3_EXPORT PFNGLTEXTURESTORAGE3DPROC                            gl3wTextureStorage3D;
extern GL3_EXPORT PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC                 gl3wTextureStorage3DMultisample;
extern GL3_EXPORT PFNGLTEXTURESUBIMAGE1DPROC                           gl3wTextureSubImage1D;
extern GL3_EXPORT PFNGLTEXTURESUBIMAGE2DPROC                           gl3wTextureSubImage2D;
extern GL3_EXPORT PFNGLTEXTURESUBIMAGE3DPROC                           gl3wTextureSubImage3D;
extern GL3_EXPORT PFNGLTEXTUREVIEWPROC                                 gl3wTextureView;
extern GL3_EXPORT PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC                 gl3wTransformFeedbackBufferBase;
extern GL3_EXPORT PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC                gl3wTransformFeedbackBufferRange;
extern GL3_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSPROC                   gl3wTransformFeedbackVaryings;
extern GL3_EXPORT PFNGLUNIFORM1DPROC                                   gl3wUniform1d;
extern GL3_EXPORT PFNGLUNIFORM1DVPROC                                  gl3wUniform1dv;
extern GL3_EXPORT PFNGLUNIFORM1FPROC                                   gl3wUniform1f;
extern GL3_EXPORT PFNGLUNIFORM1FVPROC                                  gl3wUniform1fv;
extern GL3_EXPORT PFNGLUNIFORM1IPROC                                   gl3wUniform1i;
extern GL3_EXPORT PFNGLUNIFORM1IVPROC                                  gl3wUniform1iv;
extern GL3_EXPORT PFNGLUNIFORM1UIPROC                                  gl3wUniform1ui;
extern GL3_EXPORT PFNGLUNIFORM1UIVPROC                                 gl3wUniform1uiv;
extern GL3_EXPORT PFNGLUNIFORM2DPROC                                   gl3wUniform2d;
extern GL3_EXPORT PFNGLUNIFORM2DVPROC                                  gl3wUniform2dv;
extern GL3_EXPORT PFNGLUNIFORM2FPROC                                   gl3wUniform2f;
extern GL3_EXPORT PFNGLUNIFORM2FVPROC                                  gl3wUniform2fv;
extern GL3_EXPORT PFNGLUNIFORM2IPROC                                   gl3wUniform2i;
extern GL3_EXPORT PFNGLUNIFORM2IVPROC                                  gl3wUniform2iv;
extern GL3_EXPORT PFNGLUNIFORM2UIPROC                                  gl3wUniform2ui;
extern GL3_EXPORT PFNGLUNIFORM2UIVPROC                                 gl3wUniform2uiv;
extern GL3_EXPORT PFNGLUNIFORM3DPROC                                   gl3wUniform3d;
extern GL3_EXPORT PFNGLUNIFORM3DVPROC                                  gl3wUniform3dv;
extern GL3_EXPORT PFNGLUNIFORM3FPROC                                   gl3wUniform3f;
extern GL3_EXPORT PFNGLUNIFORM3FVPROC                                  gl3wUniform3fv;
extern GL3_EXPORT PFNGLUNIFORM3IPROC                                   gl3wUniform3i;
extern GL3_EXPORT PFNGLUNIFORM3IVPROC                                  gl3wUniform3iv;
extern GL3_EXPORT PFNGLUNIFORM3UIPROC                                  gl3wUniform3ui;
extern GL3_EXPORT PFNGLUNIFORM3UIVPROC                                 gl3wUniform3uiv;
extern GL3_EXPORT PFNGLUNIFORM4DPROC                                   gl3wUniform4d;
extern GL3_EXPORT PFNGLUNIFORM4DVPROC                                  gl3wUniform4dv;
extern GL3_EXPORT PFNGLUNIFORM4FPROC                                   gl3wUniform4f;
extern GL3_EXPORT PFNGLUNIFORM4FVPROC                                  gl3wUniform4fv;
extern GL3_EXPORT PFNGLUNIFORM4IPROC                                   gl3wUniform4i;
extern GL3_EXPORT PFNGLUNIFORM4IVPROC                                  gl3wUniform4iv;
extern GL3_EXPORT PFNGLUNIFORM4UIPROC                                  gl3wUniform4ui;
extern GL3_EXPORT PFNGLUNIFORM4UIVPROC                                 gl3wUniform4uiv;
extern GL3_EXPORT PFNGLUNIFORMBLOCKBINDINGPROC                         gl3wUniformBlockBinding;
extern GL3_EXPORT PFNGLUNIFORMHANDLEUI64ARBPROC                        gl3wUniformHandleui64ARB;
extern GL3_EXPORT PFNGLUNIFORMHANDLEUI64VARBPROC                       gl3wUniformHandleui64vARB;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2DVPROC                            gl3wUniformMatrix2dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2FVPROC                            gl3wUniformMatrix2fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2X3DVPROC                          gl3wUniformMatrix2x3dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2X3FVPROC                          gl3wUniformMatrix2x3fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2X4DVPROC                          gl3wUniformMatrix2x4dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX2X4FVPROC                          gl3wUniformMatrix2x4fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3DVPROC                            gl3wUniformMatrix3dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3FVPROC                            gl3wUniformMatrix3fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3X2DVPROC                          gl3wUniformMatrix3x2dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3X2FVPROC                          gl3wUniformMatrix3x2fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3X4DVPROC                          gl3wUniformMatrix3x4dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX3X4FVPROC                          gl3wUniformMatrix3x4fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4DVPROC                            gl3wUniformMatrix4dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4FVPROC                            gl3wUniformMatrix4fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4X2DVPROC                          gl3wUniformMatrix4x2dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4X2FVPROC                          gl3wUniformMatrix4x2fv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4X3DVPROC                          gl3wUniformMatrix4x3dv;
extern GL3_EXPORT PFNGLUNIFORMMATRIX4X3FVPROC                          gl3wUniformMatrix4x3fv;
extern GL3_EXPORT PFNGLUNIFORMSUBROUTINESUIVPROC                       gl3wUniformSubroutinesuiv;
extern GL3_EXPORT PFNGLUNMAPBUFFERPROC                                 gl3wUnmapBuffer;
extern GL3_EXPORT PFNGLUNMAPNAMEDBUFFERPROC                            gl3wUnmapNamedBuffer;
extern GL3_EXPORT PFNGLUSEPROGRAMPROC                                  gl3wUseProgram;
extern GL3_EXPORT PFNGLUSEPROGRAMSTAGESPROC                            gl3wUseProgramStages;
extern GL3_EXPORT PFNGLVALIDATEPROGRAMPROC                             gl3wValidateProgram;
extern GL3_EXPORT PFNGLVALIDATEPROGRAMPIPELINEPROC                     gl3wValidateProgramPipeline;
extern GL3_EXPORT PFNGLVERTEXARRAYATTRIBBINDINGPROC                    gl3wVertexArrayAttribBinding;
extern GL3_EXPORT PFNGLVERTEXARRAYATTRIBFORMATPROC                     gl3wVertexArrayAttribFormat;
extern GL3_EXPORT PFNGLVERTEXARRAYATTRIBIFORMATPROC                    gl3wVertexArrayAttribIFormat;
extern GL3_EXPORT PFNGLVERTEXARRAYATTRIBLFORMATPROC                    gl3wVertexArrayAttribLFormat;
extern GL3_EXPORT PFNGLVERTEXARRAYBINDINGDIVISORPROC                   gl3wVertexArrayBindingDivisor;
extern GL3_EXPORT PFNGLVERTEXARRAYELEMENTBUFFERPROC                    gl3wVertexArrayElementBuffer;
extern GL3_EXPORT PFNGLVERTEXARRAYVERTEXBUFFERPROC                     gl3wVertexArrayVertexBuffer;
extern GL3_EXPORT PFNGLVERTEXARRAYVERTEXBUFFERSPROC                    gl3wVertexArrayVertexBuffers;
extern GL3_EXPORT PFNGLVERTEXATTRIB1DPROC                              gl3wVertexAttrib1d;
extern GL3_EXPORT PFNGLVERTEXATTRIB1DVPROC                             gl3wVertexAttrib1dv;
extern GL3_EXPORT PFNGLVERTEXATTRIB1FPROC                              gl3wVertexAttrib1f;
extern GL3_EXPORT PFNGLVERTEXATTRIB1FVPROC                             gl3wVertexAttrib1fv;
extern GL3_EXPORT PFNGLVERTEXATTRIB1SPROC                              gl3wVertexAttrib1s;
extern GL3_EXPORT PFNGLVERTEXATTRIB1SVPROC                             gl3wVertexAttrib1sv;
extern GL3_EXPORT PFNGLVERTEXATTRIB2DPROC                              gl3wVertexAttrib2d;
extern GL3_EXPORT PFNGLVERTEXATTRIB2DVPROC                             gl3wVertexAttrib2dv;
extern GL3_EXPORT PFNGLVERTEXATTRIB2FPROC                              gl3wVertexAttrib2f;
extern GL3_EXPORT PFNGLVERTEXATTRIB2FVPROC                             gl3wVertexAttrib2fv;
extern GL3_EXPORT PFNGLVERTEXATTRIB2SPROC                              gl3wVertexAttrib2s;
extern GL3_EXPORT PFNGLVERTEXATTRIB2SVPROC                             gl3wVertexAttrib2sv;
extern GL3_EXPORT PFNGLVERTEXATTRIB3DPROC                              gl3wVertexAttrib3d;
extern GL3_EXPORT PFNGLVERTEXATTRIB3DVPROC                             gl3wVertexAttrib3dv;
extern GL3_EXPORT PFNGLVERTEXATTRIB3FPROC                              gl3wVertexAttrib3f;
extern GL3_EXPORT PFNGLVERTEXATTRIB3FVPROC                             gl3wVertexAttrib3fv;
extern GL3_EXPORT PFNGLVERTEXATTRIB3SPROC                              gl3wVertexAttrib3s;
extern GL3_EXPORT PFNGLVERTEXATTRIB3SVPROC                             gl3wVertexAttrib3sv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NBVPROC                            gl3wVertexAttrib4Nbv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NIVPROC                            gl3wVertexAttrib4Niv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NSVPROC                            gl3wVertexAttrib4Nsv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NUBPROC                            gl3wVertexAttrib4Nub;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NUBVPROC                           gl3wVertexAttrib4Nubv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NUIVPROC                           gl3wVertexAttrib4Nuiv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4NUSVPROC                           gl3wVertexAttrib4Nusv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4BVPROC                             gl3wVertexAttrib4bv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4DPROC                              gl3wVertexAttrib4d;
extern GL3_EXPORT PFNGLVERTEXATTRIB4DVPROC                             gl3wVertexAttrib4dv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4FPROC                              gl3wVertexAttrib4f;
extern GL3_EXPORT PFNGLVERTEXATTRIB4FVPROC                             gl3wVertexAttrib4fv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4IVPROC                             gl3wVertexAttrib4iv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4SPROC                              gl3wVertexAttrib4s;
extern GL3_EXPORT PFNGLVERTEXATTRIB4SVPROC                             gl3wVertexAttrib4sv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4UBVPROC                            gl3wVertexAttrib4ubv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4UIVPROC                            gl3wVertexAttrib4uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIB4USVPROC                            gl3wVertexAttrib4usv;
extern GL3_EXPORT PFNGLVERTEXATTRIBBINDINGPROC                         gl3wVertexAttribBinding;
extern GL3_EXPORT PFNGLVERTEXATTRIBDIVISORPROC                         gl3wVertexAttribDivisor;
extern GL3_EXPORT PFNGLVERTEXATTRIBFORMATPROC                          gl3wVertexAttribFormat;
extern GL3_EXPORT PFNGLVERTEXATTRIBI1IPROC                             gl3wVertexAttribI1i;
extern GL3_EXPORT PFNGLVERTEXATTRIBI1IVPROC                            gl3wVertexAttribI1iv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI1UIPROC                            gl3wVertexAttribI1ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBI1UIVPROC                           gl3wVertexAttribI1uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI2IPROC                             gl3wVertexAttribI2i;
extern GL3_EXPORT PFNGLVERTEXATTRIBI2IVPROC                            gl3wVertexAttribI2iv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI2UIPROC                            gl3wVertexAttribI2ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBI2UIVPROC                           gl3wVertexAttribI2uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI3IPROC                             gl3wVertexAttribI3i;
extern GL3_EXPORT PFNGLVERTEXATTRIBI3IVPROC                            gl3wVertexAttribI3iv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI3UIPROC                            gl3wVertexAttribI3ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBI3UIVPROC                           gl3wVertexAttribI3uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4BVPROC                            gl3wVertexAttribI4bv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4IPROC                             gl3wVertexAttribI4i;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4IVPROC                            gl3wVertexAttribI4iv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4SVPROC                            gl3wVertexAttribI4sv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4UBVPROC                           gl3wVertexAttribI4ubv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4UIPROC                            gl3wVertexAttribI4ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4UIVPROC                           gl3wVertexAttribI4uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBI4USVPROC                           gl3wVertexAttribI4usv;
extern GL3_EXPORT PFNGLVERTEXATTRIBIFORMATPROC                         gl3wVertexAttribIFormat;
extern GL3_EXPORT PFNGLVERTEXATTRIBIPOINTERPROC                        gl3wVertexAttribIPointer;
extern GL3_EXPORT PFNGLVERTEXATTRIBL1DPROC                             gl3wVertexAttribL1d;
extern GL3_EXPORT PFNGLVERTEXATTRIBL1DVPROC                            gl3wVertexAttribL1dv;
extern GL3_EXPORT PFNGLVERTEXATTRIBL1UI64ARBPROC                       gl3wVertexAttribL1ui64ARB;
extern GL3_EXPORT PFNGLVERTEXATTRIBL1UI64VARBPROC                      gl3wVertexAttribL1ui64vARB;
extern GL3_EXPORT PFNGLVERTEXATTRIBL2DPROC                             gl3wVertexAttribL2d;
extern GL3_EXPORT PFNGLVERTEXATTRIBL2DVPROC                            gl3wVertexAttribL2dv;
extern GL3_EXPORT PFNGLVERTEXATTRIBL3DPROC                             gl3wVertexAttribL3d;
extern GL3_EXPORT PFNGLVERTEXATTRIBL3DVPROC                            gl3wVertexAttribL3dv;
extern GL3_EXPORT PFNGLVERTEXATTRIBL4DPROC                             gl3wVertexAttribL4d;
extern GL3_EXPORT PFNGLVERTEXATTRIBL4DVPROC                            gl3wVertexAttribL4dv;
extern GL3_EXPORT PFNGLVERTEXATTRIBLFORMATPROC                         gl3wVertexAttribLFormat;
extern GL3_EXPORT PFNGLVERTEXATTRIBLPOINTERPROC                        gl3wVertexAttribLPointer;
extern GL3_EXPORT PFNGLVERTEXATTRIBP1UIPROC                            gl3wVertexAttribP1ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBP1UIVPROC                           gl3wVertexAttribP1uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBP2UIPROC                            gl3wVertexAttribP2ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBP2UIVPROC                           gl3wVertexAttribP2uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBP3UIPROC                            gl3wVertexAttribP3ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBP3UIVPROC                           gl3wVertexAttribP3uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBP4UIPROC                            gl3wVertexAttribP4ui;
extern GL3_EXPORT PFNGLVERTEXATTRIBP4UIVPROC                           gl3wVertexAttribP4uiv;
extern GL3_EXPORT PFNGLVERTEXATTRIBPOINTERPROC                         gl3wVertexAttribPointer;
extern GL3_EXPORT PFNGLVERTEXBINDINGDIVISORPROC                        gl3wVertexBindingDivisor;
extern GL3_EXPORT PFNGLVIEWPORTPROC                                    gl3wViewport;
extern GL3_EXPORT PFNGLVIEWPORTARRAYVPROC                              gl3wViewportArrayv;
extern GL3_EXPORT PFNGLVIEWPORTINDEXEDFPROC                            gl3wViewportIndexedf;
extern GL3_EXPORT PFNGLVIEWPORTINDEXEDFVPROC                           gl3wViewportIndexedfv;
extern GL3_EXPORT PFNGLWAITSYNCPROC                                    gl3wWaitSync;

#define glActiveShaderProgram                         gl3wActiveShaderProgram
#define glActiveTexture                               gl3wActiveTexture
#define glAttachShader                                gl3wAttachShader
#define glBeginConditionalRender                      gl3wBeginConditionalRender
#define glBeginQuery                                  gl3wBeginQuery
#define glBeginQueryIndexed                           gl3wBeginQueryIndexed
#define glBeginTransformFeedback                      gl3wBeginTransformFeedback
#define glBindAttribLocation                          gl3wBindAttribLocation
#define glBindBuffer                                  gl3wBindBuffer
#define glBindBufferBase                              gl3wBindBufferBase
#define glBindBufferRange                             gl3wBindBufferRange
#define glBindBuffersBase                             gl3wBindBuffersBase
#define glBindBuffersRange                            gl3wBindBuffersRange
#define glBindFragDataLocation                        gl3wBindFragDataLocation
#define glBindFragDataLocationIndexed                 gl3wBindFragDataLocationIndexed
#define glBindFramebuffer                             gl3wBindFramebuffer
#define glBindImageTexture                            gl3wBindImageTexture
#define glBindImageTextures                           gl3wBindImageTextures
#define glBindProgramPipeline                         gl3wBindProgramPipeline
#define glBindRenderbuffer                            gl3wBindRenderbuffer
#define glBindSampler                                 gl3wBindSampler
#define glBindSamplers                                gl3wBindSamplers
#define glBindTexture                                 gl3wBindTexture
#define glBindTextureUnit                             gl3wBindTextureUnit
#define glBindTextures                                gl3wBindTextures
#define glBindTransformFeedback                       gl3wBindTransformFeedback
#define glBindVertexArray                             gl3wBindVertexArray
#define glBindVertexBuffer                            gl3wBindVertexBuffer
#define glBindVertexBuffers                           gl3wBindVertexBuffers
#define glBlendColor                                  gl3wBlendColor
#define glBlendEquation                               gl3wBlendEquation
#define glBlendEquationSeparate                       gl3wBlendEquationSeparate
#define glBlendEquationSeparatei                      gl3wBlendEquationSeparatei
#define glBlendEquationSeparateiARB                   gl3wBlendEquationSeparateiARB
#define glBlendEquationi                              gl3wBlendEquationi
#define glBlendEquationiARB                           gl3wBlendEquationiARB
#define glBlendFunc                                   gl3wBlendFunc
#define glBlendFuncSeparate                           gl3wBlendFuncSeparate
#define glBlendFuncSeparatei                          gl3wBlendFuncSeparatei
#define glBlendFuncSeparateiARB                       gl3wBlendFuncSeparateiARB
#define glBlendFunci                                  gl3wBlendFunci
#define glBlendFunciARB                               gl3wBlendFunciARB
#define glBlitFramebuffer                             gl3wBlitFramebuffer
#define glBlitNamedFramebuffer                        gl3wBlitNamedFramebuffer
#define glBufferData                                  gl3wBufferData
#define glBufferPageCommitmentARB                     gl3wBufferPageCommitmentARB
#define glBufferStorage                               gl3wBufferStorage
#define glBufferSubData                               gl3wBufferSubData
#define glCheckFramebufferStatus                      gl3wCheckFramebufferStatus
#define glCheckNamedFramebufferStatus                 gl3wCheckNamedFramebufferStatus
#define glClampColor                                  gl3wClampColor
#define glClear                                       gl3wClear
#define glClearBufferData                             gl3wClearBufferData
#define glClearBufferSubData                          gl3wClearBufferSubData
#define glClearBufferfi                               gl3wClearBufferfi
#define glClearBufferfv                               gl3wClearBufferfv
#define glClearBufferiv                               gl3wClearBufferiv
#define glClearBufferuiv                              gl3wClearBufferuiv
#define glClearColor                                  gl3wClearColor
#define glClearDepth                                  gl3wClearDepth
#define glClearDepthf                                 gl3wClearDepthf
#define glClearNamedBufferData                        gl3wClearNamedBufferData
#define glClearNamedBufferSubData                     gl3wClearNamedBufferSubData
#define glClearNamedFramebufferfi                     gl3wClearNamedFramebufferfi
#define glClearNamedFramebufferfv                     gl3wClearNamedFramebufferfv
#define glClearNamedFramebufferiv                     gl3wClearNamedFramebufferiv
#define glClearNamedFramebufferuiv                    gl3wClearNamedFramebufferuiv
#define glClearStencil                                gl3wClearStencil
#define glClearTexImage                               gl3wClearTexImage
#define glClearTexSubImage                            gl3wClearTexSubImage
#define glClientWaitSync                              gl3wClientWaitSync
#define glClipControl                                 gl3wClipControl
#define glColorMask                                   gl3wColorMask
#define glColorMaski                                  gl3wColorMaski
#define glCompileShader                               gl3wCompileShader
#define glCompileShaderIncludeARB                     gl3wCompileShaderIncludeARB
#define glCompressedTexImage1D                        gl3wCompressedTexImage1D
#define glCompressedTexImage2D                        gl3wCompressedTexImage2D
#define glCompressedTexImage3D                        gl3wCompressedTexImage3D
#define glCompressedTexSubImage1D                     gl3wCompressedTexSubImage1D
#define glCompressedTexSubImage2D                     gl3wCompressedTexSubImage2D
#define glCompressedTexSubImage3D                     gl3wCompressedTexSubImage3D
#define glCompressedTextureSubImage1D                 gl3wCompressedTextureSubImage1D
#define glCompressedTextureSubImage2D                 gl3wCompressedTextureSubImage2D
#define glCompressedTextureSubImage3D                 gl3wCompressedTextureSubImage3D
#define glCopyBufferSubData                           gl3wCopyBufferSubData
#define glCopyImageSubData                            gl3wCopyImageSubData
#define glCopyNamedBufferSubData                      gl3wCopyNamedBufferSubData
#define glCopyTexImage1D                              gl3wCopyTexImage1D
#define glCopyTexImage2D                              gl3wCopyTexImage2D
#define glCopyTexSubImage1D                           gl3wCopyTexSubImage1D
#define glCopyTexSubImage2D                           gl3wCopyTexSubImage2D
#define glCopyTexSubImage3D                           gl3wCopyTexSubImage3D
#define glCopyTextureSubImage1D                       gl3wCopyTextureSubImage1D
#define glCopyTextureSubImage2D                       gl3wCopyTextureSubImage2D
#define glCopyTextureSubImage3D                       gl3wCopyTextureSubImage3D
#define glCreateBuffers                               gl3wCreateBuffers
#define glCreateFramebuffers                          gl3wCreateFramebuffers
#define glCreateProgram                               gl3wCreateProgram
#define glCreateProgramPipelines                      gl3wCreateProgramPipelines
#define glCreateQueries                               gl3wCreateQueries
#define glCreateRenderbuffers                         gl3wCreateRenderbuffers
#define glCreateSamplers                              gl3wCreateSamplers
#define glCreateShader                                gl3wCreateShader
#define glCreateShaderProgramv                        gl3wCreateShaderProgramv
#define glCreateSyncFromCLeventARB                    gl3wCreateSyncFromCLeventARB
#define glCreateTextures                              gl3wCreateTextures
#define glCreateTransformFeedbacks                    gl3wCreateTransformFeedbacks
#define glCreateVertexArrays                          gl3wCreateVertexArrays
#define glCullFace                                    gl3wCullFace
#define glDebugMessageCallback                        gl3wDebugMessageCallback
#define glDebugMessageCallbackARB                     gl3wDebugMessageCallbackARB
#define glDebugMessageControl                         gl3wDebugMessageControl
#define glDebugMessageControlARB                      gl3wDebugMessageControlARB
#define glDebugMessageInsert                          gl3wDebugMessageInsert
#define glDebugMessageInsertARB                       gl3wDebugMessageInsertARB
#define glDeleteBuffers                               gl3wDeleteBuffers
#define glDeleteFramebuffers                          gl3wDeleteFramebuffers
#define glDeleteNamedStringARB                        gl3wDeleteNamedStringARB
#define glDeleteProgram                               gl3wDeleteProgram
#define glDeleteProgramPipelines                      gl3wDeleteProgramPipelines
#define glDeleteQueries                               gl3wDeleteQueries
#define glDeleteRenderbuffers                         gl3wDeleteRenderbuffers
#define glDeleteSamplers                              gl3wDeleteSamplers
#define glDeleteShader                                gl3wDeleteShader
#define glDeleteSync                                  gl3wDeleteSync
#define glDeleteTextures                              gl3wDeleteTextures
#define glDeleteTransformFeedbacks                    gl3wDeleteTransformFeedbacks
#define glDeleteVertexArrays                          gl3wDeleteVertexArrays
#define glDepthFunc                                   gl3wDepthFunc
#define glDepthMask                                   gl3wDepthMask
#define glDepthRange                                  gl3wDepthRange
#define glDepthRangeArrayv                            gl3wDepthRangeArrayv
#define glDepthRangeIndexed                           gl3wDepthRangeIndexed
#define glDepthRangef                                 gl3wDepthRangef
#define glDetachShader                                gl3wDetachShader
#define glDisable                                     gl3wDisable
#define glDisableVertexArrayAttrib                    gl3wDisableVertexArrayAttrib
#define glDisableVertexAttribArray                    gl3wDisableVertexAttribArray
#define glDisablei                                    gl3wDisablei
#define glDispatchCompute                             gl3wDispatchCompute
#define glDispatchComputeGroupSizeARB                 gl3wDispatchComputeGroupSizeARB
#define glDispatchComputeIndirect                     gl3wDispatchComputeIndirect
#define glDrawArrays                                  gl3wDrawArrays
#define glDrawArraysIndirect                          gl3wDrawArraysIndirect
#define glDrawArraysInstanced                         gl3wDrawArraysInstanced
#define glDrawArraysInstancedBaseInstance             gl3wDrawArraysInstancedBaseInstance
#define glDrawBuffer                                  gl3wDrawBuffer
#define glDrawBuffers                                 gl3wDrawBuffers
#define glDrawElements                                gl3wDrawElements
#define glDrawElementsBaseVertex                      gl3wDrawElementsBaseVertex
#define glDrawElementsIndirect                        gl3wDrawElementsIndirect
#define glDrawElementsInstanced                       gl3wDrawElementsInstanced
#define glDrawElementsInstancedBaseInstance           gl3wDrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertex             gl3wDrawElementsInstancedBaseVertex
#define glDrawElementsInstancedBaseVertexBaseInstance gl3wDrawElementsInstancedBaseVertexBaseInstance
#define glDrawRangeElements                           gl3wDrawRangeElements
#define glDrawRangeElementsBaseVertex                 gl3wDrawRangeElementsBaseVertex
#define glDrawTransformFeedback                       gl3wDrawTransformFeedback
#define glDrawTransformFeedbackInstanced              gl3wDrawTransformFeedbackInstanced
#define glDrawTransformFeedbackStream                 gl3wDrawTransformFeedbackStream
#define glDrawTransformFeedbackStreamInstanced        gl3wDrawTransformFeedbackStreamInstanced
#define glEnable                                      gl3wEnable
#define glEnableVertexArrayAttrib                     gl3wEnableVertexArrayAttrib
#define glEnableVertexAttribArray                     gl3wEnableVertexAttribArray
#define glEnablei                                     gl3wEnablei
#define glEndConditionalRender                        gl3wEndConditionalRender
#define glEndQuery                                    gl3wEndQuery
#define glEndQueryIndexed                             gl3wEndQueryIndexed
#define glEndTransformFeedback                        gl3wEndTransformFeedback
#define glFenceSync                                   gl3wFenceSync
#define glFinish                                      gl3wFinish
#define glFlush                                       gl3wFlush
#define glFlushMappedBufferRange                      gl3wFlushMappedBufferRange
#define glFlushMappedNamedBufferRange                 gl3wFlushMappedNamedBufferRange
#define glFramebufferParameteri                       gl3wFramebufferParameteri
#define glFramebufferRenderbuffer                     gl3wFramebufferRenderbuffer
#define glFramebufferTexture                          gl3wFramebufferTexture
#define glFramebufferTexture1D                        gl3wFramebufferTexture1D
#define glFramebufferTexture2D                        gl3wFramebufferTexture2D
#define glFramebufferTexture3D                        gl3wFramebufferTexture3D
#define glFramebufferTextureLayer                     gl3wFramebufferTextureLayer
#define glFrontFace                                   gl3wFrontFace
#define glGenBuffers                                  gl3wGenBuffers
#define glGenFramebuffers                             gl3wGenFramebuffers
#define glGenProgramPipelines                         gl3wGenProgramPipelines
#define glGenQueries                                  gl3wGenQueries
#define glGenRenderbuffers                            gl3wGenRenderbuffers
#define glGenSamplers                                 gl3wGenSamplers
#define glGenTextures                                 gl3wGenTextures
#define glGenTransformFeedbacks                       gl3wGenTransformFeedbacks
#define glGenVertexArrays                             gl3wGenVertexArrays
#define glGenerateMipmap                              gl3wGenerateMipmap
#define glGenerateTextureMipmap                       gl3wGenerateTextureMipmap
#define glGetActiveAtomicCounterBufferiv              gl3wGetActiveAtomicCounterBufferiv
#define glGetActiveAttrib                             gl3wGetActiveAttrib
#define glGetActiveSubroutineName                     gl3wGetActiveSubroutineName
#define glGetActiveSubroutineUniformName              gl3wGetActiveSubroutineUniformName
#define glGetActiveSubroutineUniformiv                gl3wGetActiveSubroutineUniformiv
#define glGetActiveUniform                            gl3wGetActiveUniform
#define glGetActiveUniformBlockName                   gl3wGetActiveUniformBlockName
#define glGetActiveUniformBlockiv                     gl3wGetActiveUniformBlockiv
#define glGetActiveUniformName                        gl3wGetActiveUniformName
#define glGetActiveUniformsiv                         gl3wGetActiveUniformsiv
#define glGetAttachedShaders                          gl3wGetAttachedShaders
#define glGetAttribLocation                           gl3wGetAttribLocation
#define glGetBooleani_v                               gl3wGetBooleani_v
#define glGetBooleanv                                 gl3wGetBooleanv
#define glGetBufferParameteri64v                      gl3wGetBufferParameteri64v
#define glGetBufferParameteriv                        gl3wGetBufferParameteriv
#define glGetBufferPointerv                           gl3wGetBufferPointerv
#define glGetBufferSubData                            gl3wGetBufferSubData
#define glGetCompressedTexImage                       gl3wGetCompressedTexImage
#define glGetCompressedTextureImage                   gl3wGetCompressedTextureImage
#define glGetCompressedTextureSubImage                gl3wGetCompressedTextureSubImage
#define glGetDebugMessageLog                          gl3wGetDebugMessageLog
#define glGetDebugMessageLogARB                       gl3wGetDebugMessageLogARB
#define glGetDoublei_v                                gl3wGetDoublei_v
#define glGetDoublev                                  gl3wGetDoublev
#define glGetError                                    gl3wGetError
#define glGetFloati_v                                 gl3wGetFloati_v
#define glGetFloatv                                   gl3wGetFloatv
#define glGetFragDataIndex                            gl3wGetFragDataIndex
#define glGetFragDataLocation                         gl3wGetFragDataLocation
#define glGetFramebufferAttachmentParameteriv         gl3wGetFramebufferAttachmentParameteriv
#define glGetFramebufferParameteriv                   gl3wGetFramebufferParameteriv
#define glGetGraphicsResetStatus                      gl3wGetGraphicsResetStatus
#define glGetGraphicsResetStatusARB                   gl3wGetGraphicsResetStatusARB
#define glGetImageHandleARB                           gl3wGetImageHandleARB
#define glGetInteger64i_v                             gl3wGetInteger64i_v
#define glGetInteger64v                               gl3wGetInteger64v
#define glGetIntegeri_v                               gl3wGetIntegeri_v
#define glGetIntegerv                                 gl3wGetIntegerv
#define glGetInternalformati64v                       gl3wGetInternalformati64v
#define glGetInternalformativ                         gl3wGetInternalformativ
#define glGetMultisamplefv                            gl3wGetMultisamplefv
#define glGetNamedBufferParameteri64v                 gl3wGetNamedBufferParameteri64v
#define glGetNamedBufferParameteriv                   gl3wGetNamedBufferParameteriv
#define glGetNamedBufferPointerv                      gl3wGetNamedBufferPointerv
#define glGetNamedBufferSubData                       gl3wGetNamedBufferSubData
#define glGetNamedFramebufferAttachmentParameteriv    gl3wGetNamedFramebufferAttachmentParameteriv
#define glGetNamedFramebufferParameteriv              gl3wGetNamedFramebufferParameteriv
#define glGetNamedRenderbufferParameteriv             gl3wGetNamedRenderbufferParameteriv
#define glGetNamedStringARB                           gl3wGetNamedStringARB
#define glGetNamedStringivARB                         gl3wGetNamedStringivARB
#define glGetObjectLabel                              gl3wGetObjectLabel
#define glGetObjectPtrLabel                           gl3wGetObjectPtrLabel
#define glGetPointerv                                 gl3wGetPointerv
#define glGetProgramBinary                            gl3wGetProgramBinary
#define glGetProgramInfoLog                           gl3wGetProgramInfoLog
#define glGetProgramInterfaceiv                       gl3wGetProgramInterfaceiv
#define glGetProgramPipelineInfoLog                   gl3wGetProgramPipelineInfoLog
#define glGetProgramPipelineiv                        gl3wGetProgramPipelineiv
#define glGetProgramResourceIndex                     gl3wGetProgramResourceIndex
#define glGetProgramResourceLocation                  gl3wGetProgramResourceLocation
#define glGetProgramResourceLocationIndex             gl3wGetProgramResourceLocationIndex
#define glGetProgramResourceName                      gl3wGetProgramResourceName
#define glGetProgramResourceiv                        gl3wGetProgramResourceiv
#define glGetProgramStageiv                           gl3wGetProgramStageiv
#define glGetProgramiv                                gl3wGetProgramiv
#define glGetQueryBufferObjecti64v                    gl3wGetQueryBufferObjecti64v
#define glGetQueryBufferObjectiv                      gl3wGetQueryBufferObjectiv
#define glGetQueryBufferObjectui64v                   gl3wGetQueryBufferObjectui64v
#define glGetQueryBufferObjectuiv                     gl3wGetQueryBufferObjectuiv
#define glGetQueryIndexediv                           gl3wGetQueryIndexediv
#define glGetQueryObjecti64v                          gl3wGetQueryObjecti64v
#define glGetQueryObjectiv                            gl3wGetQueryObjectiv
#define glGetQueryObjectui64v                         gl3wGetQueryObjectui64v
#define glGetQueryObjectuiv                           gl3wGetQueryObjectuiv
#define glGetQueryiv                                  gl3wGetQueryiv
#define glGetRenderbufferParameteriv                  gl3wGetRenderbufferParameteriv
#define glGetSamplerParameterIiv                      gl3wGetSamplerParameterIiv
#define glGetSamplerParameterIuiv                     gl3wGetSamplerParameterIuiv
#define glGetSamplerParameterfv                       gl3wGetSamplerParameterfv
#define glGetSamplerParameteriv                       gl3wGetSamplerParameteriv
#define glGetShaderInfoLog                            gl3wGetShaderInfoLog
#define glGetShaderPrecisionFormat                    gl3wGetShaderPrecisionFormat
#define glGetShaderSource                             gl3wGetShaderSource
#define glGetShaderiv                                 gl3wGetShaderiv
#define glGetString                                   gl3wGetString
#define glGetStringi                                  gl3wGetStringi
#define glGetSubroutineIndex                          gl3wGetSubroutineIndex
#define glGetSubroutineUniformLocation                gl3wGetSubroutineUniformLocation
#define glGetSynciv                                   gl3wGetSynciv
#define glGetTexImage                                 gl3wGetTexImage
#define glGetTexLevelParameterfv                      gl3wGetTexLevelParameterfv
#define glGetTexLevelParameteriv                      gl3wGetTexLevelParameteriv
#define glGetTexParameterIiv                          gl3wGetTexParameterIiv
#define glGetTexParameterIuiv                         gl3wGetTexParameterIuiv
#define glGetTexParameterfv                           gl3wGetTexParameterfv
#define glGetTexParameteriv                           gl3wGetTexParameteriv
#define glGetTextureHandleARB                         gl3wGetTextureHandleARB
#define glGetTextureImage                             gl3wGetTextureImage
#define glGetTextureLevelParameterfv                  gl3wGetTextureLevelParameterfv
#define glGetTextureLevelParameteriv                  gl3wGetTextureLevelParameteriv
#define glGetTextureParameterIiv                      gl3wGetTextureParameterIiv
#define glGetTextureParameterIuiv                     gl3wGetTextureParameterIuiv
#define glGetTextureParameterfv                       gl3wGetTextureParameterfv
#define glGetTextureParameteriv                       gl3wGetTextureParameteriv
#define glGetTextureSamplerHandleARB                  gl3wGetTextureSamplerHandleARB
#define glGetTextureSubImage                          gl3wGetTextureSubImage
#define glGetTransformFeedbackVarying                 gl3wGetTransformFeedbackVarying
#define glGetTransformFeedbacki64_v                   gl3wGetTransformFeedbacki64_v
#define glGetTransformFeedbacki_v                     gl3wGetTransformFeedbacki_v
#define glGetTransformFeedbackiv                      gl3wGetTransformFeedbackiv
#define glGetUniformBlockIndex                        gl3wGetUniformBlockIndex
#define glGetUniformIndices                           gl3wGetUniformIndices
#define glGetUniformLocation                          gl3wGetUniformLocation
#define glGetUniformSubroutineuiv                     gl3wGetUniformSubroutineuiv
#define glGetUniformdv                                gl3wGetUniformdv
#define glGetUniformfv                                gl3wGetUniformfv
#define glGetUniformiv                                gl3wGetUniformiv
#define glGetUniformuiv                               gl3wGetUniformuiv
#define glGetVertexArrayIndexed64iv                   gl3wGetVertexArrayIndexed64iv
#define glGetVertexArrayIndexediv                     gl3wGetVertexArrayIndexediv
#define glGetVertexArrayiv                            gl3wGetVertexArrayiv
#define glGetVertexAttribIiv                          gl3wGetVertexAttribIiv
#define glGetVertexAttribIuiv                         gl3wGetVertexAttribIuiv
#define glGetVertexAttribLdv                          gl3wGetVertexAttribLdv
#define glGetVertexAttribLui64vARB                    gl3wGetVertexAttribLui64vARB
#define glGetVertexAttribPointerv                     gl3wGetVertexAttribPointerv
#define glGetVertexAttribdv                           gl3wGetVertexAttribdv
#define glGetVertexAttribfv                           gl3wGetVertexAttribfv
#define glGetVertexAttribiv                           gl3wGetVertexAttribiv
#define glGetnCompressedTexImage                      gl3wGetnCompressedTexImage
#define glGetnCompressedTexImageARB                   gl3wGetnCompressedTexImageARB
#define glGetnTexImage                                gl3wGetnTexImage
#define glGetnTexImageARB                             gl3wGetnTexImageARB
#define glGetnUniformdv                               gl3wGetnUniformdv
#define glGetnUniformdvARB                            gl3wGetnUniformdvARB
#define glGetnUniformfv                               gl3wGetnUniformfv
#define glGetnUniformfvARB                            gl3wGetnUniformfvARB
#define glGetnUniformiv                               gl3wGetnUniformiv
#define glGetnUniformivARB                            gl3wGetnUniformivARB
#define glGetnUniformuiv                              gl3wGetnUniformuiv
#define glGetnUniformuivARB                           gl3wGetnUniformuivARB
#define glHint                                        gl3wHint
#define glInvalidateBufferData                        gl3wInvalidateBufferData
#define glInvalidateBufferSubData                     gl3wInvalidateBufferSubData
#define glInvalidateFramebuffer                       gl3wInvalidateFramebuffer
#define glInvalidateNamedFramebufferData              gl3wInvalidateNamedFramebufferData
#define glInvalidateNamedFramebufferSubData           gl3wInvalidateNamedFramebufferSubData
#define glInvalidateSubFramebuffer                    gl3wInvalidateSubFramebuffer
#define glInvalidateTexImage                          gl3wInvalidateTexImage
#define glInvalidateTexSubImage                       gl3wInvalidateTexSubImage
#define glIsBuffer                                    gl3wIsBuffer
#define glIsEnabled                                   gl3wIsEnabled
#define glIsEnabledi                                  gl3wIsEnabledi
#define glIsFramebuffer                               gl3wIsFramebuffer
#define glIsImageHandleResidentARB                    gl3wIsImageHandleResidentARB
#define glIsNamedStringARB                            gl3wIsNamedStringARB
#define glIsProgram                                   gl3wIsProgram
#define glIsProgramPipeline                           gl3wIsProgramPipeline
#define glIsQuery                                     gl3wIsQuery
#define glIsRenderbuffer                              gl3wIsRenderbuffer
#define glIsSampler                                   gl3wIsSampler
#define glIsShader                                    gl3wIsShader
#define glIsSync                                      gl3wIsSync
#define glIsTexture                                   gl3wIsTexture
#define glIsTextureHandleResidentARB                  gl3wIsTextureHandleResidentARB
#define glIsTransformFeedback                         gl3wIsTransformFeedback
#define glIsVertexArray                               gl3wIsVertexArray
#define glLineWidth                                   gl3wLineWidth
#define glLinkProgram                                 gl3wLinkProgram
#define glLogicOp                                     gl3wLogicOp
#define glMakeImageHandleNonResidentARB               gl3wMakeImageHandleNonResidentARB
#define glMakeImageHandleResidentARB                  gl3wMakeImageHandleResidentARB
#define glMakeTextureHandleNonResidentARB             gl3wMakeTextureHandleNonResidentARB
#define glMakeTextureHandleResidentARB                gl3wMakeTextureHandleResidentARB
#define glMapBuffer                                   gl3wMapBuffer
#define glMapBufferRange                              gl3wMapBufferRange
#define glMapNamedBuffer                              gl3wMapNamedBuffer
#define glMapNamedBufferRange                         gl3wMapNamedBufferRange
#define glMemoryBarrier                               gl3wMemoryBarrier
#define glMemoryBarrierByRegion                       gl3wMemoryBarrierByRegion
#define glMinSampleShading                            gl3wMinSampleShading
#define glMinSampleShadingARB                         gl3wMinSampleShadingARB
#define glMultiDrawArrays                             gl3wMultiDrawArrays
#define glMultiDrawArraysIndirect                     gl3wMultiDrawArraysIndirect
#define glMultiDrawArraysIndirectCountARB             gl3wMultiDrawArraysIndirectCountARB
#define glMultiDrawElements                           gl3wMultiDrawElements
#define glMultiDrawElementsBaseVertex                 gl3wMultiDrawElementsBaseVertex
#define glMultiDrawElementsIndirect                   gl3wMultiDrawElementsIndirect
#define glMultiDrawElementsIndirectCountARB           gl3wMultiDrawElementsIndirectCountARB
#define glNamedBufferData                             gl3wNamedBufferData
#define glNamedBufferPageCommitmentARB                gl3wNamedBufferPageCommitmentARB
#define glNamedBufferPageCommitmentEXT                gl3wNamedBufferPageCommitmentEXT
#define glNamedBufferStorage                          gl3wNamedBufferStorage
#define glNamedBufferSubData                          gl3wNamedBufferSubData
#define glNamedFramebufferDrawBuffer                  gl3wNamedFramebufferDrawBuffer
#define glNamedFramebufferDrawBuffers                 gl3wNamedFramebufferDrawBuffers
#define glNamedFramebufferParameteri                  gl3wNamedFramebufferParameteri
#define glNamedFramebufferReadBuffer                  gl3wNamedFramebufferReadBuffer
#define glNamedFramebufferRenderbuffer                gl3wNamedFramebufferRenderbuffer
#define glNamedFramebufferTexture                     gl3wNamedFramebufferTexture
#define glNamedFramebufferTextureLayer                gl3wNamedFramebufferTextureLayer
#define glNamedRenderbufferStorage                    gl3wNamedRenderbufferStorage
#define glNamedRenderbufferStorageMultisample         gl3wNamedRenderbufferStorageMultisample
#define glNamedStringARB                              gl3wNamedStringARB
#define glObjectLabel                                 gl3wObjectLabel
#define glObjectPtrLabel                              gl3wObjectPtrLabel
#define glPatchParameterfv                            gl3wPatchParameterfv
#define glPatchParameteri                             gl3wPatchParameteri
#define glPauseTransformFeedback                      gl3wPauseTransformFeedback
#define glPixelStoref                                 gl3wPixelStoref
#define glPixelStorei                                 gl3wPixelStorei
#define glPointParameterf                             gl3wPointParameterf
#define glPointParameterfv                            gl3wPointParameterfv
#define glPointParameteri                             gl3wPointParameteri
#define glPointParameteriv                            gl3wPointParameteriv
#define glPointSize                                   gl3wPointSize
#define glPolygonMode                                 gl3wPolygonMode
#define glPolygonOffset                               gl3wPolygonOffset
#define glPopDebugGroup                               gl3wPopDebugGroup
#define glPrimitiveRestartIndex                       gl3wPrimitiveRestartIndex
#define glProgramBinary                               gl3wProgramBinary
#define glProgramParameteri                           gl3wProgramParameteri
#define glProgramUniform1d                            gl3wProgramUniform1d
#define glProgramUniform1dv                           gl3wProgramUniform1dv
#define glProgramUniform1f                            gl3wProgramUniform1f
#define glProgramUniform1fv                           gl3wProgramUniform1fv
#define glProgramUniform1i                            gl3wProgramUniform1i
#define glProgramUniform1iv                           gl3wProgramUniform1iv
#define glProgramUniform1ui                           gl3wProgramUniform1ui
#define glProgramUniform1uiv                          gl3wProgramUniform1uiv
#define glProgramUniform2d                            gl3wProgramUniform2d
#define glProgramUniform2dv                           gl3wProgramUniform2dv
#define glProgramUniform2f                            gl3wProgramUniform2f
#define glProgramUniform2fv                           gl3wProgramUniform2fv
#define glProgramUniform2i                            gl3wProgramUniform2i
#define glProgramUniform2iv                           gl3wProgramUniform2iv
#define glProgramUniform2ui                           gl3wProgramUniform2ui
#define glProgramUniform2uiv                          gl3wProgramUniform2uiv
#define glProgramUniform3d                            gl3wProgramUniform3d
#define glProgramUniform3dv                           gl3wProgramUniform3dv
#define glProgramUniform3f                            gl3wProgramUniform3f
#define glProgramUniform3fv                           gl3wProgramUniform3fv
#define glProgramUniform3i                            gl3wProgramUniform3i
#define glProgramUniform3iv                           gl3wProgramUniform3iv
#define glProgramUniform3ui                           gl3wProgramUniform3ui
#define glProgramUniform3uiv                          gl3wProgramUniform3uiv
#define glProgramUniform4d                            gl3wProgramUniform4d
#define glProgramUniform4dv                           gl3wProgramUniform4dv
#define glProgramUniform4f                            gl3wProgramUniform4f
#define glProgramUniform4fv                           gl3wProgramUniform4fv
#define glProgramUniform4i                            gl3wProgramUniform4i
#define glProgramUniform4iv                           gl3wProgramUniform4iv
#define glProgramUniform4ui                           gl3wProgramUniform4ui
#define glProgramUniform4uiv                          gl3wProgramUniform4uiv
#define glProgramUniformHandleui64ARB                 gl3wProgramUniformHandleui64ARB
#define glProgramUniformHandleui64vARB                gl3wProgramUniformHandleui64vARB
#define glProgramUniformMatrix2dv                     gl3wProgramUniformMatrix2dv
#define glProgramUniformMatrix2fv                     gl3wProgramUniformMatrix2fv
#define glProgramUniformMatrix2x3dv                   gl3wProgramUniformMatrix2x3dv
#define glProgramUniformMatrix2x3fv                   gl3wProgramUniformMatrix2x3fv
#define glProgramUniformMatrix2x4dv                   gl3wProgramUniformMatrix2x4dv
#define glProgramUniformMatrix2x4fv                   gl3wProgramUniformMatrix2x4fv
#define glProgramUniformMatrix3dv                     gl3wProgramUniformMatrix3dv
#define glProgramUniformMatrix3fv                     gl3wProgramUniformMatrix3fv
#define glProgramUniformMatrix3x2dv                   gl3wProgramUniformMatrix3x2dv
#define glProgramUniformMatrix3x2fv                   gl3wProgramUniformMatrix3x2fv
#define glProgramUniformMatrix3x4dv                   gl3wProgramUniformMatrix3x4dv
#define glProgramUniformMatrix3x4fv                   gl3wProgramUniformMatrix3x4fv
#define glProgramUniformMatrix4dv                     gl3wProgramUniformMatrix4dv
#define glProgramUniformMatrix4fv                     gl3wProgramUniformMatrix4fv
#define glProgramUniformMatrix4x2dv                   gl3wProgramUniformMatrix4x2dv
#define glProgramUniformMatrix4x2fv                   gl3wProgramUniformMatrix4x2fv
#define glProgramUniformMatrix4x3dv                   gl3wProgramUniformMatrix4x3dv
#define glProgramUniformMatrix4x3fv                   gl3wProgramUniformMatrix4x3fv
#define glProvokingVertex                             gl3wProvokingVertex
#define glPushDebugGroup                              gl3wPushDebugGroup
#define glQueryCounter                                gl3wQueryCounter
#define glReadBuffer                                  gl3wReadBuffer
#define glReadPixels                                  gl3wReadPixels
#define glReadnPixels                                 gl3wReadnPixels
#define glReadnPixelsARB                              gl3wReadnPixelsARB
#define glReleaseShaderCompiler                       gl3wReleaseShaderCompiler
#define glRenderbufferStorage                         gl3wRenderbufferStorage
#define glRenderbufferStorageMultisample              gl3wRenderbufferStorageMultisample
#define glResumeTransformFeedback                     gl3wResumeTransformFeedback
#define glSampleCoverage                              gl3wSampleCoverage
#define glSampleMaski                                 gl3wSampleMaski
#define glSamplerParameterIiv                         gl3wSamplerParameterIiv
#define glSamplerParameterIuiv                        gl3wSamplerParameterIuiv
#define glSamplerParameterf                           gl3wSamplerParameterf
#define glSamplerParameterfv                          gl3wSamplerParameterfv
#define glSamplerParameteri                           gl3wSamplerParameteri
#define glSamplerParameteriv                          gl3wSamplerParameteriv
#define glScissor                                     gl3wScissor
#define glScissorArrayv                               gl3wScissorArrayv
#define glScissorIndexed                              gl3wScissorIndexed
#define glScissorIndexedv                             gl3wScissorIndexedv
#define glShaderBinary                                gl3wShaderBinary
#define glShaderSource                                gl3wShaderSource
#define glShaderStorageBlockBinding                   gl3wShaderStorageBlockBinding
#define glStencilFunc                                 gl3wStencilFunc
#define glStencilFuncSeparate                         gl3wStencilFuncSeparate
#define glStencilMask                                 gl3wStencilMask
#define glStencilMaskSeparate                         gl3wStencilMaskSeparate
#define glStencilOp                                   gl3wStencilOp
#define glStencilOpSeparate                           gl3wStencilOpSeparate
#define glTexBuffer                                   gl3wTexBuffer
#define glTexBufferRange                              gl3wTexBufferRange
#define glTexImage1D                                  gl3wTexImage1D
#define glTexImage2D                                  gl3wTexImage2D
#define glTexImage2DMultisample                       gl3wTexImage2DMultisample
#define glTexImage3D                                  gl3wTexImage3D
#define glTexImage3DMultisample                       gl3wTexImage3DMultisample
#define glTexPageCommitmentARB                        gl3wTexPageCommitmentARB
#define glTexParameterIiv                             gl3wTexParameterIiv
#define glTexParameterIuiv                            gl3wTexParameterIuiv
#define glTexParameterf                               gl3wTexParameterf
#define glTexParameterfv                              gl3wTexParameterfv
#define glTexParameteri                               gl3wTexParameteri
#define glTexParameteriv                              gl3wTexParameteriv
#define glTexStorage1D                                gl3wTexStorage1D
#define glTexStorage2D                                gl3wTexStorage2D
#define glTexStorage2DMultisample                     gl3wTexStorage2DMultisample
#define glTexStorage3D                                gl3wTexStorage3D
#define glTexStorage3DMultisample                     gl3wTexStorage3DMultisample
#define glTexSubImage1D                               gl3wTexSubImage1D
#define glTexSubImage2D                               gl3wTexSubImage2D
#define glTexSubImage3D                               gl3wTexSubImage3D
#define glTextureBarrier                              gl3wTextureBarrier
#define glTextureBuffer                               gl3wTextureBuffer
#define glTextureBufferRange                          gl3wTextureBufferRange
#define glTextureParameterIiv                         gl3wTextureParameterIiv
#define glTextureParameterIuiv                        gl3wTextureParameterIuiv
#define glTextureParameterf                           gl3wTextureParameterf
#define glTextureParameterfv                          gl3wTextureParameterfv
#define glTextureParameteri                           gl3wTextureParameteri
#define glTextureParameteriv                          gl3wTextureParameteriv
#define glTextureStorage1D                            gl3wTextureStorage1D
#define glTextureStorage2D                            gl3wTextureStorage2D
#define glTextureStorage2DMultisample                 gl3wTextureStorage2DMultisample
#define glTextureStorage3D                            gl3wTextureStorage3D
#define glTextureStorage3DMultisample                 gl3wTextureStorage3DMultisample
#define glTextureSubImage1D                           gl3wTextureSubImage1D
#define glTextureSubImage2D                           gl3wTextureSubImage2D
#define glTextureSubImage3D                           gl3wTextureSubImage3D
#define glTextureView                                 gl3wTextureView
#define glTransformFeedbackBufferBase                 gl3wTransformFeedbackBufferBase
#define glTransformFeedbackBufferRange                gl3wTransformFeedbackBufferRange
#define glTransformFeedbackVaryings                   gl3wTransformFeedbackVaryings
#define glUniform1d                                   gl3wUniform1d
#define glUniform1dv                                  gl3wUniform1dv
#define glUniform1f                                   gl3wUniform1f
#define glUniform1fv                                  gl3wUniform1fv
#define glUniform1i                                   gl3wUniform1i
#define glUniform1iv                                  gl3wUniform1iv
#define glUniform1ui                                  gl3wUniform1ui
#define glUniform1uiv                                 gl3wUniform1uiv
#define glUniform2d                                   gl3wUniform2d
#define glUniform2dv                                  gl3wUniform2dv
#define glUniform2f                                   gl3wUniform2f
#define glUniform2fv                                  gl3wUniform2fv
#define glUniform2i                                   gl3wUniform2i
#define glUniform2iv                                  gl3wUniform2iv
#define glUniform2ui                                  gl3wUniform2ui
#define glUniform2uiv                                 gl3wUniform2uiv
#define glUniform3d                                   gl3wUniform3d
#define glUniform3dv                                  gl3wUniform3dv
#define glUniform3f                                   gl3wUniform3f
#define glUniform3fv                                  gl3wUniform3fv
#define glUniform3i                                   gl3wUniform3i
#define glUniform3iv                                  gl3wUniform3iv
#define glUniform3ui                                  gl3wUniform3ui
#define glUniform3uiv                                 gl3wUniform3uiv
#define glUniform4d                                   gl3wUniform4d
#define glUniform4dv                                  gl3wUniform4dv
#define glUniform4f                                   gl3wUniform4f
#define glUniform4fv                                  gl3wUniform4fv
#define glUniform4i                                   gl3wUniform4i
#define glUniform4iv                                  gl3wUniform4iv
#define glUniform4ui                                  gl3wUniform4ui
#define glUniform4uiv                                 gl3wUniform4uiv
#define glUniformBlockBinding                         gl3wUniformBlockBinding
#define glUniformHandleui64ARB                        gl3wUniformHandleui64ARB
#define glUniformHandleui64vARB                       gl3wUniformHandleui64vARB
#define glUniformMatrix2dv                            gl3wUniformMatrix2dv
#define glUniformMatrix2fv                            gl3wUniformMatrix2fv
#define glUniformMatrix2x3dv                          gl3wUniformMatrix2x3dv
#define glUniformMatrix2x3fv                          gl3wUniformMatrix2x3fv
#define glUniformMatrix2x4dv                          gl3wUniformMatrix2x4dv
#define glUniformMatrix2x4fv                          gl3wUniformMatrix2x4fv
#define glUniformMatrix3dv                            gl3wUniformMatrix3dv
#define glUniformMatrix3fv                            gl3wUniformMatrix3fv
#define glUniformMatrix3x2dv                          gl3wUniformMatrix3x2dv
#define glUniformMatrix3x2fv                          gl3wUniformMatrix3x2fv
#define glUniformMatrix3x4dv                          gl3wUniformMatrix3x4dv
#define glUniformMatrix3x4fv                          gl3wUniformMatrix3x4fv
#define glUniformMatrix4dv                            gl3wUniformMatrix4dv
#define glUniformMatrix4fv                            gl3wUniformMatrix4fv
#define glUniformMatrix4x2dv                          gl3wUniformMatrix4x2dv
#define glUniformMatrix4x2fv                          gl3wUniformMatrix4x2fv
#define glUniformMatrix4x3dv                          gl3wUniformMatrix4x3dv
#define glUniformMatrix4x3fv                          gl3wUniformMatrix4x3fv
#define glUniformSubroutinesuiv                       gl3wUniformSubroutinesuiv
#define glUnmapBuffer                                 gl3wUnmapBuffer
#define glUnmapNamedBuffer                            gl3wUnmapNamedBuffer
#define glUseProgram                                  gl3wUseProgram
#define glUseProgramStages                            gl3wUseProgramStages
#define glValidateProgram                             gl3wValidateProgram
#define glValidateProgramPipeline                     gl3wValidateProgramPipeline
#define glVertexArrayAttribBinding                    gl3wVertexArrayAttribBinding
#define glVertexArrayAttribFormat                     gl3wVertexArrayAttribFormat
#define glVertexArrayAttribIFormat                    gl3wVertexArrayAttribIFormat
#define glVertexArrayAttribLFormat                    gl3wVertexArrayAttribLFormat
#define glVertexArrayBindingDivisor                   gl3wVertexArrayBindingDivisor
#define glVertexArrayElementBuffer                    gl3wVertexArrayElementBuffer
#define glVertexArrayVertexBuffer                     gl3wVertexArrayVertexBuffer
#define glVertexArrayVertexBuffers                    gl3wVertexArrayVertexBuffers
#define glVertexAttrib1d                              gl3wVertexAttrib1d
#define glVertexAttrib1dv                             gl3wVertexAttrib1dv
#define glVertexAttrib1f                              gl3wVertexAttrib1f
#define glVertexAttrib1fv                             gl3wVertexAttrib1fv
#define glVertexAttrib1s                              gl3wVertexAttrib1s
#define glVertexAttrib1sv                             gl3wVertexAttrib1sv
#define glVertexAttrib2d                              gl3wVertexAttrib2d
#define glVertexAttrib2dv                             gl3wVertexAttrib2dv
#define glVertexAttrib2f                              gl3wVertexAttrib2f
#define glVertexAttrib2fv                             gl3wVertexAttrib2fv
#define glVertexAttrib2s                              gl3wVertexAttrib2s
#define glVertexAttrib2sv                             gl3wVertexAttrib2sv
#define glVertexAttrib3d                              gl3wVertexAttrib3d
#define glVertexAttrib3dv                             gl3wVertexAttrib3dv
#define glVertexAttrib3f                              gl3wVertexAttrib3f
#define glVertexAttrib3fv                             gl3wVertexAttrib3fv
#define glVertexAttrib3s                              gl3wVertexAttrib3s
#define glVertexAttrib3sv                             gl3wVertexAttrib3sv
#define glVertexAttrib4Nbv                            gl3wVertexAttrib4Nbv
#define glVertexAttrib4Niv                            gl3wVertexAttrib4Niv
#define glVertexAttrib4Nsv                            gl3wVertexAttrib4Nsv
#define glVertexAttrib4Nub                            gl3wVertexAttrib4Nub
#define glVertexAttrib4Nubv                           gl3wVertexAttrib4Nubv
#define glVertexAttrib4Nuiv                           gl3wVertexAttrib4Nuiv
#define glVertexAttrib4Nusv                           gl3wVertexAttrib4Nusv
#define glVertexAttrib4bv                             gl3wVertexAttrib4bv
#define glVertexAttrib4d                              gl3wVertexAttrib4d
#define glVertexAttrib4dv                             gl3wVertexAttrib4dv
#define glVertexAttrib4f                              gl3wVertexAttrib4f
#define glVertexAttrib4fv                             gl3wVertexAttrib4fv
#define glVertexAttrib4iv                             gl3wVertexAttrib4iv
#define glVertexAttrib4s                              gl3wVertexAttrib4s
#define glVertexAttrib4sv                             gl3wVertexAttrib4sv
#define glVertexAttrib4ubv                            gl3wVertexAttrib4ubv
#define glVertexAttrib4uiv                            gl3wVertexAttrib4uiv
#define glVertexAttrib4usv                            gl3wVertexAttrib4usv
#define glVertexAttribBinding                         gl3wVertexAttribBinding
#define glVertexAttribDivisor                         gl3wVertexAttribDivisor
#define glVertexAttribFormat                          gl3wVertexAttribFormat
#define glVertexAttribI1i                             gl3wVertexAttribI1i
#define glVertexAttribI1iv                            gl3wVertexAttribI1iv
#define glVertexAttribI1ui                            gl3wVertexAttribI1ui
#define glVertexAttribI1uiv                           gl3wVertexAttribI1uiv
#define glVertexAttribI2i                             gl3wVertexAttribI2i
#define glVertexAttribI2iv                            gl3wVertexAttribI2iv
#define glVertexAttribI2ui                            gl3wVertexAttribI2ui
#define glVertexAttribI2uiv                           gl3wVertexAttribI2uiv
#define glVertexAttribI3i                             gl3wVertexAttribI3i
#define glVertexAttribI3iv                            gl3wVertexAttribI3iv
#define glVertexAttribI3ui                            gl3wVertexAttribI3ui
#define glVertexAttribI3uiv                           gl3wVertexAttribI3uiv
#define glVertexAttribI4bv                            gl3wVertexAttribI4bv
#define glVertexAttribI4i                             gl3wVertexAttribI4i
#define glVertexAttribI4iv                            gl3wVertexAttribI4iv
#define glVertexAttribI4sv                            gl3wVertexAttribI4sv
#define glVertexAttribI4ubv                           gl3wVertexAttribI4ubv
#define glVertexAttribI4ui                            gl3wVertexAttribI4ui
#define glVertexAttribI4uiv                           gl3wVertexAttribI4uiv
#define glVertexAttribI4usv                           gl3wVertexAttribI4usv
#define glVertexAttribIFormat                         gl3wVertexAttribIFormat
#define glVertexAttribIPointer                        gl3wVertexAttribIPointer
#define glVertexAttribL1d                             gl3wVertexAttribL1d
#define glVertexAttribL1dv                            gl3wVertexAttribL1dv
#define glVertexAttribL1ui64ARB                       gl3wVertexAttribL1ui64ARB
#define glVertexAttribL1ui64vARB                      gl3wVertexAttribL1ui64vARB
#define glVertexAttribL2d                             gl3wVertexAttribL2d
#define glVertexAttribL2dv                            gl3wVertexAttribL2dv
#define glVertexAttribL3d                             gl3wVertexAttribL3d
#define glVertexAttribL3dv                            gl3wVertexAttribL3dv
#define glVertexAttribL4d                             gl3wVertexAttribL4d
#define glVertexAttribL4dv                            gl3wVertexAttribL4dv
#define glVertexAttribLFormat                         gl3wVertexAttribLFormat
#define glVertexAttribLPointer                        gl3wVertexAttribLPointer
#define glVertexAttribP1ui                            gl3wVertexAttribP1ui
#define glVertexAttribP1uiv                           gl3wVertexAttribP1uiv
#define glVertexAttribP2ui                            gl3wVertexAttribP2ui
#define glVertexAttribP2uiv                           gl3wVertexAttribP2uiv
#define glVertexAttribP3ui                            gl3wVertexAttribP3ui
#define glVertexAttribP3uiv                           gl3wVertexAttribP3uiv
#define glVertexAttribP4ui                            gl3wVertexAttribP4ui
#define glVertexAttribP4uiv                           gl3wVertexAttribP4uiv
#define glVertexAttribPointer                         gl3wVertexAttribPointer
#define glVertexBindingDivisor                        gl3wVertexBindingDivisor
#define glViewport                                    gl3wViewport
#define glViewportArrayv                              gl3wViewportArrayv
#define glViewportIndexedf                            gl3wViewportIndexedf
#define glViewportIndexedfv                           gl3wViewportIndexedfv
#define glWaitSync                                    gl3wWaitSync

#ifdef __cplusplus
}
#endif

#endif
