from PyQt5.QtCore import QFileSystemWatcher

from lib.openglwindow import SharedContextOpenGLWindow
from lib.shaderdisplaymixin import ShaderDisplayMixin


class ShaderWindow(SharedContextOpenGLWindow, ShaderDisplayMixin):
    def __init__(self, dashboard_window):
        super(ShaderWindow, self).__init__(
            dashboard_window.shader_preview_widget.context()
        )

        self.time = dashboard_window.time

        file_watcher = QFileSystemWatcher(self)
        file_watcher.addPath('default.frag')
        file_watcher.fileChanged.connect(self.on_file_change)

        self.shader = 0

        dashboard_window.toggle_fullscreen_action.triggered.connect(
            self.toggle_fullscreen
        )
        self.is_full_screen = False


    def initializeGL(self):
        self.compile_shaders()


    def paintGL(self):
        self.draw_shaders(self.gl)


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
