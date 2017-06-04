from PyQt5.QtCore import QElapsedTimer
from PyQt5.QtWidgets import QMainWindow, QAction

from lib.shaderpreviewwidget import ShaderPreviewWidget


class DashboardWindow(QMainWindow):
    def __init__(self):
        super(DashboardWindow, self).__init__()

        self.time = QElapsedTimer()
        self.time.start()

        self.shader_preview_widget = ShaderPreviewWidget(self.time)
        self.setCentralWidget(self.shader_preview_widget)

        self.toggle_fullscreen_action = QAction("&Toggle fullscreen", self)
        self.shader_window_menu = self.menuBar().addMenu("&Shader window")
        self.shader_window_menu.addAction(self.toggle_fullscreen_action)
