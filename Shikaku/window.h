#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
	Q_OBJECT

	public:
	  ~Window();

	  void initializeGL();
	  void resizeGL(int width, int height);
	  void paintGL();
};

#endif // WINDOW_H
