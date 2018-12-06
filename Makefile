
CC:=clang-6.0
CPPFLAGS:=-std=c++17 -Wall -Wextra -Werror -pedantic -fsanitize=address -O0 -g -I$(CURDIR)

SRCS:=$(shell find tests -type f -name '*.cxx') test-main.cpp

OBJDIR:=build
OBJS:=$(patsubst %.cxx,${OBJDIR}/%.o,${SRCS})

.PHONY: clean

test: | build/test
	build/test

build/test: ${OBJS}
	@mkdir -p "$$(dirname "$@")"
	$(CC) $(CPPFLAGS) $(OBJS) -lstdc++ -o $@

$(OBJDIR)/%.o: %.cxx
	@mkdir -p "$$(dirname "$@")"
	$(CC) -x c++ $(CPPFLAGS) -fsanitize=address -o $@ -c $<

clean:
	rm -rf build


