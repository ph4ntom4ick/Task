CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LIBRARY_SRC = filesearchlib.cpp
LIBRARY_OBJ = $(LIBRARY_SRC:.cpp=.o)
LIBRARY_NAME = libfilesearchlib.a
APP_SRC = main.cpp
APP_OBJ = $(APP_SRC:.cpp=.o)
APP_NAME = filesearch_app

.PHONY: all clean

all: $(LIBRARY_NAME) $(APP_NAME)

$(LIBRARY_NAME): $(LIBRARY_OBJ)
	ar rcs $(LIBRARY_NAME) $(LIBRARY_OBJ)

$(APP_NAME): $(APP_OBJ) $(LIBRARY_NAME)
	$(CC) $(CFLAGS) -o $(APP_NAME) $(APP_OBJ) -L. -lfilesearchlib

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIBRARY_OBJ) $(LIBRARY_NAME) $(APP_OBJ) $(APP_NAME)
