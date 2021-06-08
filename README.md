# sXe: simple heX editor

[![Build Status](https://travis-ci.com/Jorengarenar/sXe.svg?branch=master)](https://travis-ci.com/Jorengarenar/sXe)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/16282bcfff234db6b499436128e32684)](https://www.codacy.com/gh/Jorengarenar/sXe/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Jorengarenar/sXe&amp;utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/jorengarenar/sXe/badge)](https://www.codefactor.io/repository/github/jorengarenar/sXe)

## Dependencies

  * [ncurses](https://invisible-island.net/ncurses)
  * [libXDGdirs](https://github.com/Jorengarenar/libXDGdirs)
  * [Lua](https://www.lua.org)
  * [Boost](https://www.boost.org) (for unit tests)

## Install

```sh
git clone https://github.com/Jorengarenar/sXe.git && cd sXe
cmake -B build/ -DCMAKE_BUILD_TYPE=Release && cmake --build build/
cmake --install build/
```
