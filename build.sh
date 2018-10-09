
mkdir -p build-unix
set -e
cd build-unix

if [ "$1" == "ycm" ]; then
	ycm_generator -f -b cmake ..
	exit 0
fi


cmake .. -G "Ninja"
cmake --build .
if ! pgrep -f "chrome-remote-desktop" ;then
	./src/MyApplication
else
	vglrun ./src/MyApplication
fi
