from PyQt5 import QtCore
from PyQt5.QtGui import QSurfaceFormat
from PyQt5.QtWidgets import QApplication

from lib.shaderwindow import ShaderWindow
from lib.dashboardwindow import DashboardWindow


if __name__ == '__main__':
    import sys

    app = QApplication(sys.argv)
    desktop_size = app.desktop().availableGeometry()

    shader_window = ShaderWindow()
    shader_window.setFormat(QSurfaceFormat())
    shader_window.setFlags(QtCore.Qt.WindowStaysOnTopHint)
    shader_window.show()
    shader_window.setAnimating(True)

    dashboard_window = DashboardWindow(shader_window)
    dashboard_window.resize(desktop_size.width() * 0.5, desktop_size.height())
    dashboard_window.show()

    sys.exit(app.exec_())
