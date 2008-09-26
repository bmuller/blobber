pong:	pong.o ball.o
	g++ -shared -Wl,-soname,libpong.so.0 -o libpong.so.0.0.0 pong.o ball.o `pkg-config libblobber --libs`	

pong.o:	pong.cpp pong.h
	g++ -Wall -c pong.cpp `pkg-config libblobber --cflags`

ball.o:	ball.h ball.cpp
	g++ -Wall -c ball.cpp `pkg-config libblobber --cflags`

clean:
	rm *.o
	rm libpong.so.0.0.0
	rm *~

	
