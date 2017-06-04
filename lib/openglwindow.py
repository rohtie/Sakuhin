from PyQt5.QtCore import QEvent, pyqtSignal
from PyQt5.QtGui import (
    QGuiApplication,
    QOpenGLContext,
    QOpenGLVersionProfile,
    QWindow
)


class SharedContextOpenGLWindow(QWindow):
    change_window_size = pyqtSignal(int, int)

    def __init__(self, share_context, parent=None):
        super(SharedContextOpenGLWindow, self).__init__(parent)

        self.share_context = share_context

        self.has_scheduled_update = False
        self.gl_context = None
        self.gl = None

        self.setSurfaceType(QWindow.OpenGLSurface)


    def initializeGL(self):
        pass


    def renderLater(self):
        if not self.has_scheduled_update:
            self.has_scheduled_update = True
            QGuiApplication.postEvent(self, QEvent(QEvent.UpdateRequest))


    def renderNow(self):
        if not self.isExposed():
            return

        self.has_scheduled_update = False

        needsInitialize = False

        if self.gl_context is None:
            self.gl_context = QOpenGLContext(self)
            self.gl_context.setFormat(self.requestedFormat())
            self.gl_context.setShareContext(self.share_context)
            self.gl_context.create()

            needsInitialize = True

        self.gl_context.makeCurrent(self)

        if needsInitialize:
            profile = QOpenGLVersionProfile()
            profile.setVersion(2, 1)

            self.gl = self.gl_context.versionFunctions(profile)
            self.gl.initializeOpenGLFunctions()

            self.initializeGL()

        self.paintGL()

        self.gl_context.swapBuffers(self)

        self.renderLater()


    def event(self, event):
        if event.type() == QEvent.UpdateRequest:
            self.renderNow()
            return True

        return super(SharedContextOpenGLWindow, self).event(event)


    def exposeEvent(self, event):
        self.renderNow()


    def resizeEvent(self, event):
        self.change_window_size.emit(self.width(), self.height())
        self.renderNow()
