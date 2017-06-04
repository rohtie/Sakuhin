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

        self.last_width = 1
        self.last_height = 1
        self.max_width = 1
        self.max_height = 1


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


    def resize_correctly(self, width, height):
        new_width = (width / height) * self.max_height
        new_height = self.max_height

        if new_width > self.max_width:
            new_width = self.max_width
            new_height = (height / width) * self.max_width

        self.resize(new_width, new_height)


    def on_dashboard_window_resize(self, width, height):
        # Using our own max variables because setMaximum will trigger a resize
        # which overrides our resize
        self.max_width = width
        self.max_height = height * 0.25

        self.resize_correctly(self.last_width, self.last_height)


    def on_shader_window_resize(self, width, height):
        self.last_width = width
        self.last_height = height

        self.resize_correctly(width, height)
