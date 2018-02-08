windows:
	# TODO: Add Windows compiling stuff here..
linux:
	g++ *.cpp -o BtcTickerLinux -D BUILD_LINUX -lcurl -std=c++11
linux-clean:
	rm Source.o
