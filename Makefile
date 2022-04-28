CXX = c++
LD = c++
CXXFLAGS = -std=c++17 -O3 -msse4.1 -g3 -Wall
LDFLAGS = -lglfw -lGLEW -ldl -lm -lpthread

OBJDIR := objs
DEPDIR := deps

BIN := 3dsurface
SRCS := Camera.cpp FragmentShader.cpp gl3w.c Grid.cpp main.cpp Mat.cpp Scene.cpp Shader.cpp TripleBuffer.cpp UI.cpp Values.cpp Vec.cpp VertexShader.cpp
OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))
DEPS := $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)
$(shell mkdir -p $(dir $(DEPS)) >/dev/null)

DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
LINK.o = $(LD) -o $@ $^ $(LDFLAGS)

all: $(BIN)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(BIN)

$(BIN): $(OBJS)
	$(LINK.o)

$(OBJDIR)/%.o: %.c
$(OBJDIR)/%.o: %.c | $(DEPDIR)/%.d
	$(COMPILE.cpp) $<

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp | $(DEPDIR)/%.d
	$(COMPILE.cpp) $<

.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
