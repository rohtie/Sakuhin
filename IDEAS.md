* Window layout
    * UI dashboard
    * Main window
    * Preview window (same as main window)

* Shader manager
    * Create shader (left click shaders menu)
        * New file is created in the shader directory named `unixtime_sec`.glsl
        * File is populated with chosen template: minimal/feedback/raymarch
        * The new shader is rendered to the preview window

    * Select shader (left click on shader)
        * Currently selected shader is saved to shader directory
        * Selected shader is loaded into session.glsl
        * Channels and sliders are updated to their last known state
        * The selected shader is rendered to the preview window

    * Shader context menu (right click on shader area)
        * Make current

    * Create transition (left click transition menu)
        * Create shader with transition template
        * The two first channels are populated with placeholder images
        * Placeholder images are white and black
        * Current transition is selected and rendered to preview window

    * Select transition (left click on transition)
        * Render transition with placeholder images to preview window

    * Transition context menu (right click on transition area)
        * Transition  1 Sec
        * Transition  5 Sec
        * Transition 10 Sec
        * Transition 20 Sec
        * Transition 30 Sec
        * Transition custom
            * Shows popup where one can choose custom transition time

* Session manager - For replaying the session
    * Local git repo tracking code changes (using libgit2)
    * Sqlite tracking system changes

    * Tracking details
        * Currently active main shader
        * Slider changes for each shader
        * Channel changes for each shader
        * Transitions between shaders
        * Hardware input (audio, webcam, etc)

    * `sessions/` - Directory of sessions
        * `unixtime_sec`_`day`-`month`-`year`/

            * `.git` - Git repo

            * `session.json` - Metadata about the session

            * `session.glsl` - Shader code edited by user

            * `shaders/` - directory of shaders created this session
                * `unixtime_sec`.glsl
                * `unixtime_sec`.glsl
                * [...]
                * `unixtime_sec`.glsl

            * `audio/` - directory of audio clips recorded this session
                * `unixtime_sec`.wav
                * `unixtime_sec`.wav
                * [...]
                * `unixtime_sec`.wav

            * `video/` - video clips recorded this session
                * `unixtime_sec`.mp4
                * `unixtime_sec`.mp4
                * [...]
                * `unixtime_sec`.mp4

    * `session.json` - Metadata about the current session
        ~~~~
        {
            "group": "Mary & the merry ferry men",
            "event": "Døgnfluer",
            "location": "Ingensteds",
            "time": unixtime_sec,
            "media": [
                "https://vimeo.com/234113971"
            ],
            "credits": {
                "Thor Merlin Lervik": [
                    "visuals",
                ],

                "Signe Krunderup Emmeluth": [
                    "sax",
                    "lead vocals",
                    "keyboard"
                ],

                "Jon Fosmark": [
                    "drums",
                    "congas",
                    "synth",
                    "typewriter",
                    "backing vocals"
                ],

                "Elias Tafjord": [
                    "drums",
                    "kalimba",
                    "backing vocals"
                ]
            }
        }
        ~~~~

* Inline error display through sublime text using its phantom API
    * https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom

* Animation curves can be applied to MIDI controller sliders
    * Right click on slider -> context menu with different animation curves

* Choose source for spectrum analyser visualization
    * Right click on it and choose source from context menu
