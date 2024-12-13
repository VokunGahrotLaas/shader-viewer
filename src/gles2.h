#pragma once

// SDL3
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_opengles2.h>

#define GL_DYNLOAD_ATTR(F) F##_t* F
#define GL_DYNLOAD_LOAD(F) ctx->F = (F##_t*)SDL_GL_GetProcAddress(#F)

#define GL_DYNLOAD_ATTRS()                                                                                             \
	GL_DYNLOAD_ATTR(glActiveTexture);                                                                                  \
	GL_DYNLOAD_ATTR(glAttachShader);                                                                                   \
	GL_DYNLOAD_ATTR(glBindAttribLocation);                                                                             \
	GL_DYNLOAD_ATTR(glBindBuffer);                                                                                     \
	GL_DYNLOAD_ATTR(glBindFramebuffer);                                                                                \
	GL_DYNLOAD_ATTR(glBindRenderbuffer);                                                                               \
	GL_DYNLOAD_ATTR(glBindTexture);                                                                                    \
	GL_DYNLOAD_ATTR(glBlendColor);                                                                                     \
	GL_DYNLOAD_ATTR(glBlendEquation);                                                                                  \
	GL_DYNLOAD_ATTR(glBlendEquationSeparate);                                                                          \
	GL_DYNLOAD_ATTR(glBlendFunc);                                                                                      \
	GL_DYNLOAD_ATTR(glBlendFuncSeparate);                                                                              \
	GL_DYNLOAD_ATTR(glBufferData);                                                                                     \
	GL_DYNLOAD_ATTR(glBufferSubData);                                                                                  \
	GL_DYNLOAD_ATTR(glCheckFramebufferStatus);                                                                         \
	GL_DYNLOAD_ATTR(glClear);                                                                                          \
	GL_DYNLOAD_ATTR(glClearColor);                                                                                     \
	GL_DYNLOAD_ATTR(glClearDepthf);                                                                                    \
	GL_DYNLOAD_ATTR(glClearStencil);                                                                                   \
	GL_DYNLOAD_ATTR(glColorMask);                                                                                      \
	GL_DYNLOAD_ATTR(glCompileShader);                                                                                  \
	GL_DYNLOAD_ATTR(glCompressedTexImage2D);                                                                           \
	GL_DYNLOAD_ATTR(glCompressedTexSubImage2D);                                                                        \
	GL_DYNLOAD_ATTR(glCopyTexImage2D);                                                                                 \
	GL_DYNLOAD_ATTR(glCopyTexSubImage2D);                                                                              \
	GL_DYNLOAD_ATTR(glCreateProgram);                                                                                  \
	GL_DYNLOAD_ATTR(glCreateShader);                                                                                   \
	GL_DYNLOAD_ATTR(glCullFace);                                                                                       \
	GL_DYNLOAD_ATTR(glDeleteBuffers);                                                                                  \
	GL_DYNLOAD_ATTR(glDeleteFramebuffers);                                                                             \
	GL_DYNLOAD_ATTR(glDeleteProgram);                                                                                  \
	GL_DYNLOAD_ATTR(glDeleteRenderbuffers);                                                                            \
	GL_DYNLOAD_ATTR(glDeleteShader);                                                                                   \
	GL_DYNLOAD_ATTR(glDeleteTextures);                                                                                 \
	GL_DYNLOAD_ATTR(glDepthFunc);                                                                                      \
	GL_DYNLOAD_ATTR(glDepthMask);                                                                                      \
	GL_DYNLOAD_ATTR(glDepthRangef);                                                                                    \
	GL_DYNLOAD_ATTR(glDetachShader);                                                                                   \
	GL_DYNLOAD_ATTR(glDisable);                                                                                        \
	GL_DYNLOAD_ATTR(glDisableVertexAttribArray);                                                                       \
	GL_DYNLOAD_ATTR(glDrawArrays);                                                                                     \
	GL_DYNLOAD_ATTR(glDrawElements);                                                                                   \
	GL_DYNLOAD_ATTR(glEnable);                                                                                         \
	GL_DYNLOAD_ATTR(glEnableVertexAttribArray);                                                                        \
	GL_DYNLOAD_ATTR(glFinish);                                                                                         \
	GL_DYNLOAD_ATTR(glFlush);                                                                                          \
	GL_DYNLOAD_ATTR(glFramebufferRenderbuffer);                                                                        \
	GL_DYNLOAD_ATTR(glFramebufferTexture2D);                                                                           \
	GL_DYNLOAD_ATTR(glFrontFace);                                                                                      \
	GL_DYNLOAD_ATTR(glGenBuffers);                                                                                     \
	GL_DYNLOAD_ATTR(glGenerateMipmap);                                                                                 \
	GL_DYNLOAD_ATTR(glGenFramebuffers);                                                                                \
	GL_DYNLOAD_ATTR(glGenRenderbuffers);                                                                               \
	GL_DYNLOAD_ATTR(glGenTextures);                                                                                    \
	GL_DYNLOAD_ATTR(glGetActiveAttrib);                                                                                \
	GL_DYNLOAD_ATTR(glGetActiveUniform);                                                                               \
	GL_DYNLOAD_ATTR(glGetAttachedShaders);                                                                             \
	GL_DYNLOAD_ATTR(glGetAttribLocation);                                                                              \
	GL_DYNLOAD_ATTR(glGetBooleanv);                                                                                    \
	GL_DYNLOAD_ATTR(glGetBufferParameteriv);                                                                           \
	GL_DYNLOAD_ATTR(glGetError);                                                                                       \
	GL_DYNLOAD_ATTR(glGetFloatv);                                                                                      \
	GL_DYNLOAD_ATTR(glGetFramebufferAttachmentParameteriv);                                                            \
	GL_DYNLOAD_ATTR(glGetIntegerv);                                                                                    \
	GL_DYNLOAD_ATTR(glGetProgramiv);                                                                                   \
	GL_DYNLOAD_ATTR(glGetProgramInfoLog);                                                                              \
	GL_DYNLOAD_ATTR(glGetRenderbufferParameteriv);                                                                     \
	GL_DYNLOAD_ATTR(glGetShaderiv);                                                                                    \
	GL_DYNLOAD_ATTR(glGetShaderInfoLog);                                                                               \
	GL_DYNLOAD_ATTR(glGetShaderPrecisionFormat);                                                                       \
	GL_DYNLOAD_ATTR(glGetShaderSource);                                                                                \
	GL_DYNLOAD_ATTR(glGetString);                                                                                      \
	GL_DYNLOAD_ATTR(glGetTexParameterfv);                                                                              \
	GL_DYNLOAD_ATTR(glGetTexParameteriv);                                                                              \
	GL_DYNLOAD_ATTR(glGetUniformfv);                                                                                   \
	GL_DYNLOAD_ATTR(glGetUniformiv);                                                                                   \
	GL_DYNLOAD_ATTR(glGetUniformLocation);                                                                             \
	GL_DYNLOAD_ATTR(glGetVertexAttribfv);                                                                              \
	GL_DYNLOAD_ATTR(glGetVertexAttribiv);                                                                              \
	GL_DYNLOAD_ATTR(glGetVertexAttribPointerv);                                                                        \
	GL_DYNLOAD_ATTR(glHint);                                                                                           \
	GL_DYNLOAD_ATTR(glIsBuffer);                                                                                       \
	GL_DYNLOAD_ATTR(glIsEnabled);                                                                                      \
	GL_DYNLOAD_ATTR(glIsFramebuffer);                                                                                  \
	GL_DYNLOAD_ATTR(glIsProgram);                                                                                      \
	GL_DYNLOAD_ATTR(glIsRenderbuffer);                                                                                 \
	GL_DYNLOAD_ATTR(glIsShader);                                                                                       \
	GL_DYNLOAD_ATTR(glIsTexture);                                                                                      \
	GL_DYNLOAD_ATTR(glLineWidth);                                                                                      \
	GL_DYNLOAD_ATTR(glLinkProgram);                                                                                    \
	GL_DYNLOAD_ATTR(glPixelStorei);                                                                                    \
	GL_DYNLOAD_ATTR(glPolygonOffset);                                                                                  \
	GL_DYNLOAD_ATTR(glReadPixels);                                                                                     \
	GL_DYNLOAD_ATTR(glReleaseShaderCompiler);                                                                          \
	GL_DYNLOAD_ATTR(glRenderbufferStorage);                                                                            \
	GL_DYNLOAD_ATTR(glSampleCoverage);                                                                                 \
	GL_DYNLOAD_ATTR(glScissor);                                                                                        \
	GL_DYNLOAD_ATTR(glShaderBinary);                                                                                   \
	GL_DYNLOAD_ATTR(glShaderSource);                                                                                   \
	GL_DYNLOAD_ATTR(glStencilFunc);                                                                                    \
	GL_DYNLOAD_ATTR(glStencilFuncSeparate);                                                                            \
	GL_DYNLOAD_ATTR(glStencilMask);                                                                                    \
	GL_DYNLOAD_ATTR(glStencilMaskSeparate);                                                                            \
	GL_DYNLOAD_ATTR(glStencilOp);                                                                                      \
	GL_DYNLOAD_ATTR(glStencilOpSeparate);                                                                              \
	GL_DYNLOAD_ATTR(glTexImage2D);                                                                                     \
	GL_DYNLOAD_ATTR(glTexParameterf);                                                                                  \
	GL_DYNLOAD_ATTR(glTexParameterfv);                                                                                 \
	GL_DYNLOAD_ATTR(glTexParameteri);                                                                                  \
	GL_DYNLOAD_ATTR(glTexParameteriv);                                                                                 \
	GL_DYNLOAD_ATTR(glTexSubImage2D);                                                                                  \
	GL_DYNLOAD_ATTR(glUniform1f);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform1fv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform1i);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform1iv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform2f);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform2fv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform2i);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform2iv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform3f);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform3fv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform3i);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform3iv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform4f);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform4fv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniform4i);                                                                                      \
	GL_DYNLOAD_ATTR(glUniform4iv);                                                                                     \
	GL_DYNLOAD_ATTR(glUniformMatrix2fv);                                                                               \
	GL_DYNLOAD_ATTR(glUniformMatrix3fv);                                                                               \
	GL_DYNLOAD_ATTR(glUniformMatrix4fv);                                                                               \
	GL_DYNLOAD_ATTR(glUseProgram);                                                                                     \
	GL_DYNLOAD_ATTR(glValidateProgram);                                                                                \
	GL_DYNLOAD_ATTR(glVertexAttrib1f);                                                                                 \
	GL_DYNLOAD_ATTR(glVertexAttrib1fv);                                                                                \
	GL_DYNLOAD_ATTR(glVertexAttrib2f);                                                                                 \
	GL_DYNLOAD_ATTR(glVertexAttrib2fv);                                                                                \
	GL_DYNLOAD_ATTR(glVertexAttrib3f);                                                                                 \
	GL_DYNLOAD_ATTR(glVertexAttrib3fv);                                                                                \
	GL_DYNLOAD_ATTR(glVertexAttrib4f);                                                                                 \
	GL_DYNLOAD_ATTR(glVertexAttrib4fv);                                                                                \
	GL_DYNLOAD_ATTR(glVertexAttribPointer);                                                                            \
	GL_DYNLOAD_ATTR(glViewport)

