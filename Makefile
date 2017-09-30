LDFLAGS=-shared -fPIC
all:server time.so version.so disk.so process.so
obj=module.o response.o 

response.o:response.h
module.o:module.h 

server:server.o processpool.o $(obj)
	g++ -rdynamic -o server $(obj) server.o processpool.o -ldl
processpool.o:processpool.h process.h
time.so:
	g++ time.cpp chartohtml.h $(LDFLAGS) -o time.so
version.so:
	g++ version.cpp chartohtml.h $(LDFLAGS) -o version.so
disk.so:
	g++ disk.cpp chartohtml.h $(LDFLAGS) -o disk.so
process.so:
	g++ process.cpp chartohtml.h $(LDFLAGS) -o process.so
clean:
	rm *.o *.so
