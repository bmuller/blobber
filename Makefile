main:
	g++ test.cpp -shared -o test.so
	g++ runtest.cpp -o runtest `pkg-config gtkmm-2.4 --cflags --libs`
clean:
	rm -f test.so runtest