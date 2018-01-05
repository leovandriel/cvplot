# Proxy to CMake

.PHONY: all clean demo test format

all:
	@mkdir -p build && cd build && cmake .. && make

debug:
	@rm -rf build && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

clean:
	@rm -rf build bin

demo:
	@mkdir -p build && cd build && cmake -DCVPLOT_DEMO=1 .. >/dev/null && make >/dev/null
	@./bin/demo

test:
	@mkdir -p build && cd build && cmake -DCVPLOT_TEST=1 .. >/dev/null && make >/dev/null
	@find bin -regex "bin/[a-z_]*_test" -type f -exec ./{} \;

format:
	@find . -iname "*.h" -o -iname "*.cc" | xargs clang-format --style=Google -i
