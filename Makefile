PRE ?= 01
TARGET := $(basename $(notdir $(wildcard rex_proj/$(PRE)*.cpp)))
#TARGET := $(basename $(notdir $(wildcard examples/$(PRE)*.cpp)))


$(PRE):
	g++ `pkg-config --cflags --libs sdl2` -o $(TARGET).exe -g rex_proj/$(TARGET).cpp
	./$(TARGET).exe

#$(PRE):
#	x86_64-w64-mingw32-g++ -o $(TARGET).exe examples/$(TARGET).cpp -mwindows -static
#	x86_64-w64-mingw32-g++ -o $(TARGET).exe -g rex_proj/$(TARGET).cpp -mwindows -static
