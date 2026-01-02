cc := clang++
cflags := -std=c++17 -Wall -Wextra -pedantic -MMD -MP -g3
incdir := -Iinclude
objdir := build/objs/src
test_objdir := build/objs/tests

srcs := $(wildcard src/*.cpp)
objs := $(srcs:src/%.cpp=$(objdir)/%.o)
deps := $(objs:.o=.d)
-include $(deps)

test_srcs := $(wildcard tests/*.cpp)
test_objs := $(test_srcs:tests/%.cpp=$(test_objdir)/%.o)
test_deps := $(test_objs:.o=.d)
test_exec := build/test
-include $(test_deps)

.PHONY: all test clean

all: $(test_exec)

test: $(test_exec)
	./$(test_exec)

$(test_exec): $(objs) $(test_objs)
	$(cc) $^ -o $@

$(objdir)/%.o: src/%.cpp | $(objdir)
	$(cc) $(cflags) $(incdir) -c $< -o $@

$(test_objdir)/%.o: tests/%.cpp | $(test_objdir)
	$(cc) $(cflags) $(incdir) -c $< -o $@

$(objdir) $(test_objdir):
	mkdir -p $@

clean:
	rm -rf $(test_exec) build/objs
