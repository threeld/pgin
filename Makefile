.PHONY: all build run clean

all: build

build:
	cd backend && meson setup build
	ninja -C backend/build

clean:
	rm -rf backend/build backend/.cache/

run: clean build
	./backend/build/pgin

ui:
	cd client && go run cmd/chat/main.go
