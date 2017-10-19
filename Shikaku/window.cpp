#include "window.h"
#include <QDebug>
#include <QString>

Window::~Window() {
    makeCurrent();

    // TODO: destroy data
}

void Window::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::resizeGL(int width, int height) {
    qDebug() << "GL window" << width << "x" << height;
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
}
