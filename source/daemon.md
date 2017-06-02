# `ict::daemon` module

Provides a functions for a daemon applications.

## Usage

Following functions are available:
* `daemonize(log=false)` - Converts process into system daemon (optionally syslog is used);
* `savePid(pid,path)` - Saves PID into given file.
