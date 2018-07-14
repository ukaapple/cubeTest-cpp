CXX = g++
TARGET = Program
CXXFLAGS = -Wall
LIBS = -lglfw -lglew -framework OpenGL
SRCS = Program.cpp CubeInsWindow.cpp CubeWindow.cpp FpsCounter.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)
