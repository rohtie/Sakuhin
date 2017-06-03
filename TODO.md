# Make fullscreen when pressing F11
    window.showFullScreen()

    Maximizing might be better for separate screen:
    window.setFlags(QtCore.Qt.WindowStaysOnTopHint | QtCore.Qt.FramelessWindowHint)
    window.showMaximized()

