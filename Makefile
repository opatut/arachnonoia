default: compile run


compile:
	mkdir -p build
	cd build && cmake .. && bash -c "make -j$(shell nproc)"

run:
	bin/arachnonoia

clean:
	rm -r build bin
