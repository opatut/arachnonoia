default: compile

PROC=$(nproc)

compile:
	mkdir -p build
	cd build && cmake .. && make -j${NPROC}

run:
	bin/ararchnonoia

clean:
	rm -r build bin
