EXECUTABLE = mario

CPP_FILES = \
	main.cpp \
	Level.cpp \
	GameConfig.cpp \
	Player.cpp \
	GameObjectsManager.cpp \
	Input.cpp \
	Animator.cpp \
	Time.cpp \
	Goomba.cpp \
	Fireball.cpp \
	SpriteSheet.cpp \
	SpriteSheetRepository.cpp

INCLUDE_PATHS = \
	-I/Library/Frameworks/SDL2.framework/Headers \
	-I/Library/Frameworks/SDL2_image.framework/Headers

LIBRARY_PATHS = \
	-F/Library/Frameworks

LIBRARIES = \
	-framework SDL2 \
	-framework SDL2_image

FLAGS = -Wall -std=c++11

all:
	g++ ${CPP_FILES} -o ${EXECUTABLE} ${FLAGS} ${INCLUDE_PATHS} ${LIBRARY_PATHS} ${LIBRARIES}

clean:
	rm -rf ${EXECUTABLE}
