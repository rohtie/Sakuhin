#include "timelinewindow.h"

TimelineWindow::TimelineWindow() {

}

void TimelineWindow::processTime() {
    currentTime = time.elapsed();
    performanceTime = currentTime;
}
