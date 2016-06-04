# http://mrbook.org/blog/tutorials/make/

# Compiler to be used
CC=g++

# Flags passed to compiler
CFLAGS=-c -g -Wall -Wextra -std=c++0x -pedantic

#SFML libraries
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

#Source files
SOURCES=Application.cpp Button.cpp Ship.cpp Command.cpp CommandQueue.cpp \
Component.cpp Container.cpp Entity.cpp GameState.cpp \
Label.cpp Main.cpp MenuState.cpp PauseState.cpp \
Player.cpp SceneNode.cpp SettingsState.cpp \
SpriteNode.cpp State.cpp StateStack.cpp TitleState.cpp \
Utility.cpp World.cpp

#Object files
OBJECTS=$(SOURCES:.cpp=.o)

EXE=asteroidit

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS) ; $(CC) $(OBJECTS) -o $@ $(LIBS)
    
.cpp.o: ; $(CC) $(CFLAGS) $< -o $@   
    
clean: ; rm -f $(OBJECTS)
