### Intel HD4400 - bug

Trig functions (sin, cos, tan) lose precision when input is greater than 12865.

Can be prevented by modulating the input time.


### Logging slider changes to json seems to add about 2.5 ms / frame

Consider using something like [spdlog](https://github.com/gabime/spdlog) instead.

Switch to sqlite for better record-structure and performance


### Projection mapping

We should render the shader once to an offscreen fbo
where the size is a power of 2 to get better performance


### Sublimetext inline error display

Might be viable using the phantom API:

https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom


### Session manager - For replaying the session

* Local git repo tracking code changes (using libgit2)
* Sqlite tracking system changes

Tracking details

* Currently active main shader
* Slider changes for each shader
* Channel changes for each shader
* Transitions between shaders
* Hardware input (audio, webcam, etc)


Directory of sessions - `sessions/`

    `unixtime_sec`_`day`-`month`-`year`/

        `.git` - Git repo

        `session.json` - Metadata about the session

        `session.glsl` - Shader code edited by user

        `shaders/` - directory of shaders created this session
            `unixtime_sec`.glsl
            `unixtime_sec`.glsl
            [...]
            `unixtime_sec`.glsl

            `thumbnails/` - directory of thumbnails for the shaders
                `unixtime_sec`.png
                `unixtime_sec`.png
                [...]
                `unixtime_sec`.png

        `transitions/` - directory of transitions created this session
            `unixtime_sec`.glsl
            `unixtime_sec`.glsl
            [...]
            `unixtime_sec`.glsl

            `thumbnails/` - directory of thumbnails for the shaders
                `unixtime_sec`.png
                `unixtime_sec`.png
                [...]
                `unixtime_sec`.png

        `audio/` - directory of audio clips recorded this session
            `unixtime_sec`.wav
            `unixtime_sec`.wav
            [...]
            `unixtime_sec`.wav

        `video/` - video clips recorded this session
            `unixtime_sec`.mp4
            `unixtime_sec`.mp4
            [...]
            `unixtime_sec`.mp4


Metadata about the current session - `session.json`

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
