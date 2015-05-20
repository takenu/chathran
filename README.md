# Chathran

A project to develop a free, open source 3D role-playing game in C++, licensed under the GPL.

For the project description and goal, please see [the wiki](https://github.com/takenu/chathran/wiki/).

## Requirements

Chathran uses the tiny-game-engine for rendering, and uses Selene for
Lua binding. Chathran will link against these by using FindTinygame.cmake
and FindSelene.cmake. This which works best if you install Chathran next
to tiny-game-engine and Selene. For example, in some directory, run the
following:
* git clone https://github.com/takenu/Selene.git
* git clone https://github.com/takenu/tiny-game-engine.git
* git clone https://github.com/takenu/chathran.git

Compilation of Chathran uses CMake and g++. Note that your g++ compiler
must be C++11 compliant, otherwise compilation of Selene functionality
will not work.

Finally, note that Chathran links against all libraries also linked by
tiny-game-engine. For the full list, see [tiny-game-engine](https://github.com/takenu/tiny-game-engine.git).

## Installation

Open a terminal in the main directory, and type the following:
* mkdir build
* cd build
* cmake ..
* make ( or 'make --jobs=N' where N is the number of processors you wish to dedicate. )

Note that you will need to (re)compile [tiny-game-engine](https://github.com/takenu/tiny-game-engine.git) to allow Chathran
to link against the most recent version of tiny-game-engine. Since
tiny-game-engine is for now developed in parallel with Chathran, new versions of
Chathran will often require new versions of tiny-game-engine. This
dependency is anticipated to decline in the future, and once
tiny-game-engine achieves a mature release cycle Chathran will be based
off specific releases.

#### Known linking issue

Linking of Chathran on Linux may fail with the following error:
* undefined reference to symbol 'dlclose@@GLIBC_2.2.5'
This is a Lua error that can randomly result even if you compile
everything properly. Chathran already compiles with the option '-ldl'
since this *usually* solves the problem, but even then Lua may continue
to have the nasty habit of not linking. You can try compiling Lua using
"make posix test" rather than "make linux test" which is known to solve
this issue in some instances.

## Licensing

Chathran is available under a GNU GPL v3 license (or any later license, at
your option). For the terms of the GPLv3, see LICENSE or http://www.gnu.org/licenses/.

