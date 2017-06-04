from PyQt5 import QtCore
from PyQt5.QtGui import QSurfaceFormat
from PyQt5.QtWidgets import QApplication

from lib.shaderwindow import ShaderWindow
from lib.dashboardwindow import DashboardWindow


if __name__ == '__main__':
    import sys

    app = QApplication(sys.argv)
    desktop_size = app.desktop().availableGeometry()

    dashboard_window = DashboardWindow()
    dashboard_window.resize(desktop_size.width() * 0.5, desktop_size.height())
    dashboard_window.show()

    shader_window = ShaderWindow(dashboard_window)
    shader_window.setFormat(QSurfaceFormat())
    shader_window.setFlags(QtCore.Qt.WindowStaysOnTopHint)
    shader_window.resize(512, 288)
    shader_window.show()

    sys.exit(app.exec_())
