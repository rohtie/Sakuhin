from PyQt5.QtCore import QElapsedTimer, pyqtSignal, QDir
from PyQt5.QtGui import QImage, QPixmap, QOpenGLTexture
from PyQt5.QtWidgets import (
    QMainWindow, QAction, QVBoxLayout, QWidget, QPushButton, QGridLayout,
    QFileDialog, QLabel
)

from lib.shaderpreviewwidget import ShaderPreviewWidget


class DashboardWindow(QMainWindow):
    change_window_size = pyqtSignal(int, int)
    load_texture = pyqtSignal(QOpenGLTexture)

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

        grid_layout = QGridLayout()
        vertical_layout.addLayout(grid_layout)

        texture_button = QPushButton()
        grid_layout.addWidget(texture_button, 0, 0)
        texture_button.clicked.connect(self.load_texture_from_file)

        self.img_display = QLabel()
        grid_layout.addWidget(self.img_display, 0, 1)


        self.load_texture.connect(self.shader_preview_widget.add_texture)
        self.change_window_size.connect(
            self.shader_preview_widget.on_dashboard_window_resize
        )

        self.toggle_fullscreen_action = QAction("&Toggle fullscreen", self)
        self.shader_window_menu = self.menuBar().addMenu("&Shader window")
        self.shader_window_menu.addAction(self.toggle_fullscreen_action)


    def load_texture_from_file(self):
        fileName, _ = QFileDialog.getOpenFileName(
            self, "Open File",
            QDir.currentPath()
        )

        if fileName:
            image = QImage(fileName)

            self.img_display.setPixmap(QPixmap.fromImage(image))
            self.load_texture.emit(QOpenGLTexture(image.mirrored()))


    def resizeEvent(self, event):
        super(DashboardWindow, self).resizeEvent(event)

        self.change_window_size.emit(self.width(), self.height())
