from PyQt5.QtCore import QElapsedTimer
from PyQt5.QtGui import (
    QOpenGLShader,
    QOpenGLShaderProgram
)

class ShaderDisplayMixin():
    def setup_shader_time(self):
        self.time = QElapsedTimer()
        self.time.start()


    def compile_shaders(self, fragment_shader='default.frag'):
        self.shader = QOpenGLShaderProgram(self)

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Vertex,
            'default.vert')

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Fragment,
            fragment_shader)

        self.shader.link()


    def draw_shaders(self, gl):
        width, height = self.width(), self.height()

        gl.glViewport(0, 0, width, height)
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        gl.glMatrixMode(gl.GL_MODELVIEW)

        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        self.shader.bind()

        self.shader.setUniformValue("time", self.time.elapsed() / 1000.0)
        self.shader.setUniformValue("resolution", width, height)

        gl.glBegin(gl.GL_QUADS)
        gl.glVertex2f(-1, -1)
        gl.glVertex2f(1, -1)
        gl.glVertex2f(1, 1)
        gl.glVertex2f(-1, 1)
        gl.glEnd()

        self.shader.release()
