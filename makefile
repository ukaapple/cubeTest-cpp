CXX = g++
TARGET = Program
CXXFLAGS = -Wall

UNAME = $(shell uname)
ifeq ($(UNAME),Linux)
    LIBS = -lglfw -lGLEW -lGL
endif
ifeq ($(UNAME),Darwin)
    LIBS = -lglfw -lglew -framework OpenGL
endif

SRCS = Program.cpp CubeInsWindow.cpp CubeWindow.cpp FpsCounter.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)
