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

    * Shader context menu (right click on shader)
        * Make current

    * Create transition (left click transition menu)
        * Create shader with transition template
        * The two first channels are populated with placeholder images
        * Placeholder images are white and black
        * Current transition is selected and rendered to preview window

    * Select transition (left click on transition)
        * Render transition with placeholder images to preview window

    * Transition context menu
        * Transition  1 Sec
        * Transition  5 Sec
        * Transition 10 Sec
        * Transition 20 Sec
        * Transition 30 Sec
        * Transition custom
            * Shows popup where one can choose custom transition time

* Session manager with visuals history so that the whole concert can be replayed
    * Everytime a shader is saved, it is committed to the git repo
        * git timestamp precision is 1 second, which is precise enough
          for live replayability
        * libgit2 provides an easy code-oriented way to interact with git

    * Replay of session can be exported as a executable demo

    * Every sound from the configured microphones are recorded and added to the git repo

    * Folder structure
        * sessions
            * `unixtime_sec`_`day`-`month`-`year`
                * .git (git repo)
                * session.json (metadata about the session)
                * controller.json (log of controller input changes)
                * session.glsl (The file edited by user)
                * shaders (directory of shaders created this session)
                    * `unixtime_sec`.glsl
                    * `unixtime_sec`.glsl
                    * [...]
                    * `unixtime_sec`.glsl
                * audio (directory of audio clips recorded this session)
                    * `unixtime_sec`.wav
                    * `unixtime_sec`.wav
                    * [...]
                    * `unixtime_sec`.wav

    * Everything is saved and committed each time shader is recompiled
        * Audio clips saved to the audio directory

    * session.json
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

    * controller.json (saved on every change)
        ~~~~
        {
            "unixtime_msec": [
                {
                    "id": id,
                    "value": value
                }
            ]
        }
        ~~~~


* Inline error display through sublime text using its phantom API
    * https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom

* Animation curves can be applied to MIDI controller sliders
    * Right click on slider -> context menu with different animation curves

* Choose source for spectrum analyser visualization
    * Right click on it and choose source from context menu
