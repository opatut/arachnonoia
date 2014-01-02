default: compile run


compile:
	mkdir -p build
	cd build && cmake .. && bash -c "make -j3"

run:
	bin/arachnonoia

clean:
	rm -r build bin