#define GL_DYNLOAD_LOADS()                                                                                             \
	GL_DYNLOAD_LOAD(glActiveTexture);                                                                                  \
	GL_DYNLOAD_LOAD(glAttachShader);                                                                                   \
	GL_DYNLOAD_LOAD(glBindAttribLocation);                                                                             \
	GL_DYNLOAD_LOAD(glBindBuffer);                                                                                     \
	GL_DYNLOAD_LOAD(glBindFramebuffer);                                                                                \
	GL_DYNLOAD_LOAD(glBindRenderbuffer);                                                                               \
	GL_DYNLOAD_LOAD(glBindTexture);                                                                                    \
	GL_DYNLOAD_LOAD(glBlendColor);                                                                                     \
	GL_DYNLOAD_LOAD(glBlendEquation);                                                                                  \
	GL_DYNLOAD_LOAD(glBlendEquationSeparate);                                                                          \
	GL_DYNLOAD_LOAD(glBlendFunc);                                                                                      \
	GL_DYNLOAD_LOAD(glBlendFuncSeparate);                                                                              \
	GL_DYNLOAD_LOAD(glBufferData);                                                                                     \
	GL_DYNLOAD_LOAD(glBufferSubData);                                                                                  \
	GL_DYNLOAD_LOAD(glCheckFramebufferStatus);                                                                         \
	GL_DYNLOAD_LOAD(glClear);                                                                                          \
	GL_DYNLOAD_LOAD(glClearColor);                                                                                     \
	GL_DYNLOAD_LOAD(glClearDepthf);                                                                                    \
	GL_DYNLOAD_LOAD(glClearStencil);                                                                                   \
	GL_DYNLOAD_LOAD(glColorMask);                                                                                      \
	GL_DYNLOAD_LOAD(glCompileShader);                                                                                  \
	GL_DYNLOAD_LOAD(glCompressedTexImage2D);                                                                           \
	GL_DYNLOAD_LOAD(glCompressedTexSubImage2D);                                                                        \
	GL_DYNLOAD_LOAD(glCopyTexImage2D);                                                                                 \
	GL_DYNLOAD_LOAD(glCopyTexSubImage2D);                                                                              \
	GL_DYNLOAD_LOAD(glCreateProgram);                                                                                  \
	GL_DYNLOAD_LOAD(glCreateShader);                                                                                   \
	GL_DYNLOAD_LOAD(glCullFace);                                                                                       \
	GL_DYNLOAD_LOAD(glDeleteBuffers);                                                                                  \
	GL_DYNLOAD_LOAD(glDeleteFramebuffers);                                                                             \
	GL_DYNLOAD_LOAD(glDeleteProgram);                                                                                  \
	GL_DYNLOAD_LOAD(glDeleteRenderbuffers);                                                                            \
	GL_DYNLOAD_LOAD(glDeleteShader);                                                                                   \
	GL_DYNLOAD_LOAD(glDeleteTextures);                                                                                 \
	GL_DYNLOAD_LOAD(glDepthFunc);                                                                                      \
	GL_DYNLOAD_LOAD(glDepthMask);                                                                                      \
	GL_DYNLOAD_LOAD(glDepthRangef);                                                                                    \
	GL_DYNLOAD_LOAD(glDetachShader);                                                                                   \
	GL_DYNLOAD_LOAD(glDisable);                                                                                        \
	GL_DYNLOAD_LOAD(glDisableVertexAttribArray);                                                                       \
	GL_DYNLOAD_LOAD(glDrawArrays);                                                                                     \
	GL_DYNLOAD_LOAD(glDrawElements);                                                                                   \
	GL_DYNLOAD_LOAD(glEnable);                                                                                         \
	GL_DYNLOAD_LOAD(glEnableVertexAttribArray);                                                                        \
	GL_DYNLOAD_LOAD(glFinish);                                                                                         \
	GL_DYNLOAD_LOAD(glFlush);                                                                                          \
	GL_DYNLOAD_LOAD(glFramebufferRenderbuffer);                                                                        \
	GL_DYNLOAD_LOAD(glFramebufferTexture2D);                                                                           \
	GL_DYNLOAD_LOAD(glFrontFace);                                                                                      \
	GL_DYNLOAD_LOAD(glGenBuffers);                                                                                     \
	GL_DYNLOAD_LOAD(glGenerateMipmap);                                                                                 \
	GL_DYNLOAD_LOAD(glGenFramebuffers);                                                                                \
	GL_DYNLOAD_LOAD(glGenRenderbuffers);                                                                               \
	GL_DYNLOAD_LOAD(glGenTextures);                                                                                    \
	GL_DYNLOAD_LOAD(glGetActiveAttrib);                                                                                \
	GL_DYNLOAD_LOAD(glGetActiveUniform);                                                                               \
	GL_DYNLOAD_LOAD(glGetAttachedShaders);                                                                             \
	GL_DYNLOAD_LOAD(glGetAttribLocation);                                                                              \
	GL_DYNLOAD_LOAD(glGetBooleanv);                                                                                    \
	GL_DYNLOAD_LOAD(glGetBufferParameteriv);                                                                           \
	GL_DYNLOAD_LOAD(glGetError);                                                                                       \
	GL_DYNLOAD_LOAD(glGetFloatv);                                                                                      \
	GL_DYNLOAD_LOAD(glGetFramebufferAttachmentParameteriv);                                                            \
	GL_DYNLOAD_LOAD(glGetIntegerv);                                                                                    \
	GL_DYNLOAD_LOAD(glGetProgramiv);                                                                                   \
	GL_DYNLOAD_LOAD(glGetProgramInfoLog);                                                                              \
	GL_DYNLOAD_LOAD(glGetRenderbufferParameteriv);                                                                     \
	GL_DYNLOAD_LOAD(glGetShaderiv);                                                                                    \
	GL_DYNLOAD_LOAD(glGetShaderInfoLog);                                                                               \
	GL_DYNLOAD_LOAD(glGetShaderPrecisionFormat);                                                                       \
	GL_DYNLOAD_LOAD(glGetShaderSource);                                                                                \
	GL_DYNLOAD_LOAD(glGetString);                                                                                      \
	GL_DYNLOAD_LOAD(glGetTexParameterfv);                                                                              \
	GL_DYNLOAD_LOAD(glGetTexParameteriv);                                                                              \
	GL_DYNLOAD_LOAD(glGetUniformfv);                                                                                   \
	GL_DYNLOAD_LOAD(glGetUniformiv);                                                                                   \
	GL_DYNLOAD_LOAD(glGetUniformLocation);                                                                             \
	GL_DYNLOAD_LOAD(glGetVertexAttribfv);                                                                              \
	GL_DYNLOAD_LOAD(glGetVertexAttribiv);                                                                              \
	GL_DYNLOAD_LOAD(glGetVertexAttribPointerv);                                                                        \
	GL_DYNLOAD_LOAD(glHint);                                                                                           \
	GL_DYNLOAD_LOAD(glIsBuffer);                                                                                       \
	GL_DYNLOAD_LOAD(glIsEnabled);                                                                                      \
	GL_DYNLOAD_LOAD(glIsFramebuffer);                                                                                  \
	GL_DYNLOAD_LOAD(glIsProgram);                                                                                      \
	GL_DYNLOAD_LOAD(glIsRenderbuffer);                                                                                 \
	GL_DYNLOAD_LOAD(glIsShader);                                                                                       \
	GL_DYNLOAD_LOAD(glIsTexture);                                                                                      \
	GL_DYNLOAD_LOAD(glLineWidth);                                                                                      \
	GL_DYNLOAD_LOAD(glLinkProgram);                                                                                    \
	GL_DYNLOAD_LOAD(glPixelStorei);                                                                                    \
	GL_DYNLOAD_LOAD(glPolygonOffset);                                                                                  \
	GL_DYNLOAD_LOAD(glReadPixels);                                                                                     \
	GL_DYNLOAD_LOAD(glReleaseShaderCompiler);                                                                          \
	GL_DYNLOAD_LOAD(glRenderbufferStorage);                                                                            \
	GL_DYNLOAD_LOAD(glSampleCoverage);                                                                                 \
	GL_DYNLOAD_LOAD(glScissor);                                                                                        \
	GL_DYNLOAD_LOAD(glShaderBinary);                                                                                   \
	GL_DYNLOAD_LOAD(glShaderSource);                                                                                   \
	GL_DYNLOAD_LOAD(glStencilFunc);                                                                                    \
	GL_DYNLOAD_LOAD(glStencilFuncSeparate);                                                                            \
	GL_DYNLOAD_LOAD(glStencilMask);                                                                                    \
	GL_DYNLOAD_LOAD(glStencilMaskSeparate);                                                                            \
	GL_DYNLOAD_LOAD(glStencilOp);                                                                                      \
	GL_DYNLOAD_LOAD(glStencilOpSeparate);                                                                              \
	GL_DYNLOAD_LOAD(glTexImage2D);                                                                                     \
	GL_DYNLOAD_LOAD(glTexParameterf);                                                                                  \
	GL_DYNLOAD_LOAD(glTexParameterfv);                                                                                 \
	GL_DYNLOAD_LOAD(glTexParameteri);                                                                                  \
	GL_DYNLOAD_LOAD(glTexParameteriv);                                                                                 \
	GL_DYNLOAD_LOAD(glTexSubImage2D);                                                                                  \
	GL_DYNLOAD_LOAD(glUniform1f);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform1fv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform1i);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform1iv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform2f);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform2fv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform2i);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform2iv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform3f);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform3fv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform3i);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform3iv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform4f);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform4fv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniform4i);                                                                                      \
	GL_DYNLOAD_LOAD(glUniform4iv);                                                                                     \
	GL_DYNLOAD_LOAD(glUniformMatrix2fv);                                                                               \
	GL_DYNLOAD_LOAD(glUniformMatrix3fv);                                                                               \
	GL_DYNLOAD_LOAD(glUniformMatrix4fv);                                                                               \
	GL_DYNLOAD_LOAD(glUseProgram);                                                                                     \
	GL_DYNLOAD_LOAD(glValidateProgram);                                                                                \
	GL_DYNLOAD_LOAD(glVertexAttrib1f);                                                                                 \
	GL_DYNLOAD_LOAD(glVertexAttrib1fv);                                                                                \
	GL_DYNLOAD_LOAD(glVertexAttrib2f);                                                                                 \
	GL_DYNLOAD_LOAD(glVertexAttrib2fv);                                                                                \
	GL_DYNLOAD_LOAD(glVertexAttrib3f);                                                                                 \
	GL_DYNLOAD_LOAD(glVertexAttrib3fv);                                                                                \
	GL_DYNLOAD_LOAD(glVertexAttrib4f);                                                                                 \
	GL_DYNLOAD_LOAD(glVertexAttrib4fv);                                                                                \
	GL_DYNLOAD_LOAD(glVertexAttribPointer);                                                                            \
	GL_DYNLOAD_LOAD(glViewport)

