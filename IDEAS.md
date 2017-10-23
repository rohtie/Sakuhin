
* Animation curves can be applied to MIDI controller sliders
    * Right click on slider -> context menu with different animation curves

* Choose source for spectrum analyser visualization
    * Right click on it and choose source from context menu

* Session manager with visuals history so that the whole concert can be replayed
    * Everytime a shader is saved, it is committed to the git repo
        * git timestamp precision is 1 second, which is precise enough
          for live replayability
        * libgit2 provides an easy code-oriented way to interact with git

    * Every sound from the configured microphones are recorded and added to the git repo

    * Folder structure
    	sessions
    		unixtime_day-month-year
    			session.json

    * session.json
    	{
    		"group": "Mary & the merry ferry men",
    		"event": "Døgnfluer",
    		"location": "Ingensteds",
    		"time": 1508747965,
    		"media": [
    			"https://vimeo.com/234113971",
    			"https://scontent-arn2-1.xx.fbcdn.net/v/t1.0-9/21728190_1022594971210987_8230294931162665398_n.jpg?oh=e602ba82cf5bdd141d4cf79a1b81bdca&oe=5A6A9DF6"
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

* Inline error display through sublime text using its phantom API
	* https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom
