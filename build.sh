
mkdir -p build-unix
set -e
cd build-unix

cmake .. -G "Ninja"
cmake --build .
if ! pgrep -f "chrome-remote-desktop" ;then
	./src/MyApplication
else
	vglrun ./src/MyApplication
fi