typedef GL_APICALL void GL_APIENTRY glActiveTexture_t(GLenum texture);
typedef GL_APICALL void GL_APIENTRY glAttachShader_t(GLuint program, GLuint shader);
typedef GL_APICALL void GL_APIENTRY glBindAttribLocation_t(GLuint program, GLuint index, GLchar const* name);
typedef GL_APICALL void GL_APIENTRY glBindBuffer_t(GLenum target, GLuint buffer);
typedef GL_APICALL void GL_APIENTRY glBindFramebuffer_t(GLenum target, GLuint framebuffer);
typedef GL_APICALL void GL_APIENTRY glBindRenderbuffer_t(GLenum target, GLuint renderbuffer);
typedef GL_APICALL void GL_APIENTRY glBindTexture_t(GLenum target, GLuint texture);
typedef GL_APICALL void GL_APIENTRY glBlendColor_t(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef GL_APICALL void GL_APIENTRY glBlendEquation_t(GLenum mode);
typedef GL_APICALL void GL_APIENTRY glBlendEquationSeparate_t(GLenum modeRGB, GLenum modeAlpha);
typedef GL_APICALL void GL_APIENTRY glBlendFunc_t(GLenum sfactor, GLenum dfactor);
typedef GL_APICALL void GL_APIENTRY glBlendFuncSeparate_t(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha,
														  GLenum dfactorAlpha);
typedef GL_APICALL void GL_APIENTRY glBufferData_t(GLenum target, GLsizeiptr size, void const* data, GLenum usage);
typedef GL_APICALL void GL_APIENTRY glBufferSubData_t(GLenum target, GLintptr offset, GLsizeiptr size,
													  void const* data);
typedef GL_APICALL GLenum GL_APIENTRY glCheckFramebufferStatus_t(GLenum target);
typedef GL_APICALL void GL_APIENTRY glClear_t(GLbitfield mask);
typedef GL_APICALL void GL_APIENTRY glClearColor_t(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef GL_APICALL void GL_APIENTRY glClearDepthf_t(GLfloat d);
typedef GL_APICALL void GL_APIENTRY glClearStencil_t(GLint s);
typedef GL_APICALL void GL_APIENTRY glColorMask_t(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef GL_APICALL void GL_APIENTRY glCompileShader_t(GLuint shader);
typedef GL_APICALL void GL_APIENTRY glCompressedTexImage2D_t(GLenum target, GLint level, GLenum internalformat,
															 GLsizei width, GLsizei height, GLint border,
															 GLsizei imageSize, void const* data);
typedef GL_APICALL void GL_APIENTRY glCompressedTexSubImage2D_t(GLenum target, GLint level, GLint xoffset,
																GLint yoffset, GLsizei width, GLsizei height,
																GLenum format, GLsizei imageSize, void const* data);
typedef GL_APICALL void GL_APIENTRY glCopyTexImage2D_t(GLenum target, GLint level, GLenum internalformat, GLint x,
													   GLint y, GLsizei width, GLsizei height, GLint border);
typedef GL_APICALL void GL_APIENTRY glCopyTexSubImage2D_t(GLenum target, GLint level, GLint xoffset, GLint yoffset,
														  GLint x, GLint y, GLsizei width, GLsizei height);
typedef GL_APICALL GLuint GL_APIENTRY glCreateProgram_t(void);
typedef GL_APICALL GLuint GL_APIENTRY glCreateShader_t(GLenum type);
typedef GL_APICALL void GL_APIENTRY glCullFace_t(GLenum mode);
typedef GL_APICALL void GL_APIENTRY glDeleteBuffers_t(GLsizei n, GLuint const* buffers);
typedef GL_APICALL void GL_APIENTRY glDeleteFramebuffers_t(GLsizei n, GLuint const* framebuffers);
typedef GL_APICALL void GL_APIENTRY glDeleteProgram_t(GLuint program);
typedef GL_APICALL void GL_APIENTRY glDeleteRenderbuffers_t(GLsizei n, GLuint const* renderbuffers);
typedef GL_APICALL void GL_APIENTRY glDeleteShader_t(GLuint shader);
typedef GL_APICALL void GL_APIENTRY glDeleteTextures_t(GLsizei n, GLuint const* textures);
typedef GL_APICALL void GL_APIENTRY glDepthFunc_t(GLenum func);
typedef GL_APICALL void GL_APIENTRY glDepthMask_t(GLboolean flag);
typedef GL_APICALL void GL_APIENTRY glDepthRangef_t(GLfloat n, GLfloat f);
typedef GL_APICALL void GL_APIENTRY glDetachShader_t(GLuint program, GLuint shader);
typedef GL_APICALL void GL_APIENTRY glDisable_t(GLenum cap);
typedef GL_APICALL void GL_APIENTRY glDisableVertexAttribArray_t(GLuint index);
typedef GL_APICALL void GL_APIENTRY glDrawArrays_t(GLenum mode, GLint first, GLsizei count);
typedef GL_APICALL void GL_APIENTRY glDrawElements_t(GLenum mode, GLsizei count, GLenum type, void const* indices);
typedef GL_APICALL void GL_APIENTRY glEnable_t(GLenum cap);
typedef GL_APICALL void GL_APIENTRY glEnableVertexAttribArray_t(GLuint index);
typedef GL_APICALL void GL_APIENTRY glFinish_t(void);
typedef GL_APICALL void GL_APIENTRY glFlush_t(void);
typedef GL_APICALL void GL_APIENTRY glFramebufferRenderbuffer_t(GLenum target, GLenum attachment,
																GLenum renderbuffertarget, GLuint renderbuffer);
typedef GL_APICALL void GL_APIENTRY glFramebufferTexture2D_t(GLenum target, GLenum attachment, GLenum textarget,
															 GLuint texture, GLint level);
typedef GL_APICALL void GL_APIENTRY glFrontFace_t(GLenum mode);
typedef GL_APICALL void GL_APIENTRY glGenBuffers_t(GLsizei n, GLuint* buffers);
typedef GL_APICALL void GL_APIENTRY glGenerateMipmap_t(GLenum target);
typedef GL_APICALL void GL_APIENTRY glGenFramebuffers_t(GLsizei n, GLuint* framebuffers);
typedef GL_APICALL void GL_APIENTRY glGenRenderbuffers_t(GLsizei n, GLuint* renderbuffers);
typedef GL_APICALL void GL_APIENTRY glGenTextures_t(GLsizei n, GLuint* textures);
typedef GL_APICALL void GL_APIENTRY glGetActiveAttrib_t(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length,
														GLint* size, GLenum* type, GLchar* name);
typedef GL_APICALL void GL_APIENTRY glGetActiveUniform_t(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length,
														 GLint* size, GLenum* type, GLchar* name);
typedef GL_APICALL void GL_APIENTRY glGetAttachedShaders_t(GLuint program, GLsizei maxCount, GLsizei* count,
														   GLuint* shaders);
typedef GL_APICALL GLint GL_APIENTRY glGetAttribLocation_t(GLuint program, GLchar const* name);
typedef GL_APICALL void GL_APIENTRY glGetBooleanv_t(GLenum pname, GLboolean* data);
typedef GL_APICALL void GL_APIENTRY glGetBufferParameteriv_t(GLenum target, GLenum pname, GLint* params);
typedef GL_APICALL GLenum GL_APIENTRY glGetError_t(void);
typedef GL_APICALL void GL_APIENTRY glGetFloatv_t(GLenum pname, GLfloat* data);
typedef GL_APICALL void GL_APIENTRY glGetFramebufferAttachmentParameteriv_t(GLenum target, GLenum attachment,
																			GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetIntegerv_t(GLenum pname, GLint* data);
typedef GL_APICALL void GL_APIENTRY glGetProgramiv_t(GLuint program, GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetProgramInfoLog_t(GLuint program, GLsizei bufSize, GLsizei* length,
														  GLchar* infoLog);
typedef GL_APICALL void GL_APIENTRY glGetRenderbufferParameteriv_t(GLenum target, GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetShaderiv_t(GLuint shader, GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length,
														 GLchar* infoLog);
typedef GL_APICALL void GL_APIENTRY glGetShaderPrecisionFormat_t(GLenum shadertype, GLenum precisiontype, GLint* range,
																 GLint* precision);
typedef GL_APICALL void GL_APIENTRY glGetShaderSource_t(GLuint shader, GLsizei bufSize, GLsizei* length,
														GLchar* source);
typedef GL_APICALL const GLubyte* GL_APIENTRY glGetString_t(GLenum name);
typedef GL_APICALL void GL_APIENTRY glGetTexParameterfv_t(GLenum target, GLenum pname, GLfloat* params);
typedef GL_APICALL void GL_APIENTRY glGetTexParameteriv_t(GLenum target, GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetUniformfv_t(GLuint program, GLint location, GLfloat* params);
typedef GL_APICALL void GL_APIENTRY glGetUniformiv_t(GLuint program, GLint location, GLint* params);
typedef GL_APICALL GLint GL_APIENTRY glGetUniformLocation_t(GLuint program, GLchar const* name);
typedef GL_APICALL void GL_APIENTRY glGetVertexAttribfv_t(GLuint index, GLenum pname, GLfloat* params);
typedef GL_APICALL void GL_APIENTRY glGetVertexAttribiv_t(GLuint index, GLenum pname, GLint* params);
typedef GL_APICALL void GL_APIENTRY glGetVertexAttribPointerv_t(GLuint index, GLenum pname, void** pointer);
typedef GL_APICALL void GL_APIENTRY glHint_t(GLenum target, GLenum mode);
typedef GL_APICALL GLboolean GL_APIENTRY glIsBuffer_t(GLuint buffer);
typedef GL_APICALL GLboolean GL_APIENTRY glIsEnabled_t(GLenum cap);
typedef GL_APICALL GLboolean GL_APIENTRY glIsFramebuffer_t(GLuint framebuffer);
typedef GL_APICALL GLboolean GL_APIENTRY glIsProgram_t(GLuint program);
typedef GL_APICALL GLboolean GL_APIENTRY glIsRenderbuffer_t(GLuint renderbuffer);
typedef GL_APICALL GLboolean GL_APIENTRY glIsShader_t(GLuint shader);
typedef GL_APICALL GLboolean GL_APIENTRY glIsTexture_t(GLuint texture);
typedef GL_APICALL void GL_APIENTRY glLineWidth_t(GLfloat width);
typedef GL_APICALL void GL_APIENTRY glLinkProgram_t(GLuint program);
typedef GL_APICALL void GL_APIENTRY glPixelStorei_t(GLenum pname, GLint param);
typedef GL_APICALL void GL_APIENTRY glPolygonOffset_t(GLfloat factor, GLfloat units);
typedef GL_APICALL void GL_APIENTRY glReadPixels_t(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,
												   GLenum type, void* pixels);
typedef GL_APICALL void GL_APIENTRY glReleaseShaderCompiler_t(void);
typedef GL_APICALL void GL_APIENTRY glRenderbufferStorage_t(GLenum target, GLenum internalformat, GLsizei width,
															GLsizei height);
typedef GL_APICALL void GL_APIENTRY glSampleCoverage_t(GLfloat value, GLboolean invert);
typedef GL_APICALL void GL_APIENTRY glScissor_t(GLint x, GLint y, GLsizei width, GLsizei height);
typedef GL_APICALL void GL_APIENTRY glShaderBinary_t(GLsizei count, GLuint const* shaders, GLenum binaryFormat,
													 void const* binary, GLsizei length);
typedef GL_APICALL void GL_APIENTRY glShaderSource_t(GLuint shader, GLsizei count, GLchar const* const* string,
													 GLint const* length);
typedef GL_APICALL void GL_APIENTRY glStencilFunc_t(GLenum func, GLint ref, GLuint mask);
typedef GL_APICALL void GL_APIENTRY glStencilFuncSeparate_t(GLenum face, GLenum func, GLint ref, GLuint mask);
typedef GL_APICALL void GL_APIENTRY glStencilMask_t(GLuint mask);
typedef GL_APICALL void GL_APIENTRY glStencilMaskSeparate_t(GLenum face, GLuint mask);
typedef GL_APICALL void GL_APIENTRY glStencilOp_t(GLenum fail, GLenum zfail, GLenum zpass);
typedef GL_APICALL void GL_APIENTRY glStencilOpSeparate_t(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef GL_APICALL void GL_APIENTRY glTexImage2D_t(GLenum target, GLint level, GLint internalformat, GLsizei width,
												   GLsizei height, GLint border, GLenum format, GLenum type,
												   void const* pixels);
typedef GL_APICALL void GL_APIENTRY glTexParameterf_t(GLenum target, GLenum pname, GLfloat param);
typedef GL_APICALL void GL_APIENTRY glTexParameterfv_t(GLenum target, GLenum pname, GLfloat const* params);
typedef GL_APICALL void GL_APIENTRY glTexParameteri_t(GLenum target, GLenum pname, GLint param);
typedef GL_APICALL void GL_APIENTRY glTexParameteriv_t(GLenum target, GLenum pname, GLint const* params);
typedef GL_APICALL void GL_APIENTRY glTexSubImage2D_t(GLenum target, GLint level, GLint xoffset, GLint yoffset,
													  GLsizei width, GLsizei height, GLenum format, GLenum type,
													  void const* pixels);
typedef GL_APICALL void GL_APIENTRY glUniform1f_t(GLint location, GLfloat v0);
typedef GL_APICALL void GL_APIENTRY glUniform1fv_t(GLint location, GLsizei count, GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniform1i_t(GLint location, GLint v0);
typedef GL_APICALL void GL_APIENTRY glUniform1iv_t(GLint location, GLsizei count, GLint const* value);
typedef GL_APICALL void GL_APIENTRY glUniform2f_t(GLint location, GLfloat v0, GLfloat v1);
typedef GL_APICALL void GL_APIENTRY glUniform2fv_t(GLint location, GLsizei count, GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniform2i_t(GLint location, GLint v0, GLint v1);
typedef GL_APICALL void GL_APIENTRY glUniform2iv_t(GLint location, GLsizei count, GLint const* value);
typedef GL_APICALL void GL_APIENTRY glUniform3f_t(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef GL_APICALL void GL_APIENTRY glUniform3fv_t(GLint location, GLsizei count, GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniform3i_t(GLint location, GLint v0, GLint v1, GLint v2);
typedef GL_APICALL void GL_APIENTRY glUniform3iv_t(GLint location, GLsizei count, GLint const* value);
typedef GL_APICALL void GL_APIENTRY glUniform4f_t(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef GL_APICALL void GL_APIENTRY glUniform4fv_t(GLint location, GLsizei count, GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniform4i_t(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef GL_APICALL void GL_APIENTRY glUniform4iv_t(GLint location, GLsizei count, GLint const* value);
typedef GL_APICALL void GL_APIENTRY glUniformMatrix2fv_t(GLint location, GLsizei count, GLboolean transpose,
														 GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniformMatrix3fv_t(GLint location, GLsizei count, GLboolean transpose,
														 GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUniformMatrix4fv_t(GLint location, GLsizei count, GLboolean transpose,
														 GLfloat const* value);
typedef GL_APICALL void GL_APIENTRY glUseProgram_t(GLuint program);
typedef GL_APICALL void GL_APIENTRY glValidateProgram_t(GLuint program);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib1f_t(GLuint index, GLfloat x);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib1fv_t(GLuint index, GLfloat const* v);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib2f_t(GLuint index, GLfloat x, GLfloat y);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib2fv_t(GLuint index, GLfloat const* v);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib3f_t(GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib3fv_t(GLuint index, GLfloat const* v);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib4f_t(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef GL_APICALL void GL_APIENTRY glVertexAttrib4fv_t(GLuint index, GLfloat const* v);
typedef GL_APICALL void GL_APIENTRY glVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized,
															GLsizei stride, void const* pointer);
typedef GL_APICALL void GL_APIENTRY glViewport_t(GLint x, GLint y, GLsizei width, GLsizei height);

