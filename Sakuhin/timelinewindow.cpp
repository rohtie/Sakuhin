#include "timelinewindow.h"

TimelineWindow::TimelineWindow() {

}

void TimelineWindow::processTime() {
    currentTime = scenemanager->markerPosition;
    performanceTime = time.elapsed();
}
