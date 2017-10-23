#include "window.h"
#include <QDebug>
#include <QString>

static GLfloat const rectangle[] = {
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

void Window::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));


    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 450 core\n"
        "layout(location = 0) in vec3 position;\n"

        "void main() {\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "}\n"
    );

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        "#version 450 core\n"

        "uniform vec2 resolution;\n"

        "out vec4 color;\n"

        "void main() {\n"
        "   color = vec4(resolution.x / 1024., 1., resolution.y / 1024, 1.);\n"
        "}\n"
    );

    shader.link();

    shader.bind();
        vbo.create();
        vbo.bind();
            vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo.allocate(rectangle, sizeof(rectangle));

            vao.create();
            vao.bind();
                shader.enableAttributeArray(0);
                shader.setAttributeBuffer(0, GL_FLOAT, 0, 3);
            vao.release();
        vbo.release();
    shader.release();
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.bind();
    shader.setUniformValue("resolution", width(), height());

        vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.release();
    shader.release();
}
