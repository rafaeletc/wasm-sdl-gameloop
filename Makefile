build-wasm:
	emcc src/*.c \
	-s WASM=1 \
	-s USE_SDL=2 \
	-o index.js

clean-wasm:
	rm -vf index.{js,wasm}

run-wasm:
	emrun index.html
