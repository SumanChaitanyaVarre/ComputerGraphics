OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include
BIN = bin
BIN1=bin/fmx-animate
SRCS1=src/*.cpp
INCLUDES=src/*.hpp

all: $(BIN1)

$(shell   mkdir -p $(BIN))

$(BIN1): $(SRCS1) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1)
	rmdir $(BIN)
