default: compile run


compile:
	mkdir -p build
	cd build && cmake .. && make -j2

run:
	bin/arachnonoia

clean:
	rm -r build bin
