# wfs - Web From Scratch

One of the many reiterations of my stupid programming project;
the current one, actually (03.01.24)


Features
--------
- Garbage-collected DOM trees
- Full HTML5 syntax support (in the *re*works, LOL!)


Plans (feasible)
--------------------
- HTTP support
- DOM event support
- JavaScript support (most likely a QuickJS fork)


Long-term goals
---------------
- Serve as a WebKit-like library for homebrewn browsers
- Run on modest hardware
- Be easily modifiable

Non-goals
---------
- Replace Firefox/Chromium/WebKit/etc.
- Support for crufty, obsolete features nobody cares about


Build Instructions
------------------

GCC *should* be used, as the code makes use of 'unfinished' C2X features
(mainly typed enumerations).

Apart from this, it should compile as standard C99.

```$ make```
... with your crazy multi-core flags.
