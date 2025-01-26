CXX = g++
CXXFLAGS = -std=c++17 -I./include -I/usr/include/nlohmann
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lyaml-cpp -lexiv2 -lcurl

TARGET = unscreensaver
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)
	
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
