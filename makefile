CXX = g++
TARGET = Program
CXXFLAGS = -Wall

ifeq ($(OS),Windows_NT)
	LIBS = -lglfw3 -lglew32 -lgdi32 -lopengl32
else
	UNAME = $(shell uname)
	ifeq ($(UNAME),Linux)
		LIBS = -lglfw -lGLEW -lGL
	endif
	ifeq ($(UNAME),Darwin)
		LIBS = -lglfw -lglew -framework OpenGL
	endif
endif

SRCS = Program.cpp CubeInsWindow.cpp CubeWindow.cpp FpsCounter.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)
