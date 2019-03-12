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

        `audio/` - directory of audio clips recorded this session
            `unixtime_sec`.wav
            `unixtime_sec`.wav
            [...]
            `unixtime_sec`.wav


LIVE -- Metadata about the current session - `session.json`

    {
        "mode": "live",
        "group": "Han Gaiden",
        "event": "Serendipfestivalen 2019",
        "location": "Kulturhuset",
        "time": unixtime_sec,
        "media": [
            "https://vimeo.com/318410360"
        ],
        "credits": {
            "Thor Merlin Lervik": [
                "visuals",
            ],
            "Ragnhild Moan": [
                "vocals",
                "synth"
            ],
            "Kristin Myhrvold": [
                "vocals",
                "synth"
            ],
            "Ingrid Skåland Lia": [
                "vocals",
                "synth"
            ]
        },
        "shaders": [
            {
                "id": 1588299344,
                "channels": [
                    {
                        "type": "audio",
                        "device": "alsa_output_16pcm"
                    },
                    {
                        "type": "texture",
                        "filename": "grass.png"
                    },
                    {
                        "type": "shader",
                        "id": 1588299344
                    }
                ]
            },
            {
                "id": 6662323623,
                "channels": [
                    {
                        "type": shader",
                        "id": 6662323623
                    },
                    {
                        "type": shader",
                        "id": 1588299344
                    }
                ]
            }
        ],
        "windows": [
            {
                "type": "master",
                "x": 1920,
                "y": 0,
                "width": 1280,
                "height": 720,
                "model": "dinzoil_F"
            },
            {
                "type": "slave",
                "x": 3200,
                "y": 0,
                "width": 1280,
                "height": 720,
                "model": "dinzoil_B"
            },
            {
                "type": "preview",
                "x": 0,
                "y": 400,
                "width": 256,
                "height": 256
            }
        ]
    }

STUDIO -- Metadata about the current session - `session.json`

    {
        "mode": "studio",
        "name": "BlackPurpl€$BlackPurpl£ 2"
        "group": "rohtie",
        "event": "Revision 2019",
        "time": unixtime_sec,
        "credits": {
            "rohtie": [
                "code",
                "graphics"
            ],

            "Dawg Phaze": [
                "music"
            ]
        },
        "shaders": [
            {
                "id": 1588299344,
                "channels": [
                    {
                        "type": "audio",
                        "device": "alsa_output_16pcm"
                    },
                    {
                        "type": "texture",
                        "filename": "grass.png"
                    },
                    {
                        "type": "shader",
                        "id": 1588299344
                    }
                ]
            },
            {
                "id": 6662323623,
                "channels": [
                    {
                        "type": shader",
                        "id": 6662323623
                    },
                    {
                        "type": shader",
                        "id": 1588299344
                    }
                ]
            }
        ],
        "scenes": [
            {
                "name": "Intro",
                "length": 24000,
                "shader": 6662323623
            },
            {
                "name": "Cream scene",
                "length": 13000,
                "shader": 1588299344
            },
            {
                "name": "Outro",
                "length": 16500,
                "shader": 6662323623
            }
        ],
        "windows": [
            {
                "type": "master",
                "x": 0,
                "y": 400,
                "width": 256,
                "height": 256
            },
            {
                "type": "preview",
                "x": 0,
                "y": 400,
                "width": 256,
                "height": 256
            }
        ]
    }
