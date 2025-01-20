main: main.cpp
	g++ main.cpp -o build/forgetWhat `pkg-config --cflags --libs libnotify`