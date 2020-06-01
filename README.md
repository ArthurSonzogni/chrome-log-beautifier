# chrome-log-beautifier
[![travis][badge.travis]][travis]
[![issues][badge.issues]][issues]
[![license][badge.license]][license]
[![contributors][badge.contributors]][contributors]

[badge.travis]: https://img.shields.io/travis/com/arthursonzogni/chrome-log-beautifier
[badge.issues]: https://img.shields.io/github/issues-raw/arthursonzogni/chrome-log-beautifier
[badge.license]: https://img.shields.io/github/license/arthursonzogni/chrome-log-beautifier?color=black
[badge.contributors]: https://img.shields.io/github/contributors/arthursonzogni/chrome-log-beautifier?color=blue

[travis]: https://travis-ci.com/ArthurSonzogni/chrome-log-beautifier
[issues]: https://github.com/ArthurSonzogni/chrome-log-beautifier/issues
[license]: http://opensource.org/licenses/MIT
[contributors]: https://github.com/ArthurSonzogni/chrome-log-beautifier/graphs/contributors

Make Chrome logs fancy.

![logo](./doc/logo.webp)

## Features:
  - Streaming supported. E.g.  ``` chrome 2>&1 | chrome-log-beautifier```
  - Colorization.
  - Filter by:
    - Process & Thread
    - Log level

#### Potential improvements:
  - Save logs as txt / json
  - Support StackTraces. Find a way to smartly attribute the trace to its emitting thread.
  - Find the process type (browser, renderer, GPU). Is this possible?

## Usage
```bash
chrome-log-beautifier logfile
```
Or you can even stream the logs:
```bash
chrome --v=2 2>&1 | chrome-log-beautifier
```

## Installation:
Install build tools:
```
  sudo apt install git cmake build-essential
```

Build & install chrome-log-beautifier
```bash
  git clone https://github.com/ArthurSonzogni/chrome-log-beautifier
  cd chrome-log-beautifier
  mkdir build
  cd build
  cmake ..
  make -j
  sudo make install
```
