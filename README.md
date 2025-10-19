# pgin - a Chat App (C) – WIP

A work-in-progress **terminal-based chat application** in C. The server skeleton is complete, and the plan is to build a terminal-based UI (TUI) client in **Go** using [Bubble Tea](https://github.com/charmbracelet/bubbletea).

## Features (Planned)

- Multi-client chat over TCP
- Terminal-based UI (TUI) in Go (Bubble Tea)
- Graceful shutdown on Ctrl+C for server
- Message broadcasting between clients
- Optional username support

## Current Status

- [x] TCP server skeleton in C
- [x] Graceful shutdown with Ctrl+C
- [ ] Multi-client support
- [ ] Client application (Bubble Tea TUI)
- [ ] Chat protocol and message broadcasting
- [ ] TUI client interface

## Prerequisites

- GCC or Clang
- **Meson** and **Ninja** installed
- Linux/macOS (POSIX-compliant systems recommended)
- Go (for TUI client with Bubble Tea)

## Build & Run

After cloning the repository:

```bash
git clone https://github.com/shandilyaaryan/pgin.git
cd pgin
```

### Build

```bash
make
```

This will:

1. Set up the Meson build in the `backend` folder.
2. Compile the project using Ninja.

### Run

```bash
make run
```

This will:

1. Clean previous builds.
2. Build the project again.
3. Run the executable (`pgin`) located in `backend/build`:

### Clean

```bash
make clean
```

Removes the build directory and Meson cache for a fresh build.

---

## Future Roadmap

1. Implement **multi-client handling** using `select()` or threads
2. Add **message broadcasting**
3. Create a **TUI client** in Go using Bubble Tea
4. Implement **usernames** and **chat protocol**

---

## Notes

- This project is currently a **work-in-progress**.
- Contributions are welcome!
