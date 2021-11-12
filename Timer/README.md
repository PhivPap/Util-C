# Timer
Implementation of a stopwatch ADT, used for measuring execution time of various parts of a program.

Checked malloc(...)

    StopWatch_new():
        Returns a pointer to StopWatch object or NULL if malloc(...) fails.
        The StopWatch is running when returned.
        
    StopWatch_elapsed_sec(...):
        Returns the time elapsed (in seconds) since the last StopWatch_reset(...) call or the constructor call.
        Excludes time periods where the stopwatch was stopped.
        
    StopWatch_reset(...):
        Resets the stopwatch. Does not affect the "running" state of the stopwatch.
        
    StopWatch_pause(...):
        Pauses the stopwatch. Pausing already paused stopwatch has no effect.
        
    StopWatch_resume(...):
        Resumes the stopwatch. Resuming already running stopwatch has no effect.
        
    StopWatch_is_running(...):
        Returns 1 if the stopwatch is running.
        
    StopWatch_diff(...):
        TODO
