# wasm-sdl-gameloop [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

A Simple Game Loop Written in C & SDL[^1] with browser execution with WebAssembly, binding it to an HTML5 canvas using Emscripten.

Live demo: https://rafaeletc.github.io/wasm-sdl-gameloop

## Installation:

1. Install Emscripten:<br/>
https://emscripten.org/docs/getting_started/downloads.html

2. Clone the repository:<br/>
`git clone https://github.com/rafaeletc/wasm-sdl-gameloop`<br/>
`cd wasm-sdl-gameloop`

3. Run `make build-wasm` or build manually:<br/>
`emcc src/*.c -s WASM=1 -s USE_SDL=2 -o public/index.js`

4. Browsers doesn't support file XHR requests so you need to open index.html from a web server.<br />
Run `make run-wasm` or manually:<br/>
`emrun public/index.html`

5. 🕹️ Use the keys ⬅️ and ➡️ to move the paddle, key **R** to start again.

6. You can run on terminal too, with compiler and SDL2 dependencies on place, just hit `make build; make run`

## Todos:

- [ ] Improve Makefile
- [ ] Better physics for the game
- [ ] Build a game interface for pontuation
- [ ] Add a game start scene
- [ ] Add a game over scene

---

[^1] Game design taken from [Pikuma's Creating a Game Loop with C & SDL (Tutorial)](https://www.youtube.com/watch?v=XfZ6WrV5Z7Y)
