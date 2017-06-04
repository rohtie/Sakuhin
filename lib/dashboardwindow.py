from PyQt5.QtWidgets import QMainWindow, QAction

from lib.shaderpreviewwidget import ShaderPreviewWidget

class DashboardWindow(QMainWindow):
    def __init__(self, shader_window):
        super(DashboardWindow, self).__init__()

        shader_preview_widget = ShaderPreviewWidget()
        self.setCentralWidget(shader_preview_widget)

        toggle_fullscreen_action = QAction("&Toggle fullscreen", self)
        toggle_fullscreen_action.triggered.connect(shader_window.toggle_fullscreen)

        self.shader_window_menu = self.menuBar().addMenu("&Shader window")
        self.shader_window_menu.addAction(toggle_fullscreen_action)
