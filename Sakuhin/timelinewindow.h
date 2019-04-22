#ifndef TIMELINEWINDOW_H
#define TIMELINEWINDOW_H

#include "window.h"

class TimelineWindow : public Window {
    public:
        TimelineWindow();
        void processTime();
        void setupRecordingTime();
        void handleRecordingTime();
};

#endif // TIMELINEWINDOW_H
