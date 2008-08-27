main: camarea frame framegrabber utils options_window
	g++ blobber.cpp options_window.o camarea.o frame.o framegrabber.o utils.o -o blobber `pkg-config gtkmm-2.4 --cflags --libs`
frame:
	g++ frame.cpp -o frame.o -c `pkg-config gtkmm-2.4 --cflags` 
framegrabber:
	g++ framegrabber.cpp -o framegrabber.o -c `pkg-config gtkmm-2.4 --cflags`
utils:
	g++ utils.cpp -o utils.o -c `pkg-config gtkmm-2.4 --cflags`
projection:
	g++ projection.cpp -o projection.o -c `pkg-config gtkmm-2.4 --cflags`
camarea:
	g++ camarea.cpp -o camarea.o -c `pkg-config gtkmm-2.4 --cflags`
options_window:
	g++ options_window.cpp -o options_window.o -c `pkg-config gtkmm-2.4 --cflags`
clean:
	rm *.o test