sXe: simple heX editor
======================

[![Build Status](https://travis-ci.com/Jorengarenar/sXe.svg?branch=master)](https://travis-ci.com/Jorengarenar/sXe)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/16282bcfff234db6b499436128e32684)](https://www.codacy.com/gh/Jorengarenar/sXe/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Jorengarenar/sXe&amp;utm_campaign=Badge_Grade)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Jorengarenar/sXe.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Jorengarenar/sXe/context:cpp)
[![CodeFactor](https://www.codefactor.io/repository/github/jorengarenar/sXe/badge)](https://www.codefactor.io/repository/github/jorengarenar/sXe)

# Dependencies

* [ncurses](https://invisible-island.net/ncurses)
* [libXDGdirs](https://github.com/Jorengarenar/libXDGdirs)
* [Lua](https://www.lua.org)
* [Boost](https://www.boost.org) (for tests)

# Obtaining

## Download

Clone Git repository for latest:
```sh
git clone https://github.com/Jorengarenar/sXe.git && cd sXe
```

## Build
```sh
cmake -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/
```

## Install
```sh
cmake --install build/
```
