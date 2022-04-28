CXX = c++
CXXFLAGS = -fsanitize=address -std=c++17 -O3 -msse4.1 -g3 -Wall
LDFLAGS = -fsanitize=address -lglfw -lGLEW -ldl -lpthread

3dsurface: gl3w.o main.o TripleBuffer.o VertexShader.o FragmentShader.o Shader.o Grid.o Scene.o Camera.o UI.o Vec.o Mat.o Values.o
	$(CXX) -o 3dsurface gl3w.o main.o TripleBuffer.o VertexShader.o FragmentShader.o Shader.o Grid.o Scene.o Camera.o UI.o Vec.o Mat.o Values.o $(LDFLAGS)

gl3w.o: gl3w.c
	$(CXX) -c $(CXXFLAGS) gl3w.c

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp

TripleBuffer.o: TripleBuffer.cpp TripleBuffer.h
	$(CXX) -c $(CXXFLAGS) TripleBuffer.cpp

VertexShader.o: VertexShader.cpp VertexShader.h
	$(CXX) -c $(CXXFLAGS) VertexShader.cpp

FragmentShader.o: FragmentShader.cpp FragmentShader.h
	$(CXX) -c $(CXXFLAGS) FragmentShader.cpp

Shader.o: Shader.cpp Shader.h
	$(CXX) -c $(CXXFLAGS) Shader.cpp

Grid.o: Grid.cpp Grid.h
	$(CXX) -c $(CXXFLAGS) Grid.cpp

Scene.o: Scene.cpp Scene.h
	$(CXX) -c $(CXXFLAGS) Scene.cpp

Camera.o: Camera.cpp Camera.h
	$(CXX) -c $(CXXFLAGS) Camera.cpp

UI.o: UI.cpp UI.h
	$(CXX) -c $(CXXFLAGS) UI.cpp

Vec.o: Vec.cpp Vec.h
	$(CXX) -c $(CXXFLAGS) Vec.cpp

Mat.o: Mat.cpp Mat.h
	$(CXX) -c $(CXXFLAGS) Mat.cpp

Values.o: Values.cpp Values.h
	$(CXX) -c $(CXXFLAGS) Values.cpp

clean:
	rm -f *.o 3dsurface
