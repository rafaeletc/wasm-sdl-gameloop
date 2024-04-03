build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -I/usr/include/SDL2 -D_REENTRANT -lm -o game

run:
	./game

clean:
	rm -vf game

build-wasm:
	emcc src/*.c \
	-s WASM=1 \
	-s USE_SDL=2 \
	-o docs/index.js

clean-wasm:
	rm -vf docs/index.{js,wasm}

run-wasm:
	emrun docs/index.html
