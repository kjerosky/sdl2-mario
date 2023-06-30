EXECUTABLE = mario

CPP_FILES = \
	main.cpp \
	Level.cpp

INCLUDE_PATHS = \
	-I/Library/Frameworks/SDL2.framework/Headers \
	-I/Library/Frameworks/SDL2_image.framework/Headers

LIBRARY_PATHS = \
	-F/Library/Frameworks

LIBRARIES = \
	-framework SDL2 \
	-framework SDL2_image

all:
	g++ ${CPP_FILES} -o ${EXECUTABLE} ${INCLUDE_PATHS} ${LIBRARY_PATHS} ${LIBRARIES}

clean:
	rm -rf ${EXECUTABLE}
