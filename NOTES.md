
* Intel HD4400
    * Trig functions (sin, cos, tan) lose precision when input is greater than 12865.
        * Prevent this by resetting the input time when it is reaching the number

* Logging slider changes to json seems to add about 2.5 ms / frame
    * Consider using something like [spdlog](https://github.com/gabime/spdlog) instead.
    * Switch to sqlite for better record-structure and performance
