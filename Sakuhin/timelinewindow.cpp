#include "timelinewindow.h"

#include "scenemanager.h"

TimelineWindow::TimelineWindow() {

}

void TimelineWindow::processTime() {
    currentTime = scenemanager->markerPosition;
    performanceTime = time.elapsed();
}
