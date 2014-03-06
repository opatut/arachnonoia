default: compile run


compile:
	mkdir -p build
	cd build && \
		cmake .. && \
		make -j$(shell nproc)

run:
	bin/arachnonoia

win64:
	mkdir -p build-win64
	cd build-win64 && \
		x86_64-w64-mingw32-cmake \
			-DCMAKE_CXX_FLAGS="-DSFML_STATIC -DFLT_EPSILON=__FLT_EPSILON__" \
			-DGLM_INCLUDE_DIR=/usr/include/ \
			.. && \
			make -j$(shell nproc) -j1 VERBOSE=1

clean:
	rm -rf build build-win64 bin
