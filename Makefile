C = g++
PROG = game

SRCS = code.cpp imageloader.cpp

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
	else
	LIBS = -lglut -lGLU -lGL
	endif

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
