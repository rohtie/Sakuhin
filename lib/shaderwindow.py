from PyQt5 import QtCore
from PyQt5.QtCore import QFileSystemWatcher
from PyQt5.QtGui import (
    QOpenGLShader,
    QOpenGLShaderProgram
)

from lib.openglwindow import OpenGLWindow


class ShaderWindow(OpenGLWindow):
    def __init__(self):
        super(ShaderWindow, self).__init__()

        self.shader = 0

        self.time = QtCore.QTime()
        self.time.start()

        file_watcher = QFileSystemWatcher(self)
        file_watcher.addPath('default.frag')
        file_watcher.fileChanged.connect(self.on_file_change)

        self.is_full_screen = False


    def compile_shaders(self):
        self.shader = QOpenGLShaderProgram(self)

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Vertex,
            'default.vert')

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Fragment,
            'default.frag')

        self.shader.link()


    def initialize(self):
        self.compile_shaders()


    def render(self, gl):
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


    def on_file_change(self, filename):
        self.shader.removeAllShaders()
        self.compile_shaders()


    def toggle_fullscreen(self):
        if self.is_full_screen:
            self.showNormal()
            self.is_full_screen = False

        else:
            self.showFullScreen()
            self.is_full_screen = True
