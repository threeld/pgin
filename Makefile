.PHONY: all build run clean

all: build

build:
	cd backend && meson setup build
	ninja -C backend/build

run: build
	./backend/build/pgin

clean:
	rm -rf backend/build backend/.cache/
