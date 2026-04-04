TARGET = database
CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -g -I.

SRCS = main.cpp database.cpp list.cpp menu.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
