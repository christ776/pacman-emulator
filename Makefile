stripes:
	gcc src/main.cpp src/parser/parser.cpp src/renderer/opengl_renderer.cpp src/renderer/pixel.cpp -o stripes -g  -lstdc++ -lGL -lGLU -lglut
