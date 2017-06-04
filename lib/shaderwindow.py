from PyQt5 import QtCore
from PyQt5.QtCore import QFileSystemWatcher
from PyQt5.QtGui import (
    QOpenGLShader,
    QOpenGLShaderProgram
)

from lib.openglwindow import OpenGLWindow
from lib.shaderdisplaymixin import ShaderDisplayMixin


class ShaderWindow(OpenGLWindow, ShaderDisplayMixin):
    def __init__(self):
        super(ShaderWindow, self).__init__()

        self.shader = 0

        self.setup_shader_time()

        file_watcher = QFileSystemWatcher(self)
        file_watcher.addPath('default.frag')
        file_watcher.fileChanged.connect(self.on_file_change)

        self.is_full_screen = False


    def initialize(self):
        self.compile_shaders()


    def render(self, gl):
        self.draw_shaders(gl)


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
