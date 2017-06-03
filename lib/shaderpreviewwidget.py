from PyQt5 import QtCore
from PyQt5.QtCore import QFileSystemWatcher, QTimer
from PyQt5.QtGui import (
    QOpenGLVersionProfile,
    QOpenGLShader,
    QOpenGLShaderProgram
)
from PyQt5.QtWidgets import QOpenGLWidget


class ShaderPreviewWidget(QOpenGLWidget):
    def __init__(self, parent=None):
        super(ShaderPreviewWidget, self).__init__(parent)

        timer = QTimer(self)
        timer.timeout.connect(self.update)
        timer.start(10)

        self.shader = 0

        self.time = QtCore.QTime()
        self.time.start()

        file_watcher = QFileSystemWatcher(self)
        file_watcher.addPath('default.frag')
        file_watcher.fileChanged.connect(self.on_file_change)


    def compile_shaders(self):
        self.shader = QOpenGLShaderProgram(self)

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Vertex,
            'default.vert')

        self.shader.addShaderFromSourceFile(
            QOpenGLShader.Fragment,
            'default.frag')

        self.shader.link()


    def initializeGL(self):
        profile = QOpenGLVersionProfile()
        profile.setVersion(2, 1)

        self.gl = self.context().versionFunctions(profile)
        self.gl.initializeOpenGLFunctions()

        self.compile_shaders()


    def paintGL(self):
        width, height = self.width(), self.height()

        self.gl.glViewport(0, 0, width, height)
        self.gl.glMatrixMode(self.gl.GL_PROJECTION)
        self.gl.glLoadIdentity()
        self.gl.glMatrixMode(self.gl.GL_MODELVIEW)

        self.gl.glClear(self.gl.GL_COLOR_BUFFER_BIT)

        self.shader.bind()

        self.shader.setUniformValue("time", self.time.elapsed() / 1000.0)
        self.shader.setUniformValue("resolution", width, height)

        self.gl.glBegin(self.gl.GL_QUADS)
        self.gl.glVertex2f(-1, -1)
        self.gl.glVertex2f(1, -1)
        self.gl.glVertex2f(1, 1)
        self.gl.glVertex2f(-1, 1)
        self.gl.glEnd()

        self.shader.release()


    def on_file_change(self, filename):
        self.shader.removeAllShaders()
        self.compile_shaders()


    def resizeGL(self, width, height):
        pass
