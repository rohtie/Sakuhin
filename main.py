from PyQt5.QtGui import QGuiApplication, QSurfaceFormat

from lib.shaderwindow import ShaderWindow

if __name__ == '__main__':
    import sys

    app = QGuiApplication(sys.argv)

    window = ShaderWindow()
    window.setFormat(QSurfaceFormat())
    window.show()
    window.setAnimating(True)

    sys.exit(app.exec_())
