
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

* Inline error display through sublime text using its phantom API
	* https://www.sublimetext.com/docs/3/api_reference.html#sublime.Phantom
