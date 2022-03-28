all:
	c++ -fsanitize=address -std=c++17 -O0 -g3 -Wall gl3w.c main.cpp ConfBuffer.cpp TripleBuffer.cpp VertexShader.cpp FragmentShader.cpp Shader.cpp Grid.cpp Scene.cpp UI.cpp -lglfw -lGLEW -ldl -lpthread -o 3dsurface
