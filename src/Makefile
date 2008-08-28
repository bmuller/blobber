main: camarea frame framegrabber utils options_window projection_window laser_tag
	g++ blobber.cpp mods/laser_tag.o options_window.o camarea.o frame.o projection_window.o framegrabber.o utils.o -o blobber `pkg-config gtkmm-2.4 --cflags --libs`
laser_tag:
	g++ mods/laser_tag.cpp -o mods/laser_tag.o -c `pkg-config gtkmm-2.4 --cflags` 
frame:
	g++ frame.cpp -o frame.o -c `pkg-config gtkmm-2.4 --cflags` 
framegrabber:
	g++ framegrabber.cpp -o framegrabber.o -c `pkg-config gtkmm-2.4 --cflags`
utils:
	g++ utils.cpp -o utils.o -c `pkg-config gtkmm-2.4 --cflags`
camarea:
	g++ camarea.cpp -o camarea.o -c `pkg-config gtkmm-2.4 --cflags`
projection_window:
	g++ projection_window.cpp -o projection_window.o -c `pkg-config gtkmm-2.4 --cflags`
options_window:
	g++ options_window.cpp -o options_window.o -c `pkg-config gtkmm-2.4 --cflags`
clean:
	rm *.o test