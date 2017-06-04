from PyQt5.QtCore import QElapsedTimer, pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QAction, QVBoxLayout, QWidget

from lib.shaderpreviewwidget import ShaderPreviewWidget


class DashboardWindow(QMainWindow):
    change_window_size = pyqtSignal(int, int)

    def __init__(self):
        super(DashboardWindow, self).__init__()

        self.time = QElapsedTimer()
        self.time.start()


        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        vertical_layout = QVBoxLayout(central_widget)
        vertical_layout.setContentsMargins(0, 0, 0, 0);

        self.shader_preview_widget = ShaderPreviewWidget(self.time)

        vertical_layout.addWidget(self.shader_preview_widget)

        self.change_window_size.connect(
            self.shader_preview_widget.on_dashboard_window_resize
        )

        self.toggle_fullscreen_action = QAction("&Toggle fullscreen", self)
        self.shader_window_menu = self.menuBar().addMenu("&Shader window")
        self.shader_window_menu.addAction(self.toggle_fullscreen_action)


    def resizeEvent(self, event):
        super(DashboardWindow, self).resizeEvent(event)

        self.change_window_size.emit(self.width(), self.height())
