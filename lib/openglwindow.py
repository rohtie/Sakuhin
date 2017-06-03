from PyQt5.QtCore import QEvent
from PyQt5.QtGui import (
    QGuiApplication,
    QOpenGLContext,
    QOpenGLVersionProfile,
    QWindow
)


class OpenGLWindow(QWindow):
    def __init__(self, parent=None):
        super(OpenGLWindow, self).__init__(parent)

        self.m_update_pending = False
        self.m_animating = False
        self.m_context = None
        self.m_gl = None

        self.setSurfaceType(QWindow.OpenGLSurface)


    def initialize(self):
        pass


    def setAnimating(self, animating):
        self.m_animating = animating

        if animating:
            self.renderLater()


    def renderLater(self):
        if not self.m_update_pending:
            self.m_update_pending = True
            QGuiApplication.postEvent(self, QEvent(QEvent.UpdateRequest))


    def renderNow(self):
        if not self.isExposed():
            return

        self.m_update_pending = False

        needsInitialize = False

        if self.m_context is None:
            self.m_context = QOpenGLContext(self)
            self.m_context.setFormat(self.requestedFormat())
            self.m_context.create()

            needsInitialize = True

        self.m_context.makeCurrent(self)

        if needsInitialize:
            profile = QOpenGLVersionProfile()
            profile.setVersion(2, 1)

            self.m_gl = self.m_context.versionFunctions(profile)
            self.m_gl.initializeOpenGLFunctions()

            self.initialize()

        self.render(self.m_gl)

        self.m_context.swapBuffers(self)

        if self.m_animating:
            self.renderLater()


    def event(self, event):
        if event.type() == QEvent.UpdateRequest:
            self.renderNow()
            return True

        return super(OpenGLWindow, self).event(event)


    def exposeEvent(self, event):
        self.renderNow()


    def resizeEvent(self, event):
        self.renderNow()
