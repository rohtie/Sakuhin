
* Animation curves can be applied to MIDI controller sliders
    * Right click on slider -> context menu with different animation curves

* Choose source for spectrum analyser visualization
    * Right click on it and choose source from context menu

* Shader manager
    * Selecting a shader
        * Trivial: Main shader is switched

        * Advanced: Shader is switched for preview only
            * To switch main shader, one must transition to the shader

    * Creating a new shader
        * Save current shader to the shader directory

        * Trivial: Switch shader immediately
            * Load selected shader template into session.glsl

        * Advanced
            * Open new shader in preview only

* Transitions between shaders
    * Transition types
        * Select from a selection of pre-written transitions
        * Write a custom transition live

    * Trivial implementation
        * Render both shaders and input them to a third shader

    * Efficient implementation
        * Combine the code of both shaders into one shader and use their main functions to only render parts of each shader as needed

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
