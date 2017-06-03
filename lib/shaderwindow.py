from PyQt5 import QtCore
from PyQt5.QtGui import (
    QOpenGLShader,
    QOpenGLShaderProgram
)

from lib.openglwindow import OpenGLWindow


class ShaderWindow(OpenGLWindow):
    vertexShaderSource = '''
    in vec4 position;

    void main(){
        gl_Position = position;
    }
    '''

    fragmentShaderSource = '''
    uniform float time;
    uniform vec2 resolution;

    void main() {
        vec2 p = gl_FragCoord.xy;

        p /= resolution.xy;
        p -= 0.5;
        p.x *= resolution.x / resolution.y;

        gl_FragColor = vec4(vec3(smoothstep(0.35, 0.35 + sin(time) * 0.1, length(p))), 1.0);
    }
    '''

    def __init__(self):
        super(ShaderWindow, self).__init__()

        self.shader = 0
        self.time = QtCore.QTime()
        self.time.start()


    def initialize(self):
        self.shader = QOpenGLShaderProgram(self)

        self.shader.addShaderFromSourceCode(
            QOpenGLShader.Vertex,
            self.vertexShaderSource)

        self.shader.addShaderFromSourceCode(
            QOpenGLShader.Fragment,
            self.fragmentShaderSource)

        self.shader.link()


    def render(self, gl):
        width, height = self.width(), self.height()

        gl.glViewport(0, 0, width, height)
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        gl.glMatrixMode(gl.GL_MODELVIEW)

        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        self.shader.bind()

        self.shader.setUniformValue("time", self.time.elapsed() / 1000.0)
        self.shader.setUniformValue("resolution", width, height)

        # Fullscreen quad
        gl.glBegin(gl.GL_QUADS)
        gl.glVertex2f(-1, -1)
        gl.glVertex2f(1, -1)
        gl.glVertex2f(1, 1)
        gl.glVertex2f(-1, 1)
        gl.glEnd()

        self.shader.release()