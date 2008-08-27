main: camarea frame framegrabber utils
	g++ options_window.cpp camarea.o frame.o framegrabber.o utils.o -o nerdtag `pkg-config gtkmm-2.4 --cflags --libs`
frame:
	g++ frame.cpp -o frame.o -c `pkg-config gtkmm-2.4 --cflags` 
framegrabber:
	g++ framegrabber.cpp -o framegrabber.o -c `pkg-config gtkmm-2.4 --cflags`
utils:
	g++ utils.cpp -o utils.o -c `pkg-config gtkmm-2.4 --cflags`
clean:
	rm *.o test
projection: frame framegrabber utils
	g++ projection.cpp -o projection.o -c `pkg-config gtkmm-2.4 --cflags`
camarea: frame framegrabber utils
	g++ camarea.cpp -o camarea.o -c `pkg-config gtkmm-2.4 --cflags`