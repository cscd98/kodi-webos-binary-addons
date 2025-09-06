#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>

int main(void) {
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    EGLint num_config;

    // 1. Get default display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("eglGetDisplay failed\n");
        return 1;
    }

    // 2. Initialize EGL
    if (!eglInitialize(display, NULL, NULL)) {
        printf("eglInitialize failed\n");
        return 1;
    }

    // 3. Choose a basic config
    EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
        EGL_NONE
    };

    if (!eglChooseConfig(display, attribs, &config, 1, &num_config)) {
        printf("eglChooseConfig failed\n");
        return 1;
    }

    // 4. Create a tiny 1×1 pbuffer surface
    EGLint pbuf_attribs[] = {
        EGL_WIDTH,  1,
        EGL_HEIGHT, 1,
        EGL_NONE,
    };

    surface = eglCreatePbufferSurface(display, config, pbuf_attribs);
    if (surface == EGL_NO_SURFACE) {
        printf("eglCreatePbufferSurface failed\n");
        return 1;
    }

    // 5. Create GLES2 context
    EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attribs);
    if (context == EGL_NO_CONTEXT) {
        printf("eglCreateContext failed\n");
        return 1;
    }

    // 6. Make context current
    if (!eglMakeCurrent(display, surface, surface, context)) {
        printf("eglMakeCurrent failed\n");
        return 1;
    }

    // 7. Print GPU driver info
    printf("GL_VENDOR    : %s\n",   glGetString(GL_VENDOR));
    printf("GL_RENDERER  : %s\n",   glGetString(GL_RENDERER));
    printf("GL_VERSION   : %s\n",   glGetString(GL_VERSION));
    printf("GL_EXTENSIONS: %s\n",   glGetString(GL_EXTENSIONS));

    // 8. Cleanup
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);

    return 0;
}

