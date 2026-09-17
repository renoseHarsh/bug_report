# Bug report: non-deterministic clangd crash after `glaze::format_error`

Minimal repro of a non-deterministic **clangd crash** that happens while typing
in the editor after a call to `glaze::format_error(...)` in `main.cpp`.

Typing code after the `format_error` call (e.g. `std::string hi = "random";`)
crashes clangd most of the time, but not always — it is not deterministic.

## Environment

- macOS (arm64, Homebrew LLVM 23)
- Compiler: `/opt/homebrew/opt/llvm/bin/clang++` (C++23)
- Build: CMake (`-G Ninja`) + `import std` + LLVM libc++ modules
- clangd (via `compile_commands.json`)
- Libraries: [glaze](https://github.com/stephenberry/glaze) (fetched at configure time)

## Project layout

- `glaze_wrapper.cppm` — C++23 module wrapping glaze and exposing `glaze::read_json` / `glaze::format_error`
- `main.cpp` — parses JSON, then calls `glaze::format_error`

## Build

```sh
cmake -B build -G Ninja
cmake --build build
ln -s build/compile_commands.json compile_commands.json
```

The symlink is required so clangd picks up the compile database.

## Non-macOS / non-Homebrew setup

`CMakeLists.txt` hardcodes a Homebrew LLVM toolchain for mac. On Linux or a
different LLVM install, remove/change these four lines:

```cmake
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
set(CMAKE_AR /opt/homebrew/opt/llvm/bin/llvm-ar)
set(CMAKE_RANLIB /opt/homebrew/opt/llvm/bin/llvm-ranlib)
set(CMAKE_CXX_STDLIB_MODULES_JSON
    /opt/homebrew/Cellar/llvm/23.1.0/lib/c++/libc++.modules.json)
```

After removing them, CMake falls back to the system default C++ compiler and
archiver — but the project requires Clang with C++23 module support
(`import std`, `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD`), plus a supported LLVM
version where `libc++.modules.json` ships. If `CMAKE_CXX_STDLIB_MODULES_JSON`
is removed, you must either point it at your LLVM's `libc++.modules.json` or
set it via the CMake cache, e.g.:

```sh
cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=/path/to/clang++ \
  -DCMAKE_CXX_STDLIB_MODULES_JSON=/path/to/llvm/lib/c++/libc++.modules.json
```

Then build:

```sh
cmake --build build
ln -s build/compile_commands.json compile_commands.json
```

## Reproduce

1. Configure + build as above.
2. Open `main.cpp` in an editor using clangd.
3. Place the cursor on a new line after `glaze::format_error(error, buffer);`
   and start typing, e.g. `std::string hi = "random";`
4. clangd crashes; the crash reproduces most of the time but intermittently
   the keystrokes are accepted first.

## Minimal repro snippet

```cpp
import std;
import glaze_wrapper;

int main() {
  type::Company company{};

  std::string json = R"({
        "name": "Drum",
        "address": "Mumbai"
    })";

  auto error = glaze::read_json(company, json);
  std::string buffer;
  std::string err = glaze::format_error(error, buffer);
  // start typing here -> clangd crashes

  return 0;
}
```

## Notes

- CMake toolchain paths are hardcoded in `CMakeLists.txt` and must match your
  local LLVM installation.
- `build/` and `.cache/` are gitignored.