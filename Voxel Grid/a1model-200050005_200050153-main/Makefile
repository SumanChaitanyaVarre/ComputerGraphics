OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include
BIN = bin
BIN1=bin/a1model
# BIN2=bin/01_triangle_dep
SRCS1=src/*.cpp
# SRCS2=01_triangle_dep.cpp gl_framework.cpp shader_util.cpp 
INCLUDES=src/*.hpp

all: $(BIN1) #$(BIN2)

$(shell   mkdir -p $(BIN))

$(BIN1): $(SRCS1) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

# $(BIN2): $(SRCS2) $(INCLUDES)
# 	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1)
	rmdir $(BIN)
