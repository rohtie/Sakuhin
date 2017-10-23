
* Animation curves can be applied to MIDI controller sliders
    * Right click on slider -> context menu with different animation curves

* Choose source for spectrum analyser visualization
    * Right click on it and choose source from context menu

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
            "unixtime_msec": {
                "slider0": 0.50,
                "slider1": 0.39,
                "slider2": 0.25,
                "slider3": 0.99
            }
        }
        ~~~~


* Inline error display through sublime text using its phantom API
    * https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom
