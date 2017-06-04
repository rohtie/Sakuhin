from PyQt5.QtCore import QTimer, QFileSystemWatcher
from PyQt5.QtGui import QOpenGLVersionProfile
from PyQt5.QtWidgets import QOpenGLWidget

from lib.shaderdisplaymixin import ShaderDisplayMixin


class ShaderPreviewWidget(QOpenGLWidget, ShaderDisplayMixin):
    def __init__(self, time, parent=None):
        super(ShaderPreviewWidget, self).__init__(parent)

        self.time = time

        timer = QTimer(self)
        timer.timeout.connect(self.update)
        timer.start(10)

        file_watcher = QFileSystemWatcher(self)
        file_watcher.addPath('default.frag')
        file_watcher.fileChanged.connect(self.on_file_change)

        self.shader = 0


    def initializeGL(self):
        profile = QOpenGLVersionProfile()
        profile.setVersion(2, 1)

        self.gl = self.context().versionFunctions(profile)
        self.gl.initializeOpenGLFunctions()

        self.compile_shaders()


    def paintGL(self):
        self.draw_shaders(self.gl)


    def on_file_change(self, filename):
        self.shader.removeAllShaders()
        self.compile_shaders()


    def resizeGL(self, width, height):
        pass

