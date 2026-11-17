# grimace

A CMake toolchain for Windows CE programs and DLLs. It uses clang and lld, then `tools/mkpe.py` turns the linked ELF into a CE PE.

## Targets

| Target | `GRIMACE_ARCH` | `GRIMACE_CE_VERSION` | `GRIMACE_PLATFORM` |
| --- | --- | --- | --- |
| MIPS CE 1.0, 2.0: Philips Velo, NEC MobilePro, Sharp Mobilon and other Handheld PCs | `mips` | `1`, `2` | `hpc` |
| SH3 CE 1.0, 1.01, 2.0: HP 300LX to 620LX, Casio Cassiopeia and other Handheld PCs | `sh3` | `1`, `1.01`, `2` | `hpc` |
| x86 CE 2.x: Clarion Joyride | `x86` | `2` | `autopc` |
| ARM CE 3.0: Handheld PC 2000 (HP Jornada 720), Pocket PC 2000 and 2002 (iPAQ) and later | `arm` | `3` | `hpc2000`, `pocketpc`, `embedded` |
| ARM CE 5.2: Windows Mobile 6 to 6.5 (Trimble Juno SB) | `arm` | `5.2` | `pocketpc`, `smartphone` |
| ARM CE 6.0: Sharp Brain, Windows Embedded CE 6 | `arm` | `6` | `brain`, `embedded` |

- `GRIMACE_DEVICE` pins a target to one device. `python3 tools/manifests.py` lists them: `velo1`, `jornada720`, `junosb`, the Brains' `pwsh2`, `pwsb1`, `pwsh4`, `pwa7300` and `pwg5300`, and the devices imported from CEM Porter, named as in cerf-bundles (`ipaq_h3600_ppc2002`, `hp_620lx_ce2`).
- Other platforms are `hpcpro` and `palmsize`.
- grimace doesn't build for CE 2.01 to 2.12, 4.x, 5.0 or 5.1 yet. Their lists count towards an earlier version's targets on the same platform: Windows Mobile 2003 to 5.0 Pocket PCs towards ARM CE 3.0's.

## LLVM

| Architecture | LLVM |
| --- | --- |
| MIPS, x86 | any with those backends: Homebrew's (`brew install llvm lld cmake`) or Ubuntu's (`apt install clang lld llvm cmake`). Apple's clang has neither |
| SH3, ARM | [llvm-builds](https://github.com/grimace-sdk/llvm-builds/releases) v6 or later, for the `sh3el-unknown-none-wince` and `armv4-unknown-none-wince` targets, which no LLVM release has. It has ARM, MIPS, SuperH and the C++ standard library |

- The toolchain uses, in order: `GRIMACE_LLVM_ROOT` (a CMake setting or environment variable), `llvm` in this folder (a link to the unpacked release), Homebrew's LLVM, then the `clang` on `PATH`.
- The macOS binaries aren't notarized: if a browser downloaded the tarball, run `xattr -dr com.apple.quarantine` on the unpacked folder.
- Other LLVMs, and llvm-builds before v5, inline 1, 2 and 4-byte atomics on MIPS with `ll` and `sc`, which MIPS I processors lack.
- From v6, ARM CE uses `.init_array` for constructors, and on ARM and SH3 CE, globals of two bytes or more, string literals included, are 4-byte aligned, as Microsoft's compilers align them. CE reads some names as words, such as `CreateAPISet`'s, which fault if misaligned. MIPS isn't covered, since its triple isn't CE's.

## Usage

```cmake
cmake_minimum_required(VERSION 3.20)
project(myapp C)
include(Grimace)

grimace_add_executable(myapp main.c ICON myapp.ico RESOURCES myapp.rc resource.h)
target_link_libraries(myapp PRIVATE grimace::commctrl)

grimace_add_library(mylib mylib.c EXPORTS Add Greeting=greeting_impl)
```

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/grimace/cmake/toolchain.cmake -DGRIMACE_ARCH=mips -DGRIMACE_CE_VERSION=2
cmake --build build
```

This gives `build/myapp.exe` and `build/mylib.dll`, with the linked `.elf` beside each.

### Functions

`grimace_add_executable(target [EXCLUDE_FROM_ALL] [OUTPUT file.exe] [ICON file.ico] [TEXT_ADDRESS address] [STACK_SIZE bytes] [RESOURCES files...] sources...)` makes a program, entry point `WinMain`.

`grimace_add_library(target [EXCLUDE_FROM_ALL] [OUTPUT file.dll] [RESOURCES files...] EXPORTS name[=symbol]... | EXPORTS_FILE file sources...)` makes a DLL, entry point `DllMain`, or one that returns `TRUE` if the library has none.

| Option | |
| --- | --- |
| `TEXT_ADDRESS` | link the code at another address than `0x11000`, a multiple of `0x1000`. The image base stays `0x10000`, so the image covers the space between |
| `STACK_SIZE` | the stack reserve, a multiple of `0x1000`: by default `GRIMACE_DEFAULT_STACK_SIZE`, or else 64 KB as Microsoft's linker gives. CE commits it a page at a time |
| `RESOURCES` | `.rc` files are compiled with `llvm-rc` and linked into `.rsrc` with the icon. Other files listed are dependencies |
| `EXPORTS_FILE` | one export per line |

### Settings

| Setting | |
| --- | --- |
| `GRIMACE_ARCH` | `mips` (default), `sh3`, `x86` or `arm` |
| `GRIMACE_CE_VERSION` | the oldest version the program runs on: `3` for `arm` and `1` otherwise by default. `_WIN32_WCE` is `100`, `101`, `200`, `300`, `502` or `600` |
| `GRIMACE_PLATFORM`, `GRIMACE_DEVICE` | one platform's devices, or one device's ROM. See [Imports](#imports) |
| `GRIMACE_RC_INCLUDES` | more folders to search for resource script headers |
| `CMAKE_BUILD_TYPE` | `MinSizeRel` by default (`-Os`, plus `-fomit-frame-pointer` on ARM). `Debug` gives `-O0 -g`. `examples/CMakePresets.json` has a Debug preset for each target |

### Notes

- Programs and DLLs start in `runtime/startup.c`. It runs constructors (C++ globals and `__attribute__((constructor))`, in C too), calls `WinMain` or `DllMain`, then runs `atexit` handlers and destructors. It runs `.init_array` (MIPS, SH3, x86) and `.ctors` (ARM CE), with priorities, in GNU's order.
- CE has no `DialogBoxParamW`: use `FindResource`, `LoadResource` and `DialogBoxIndirectParamW`.
- `llvm-rc` needs the comma after a control's text or name (`LTEXT "Text", -1, 5, 5, 50, 12`), and keeps the leading spaces of a line continued with `\`. Microsoft's rc does neither.
- The PE has subsystem 9 (Windows CE GUI) with the CE version, as Microsoft's linkers write. CE 2.0 treats a program marked 1.0 as a CE 1.0 program, with another system font for client-area text.

## Imports

Each folder in `exports/` is a ROM's or SDK's export lists, with a `MANIFEST` giving its architecture, CE version, platform, devices and source. Devices with identical lists share a folder. `python3 tools/manifests.py` lists them.

The lists are layers, so a device records only how it differs:

| Folder | Holds |
| --- | --- |
| `exports/shared/<arch>-ce<version>` | what most lists for that architecture and version export, as changes from the architecture's previous version (`arm-ce3` from `arm-ce212`), or in full for its first |
| `exports/<list>` | a device's changes from its version's layer |

- A `<dll>.txt` has `+name` and `-name` lines, or plain names for a DLL its layer lacks. A `MANIFEST`'s `without` names the layer's DLLs the list lacks.
- `tools/manifests.py` rebuilds each list. `python3 tools/exports.py layer` recomputes the layers after a list changes.
- `python3 tools/exports.py ceporter <cem-porter>/data/compat` imports the CE 1 to 6 devices from [CEM Porter](https://github.com/Gadgetoid/cem-porter)'s tables, which its `tools/compat-tables.mjs` reads from the ROMs in cerf-bundles.

A target imports from the lists of its architecture at its CE version and later, since the program may run on any of those devices, but only from the platforms with lists at its own version: ARM CE 3.0 programs don't import from the Brain's CE 6.0 lists. Embedded devices (`embedded`: industrial panels, dev boards, the CE emulator images) are the exception: they only count when chosen with `GRIMACE_PLATFORM=embedded` or `GRIMACE_DEVICE`, or where no other platform has a list at the version.

| A function | is |
| --- | --- |
| all of them export | a direct import, which the loader binds |
| only some export | a lazy import. Its first call loads the DLL and looks the function up with `GetProcAddressW`, also trying the name with or without a trailing `W` (CE 1.0 exports `FindResource`, CE 2.0 `FindResourceW`). If the device lacks it, the call sets `ERROR_CALL_NOT_IMPLEMENTED` and returns 0 |
| none export | a compile error |

`ce_has(function)` says whether the device has a function: always true for a direct import, and for a lazy one it binds it and says whether that worked. Use it before calling a lazy import where 0 isn't a failure, or to fall back to something else:

```c
if (ce_has(GetModuleFileNameW)) {
    GetModuleFileNameW(NULL, path, MAX_PATH);
}
```

- With `GRIMACE_DEVICE` the target imports exactly what that device's ROM exports at its CE version, all direct, and anything else is a compile error. With `GRIMACE_PLATFORM`, only that platform's lists count. Configuring prints how many direct and lazy imports the target gets, and `python3 tools/manifests.py --lists --arch arm --version 3` lists the lists it covers.
- `grimace::<dll>` links a DLL's import library, and `grimace::coredll` is always linked. Each is a set of weak stubs, so only the functions a program calls end up in it, and a program's own definition of a name overrides the import.
- A direct import's stub jumps through a slot in `.iat.<DLL>.<name>`. A lazy one's jumps through a pointer that starts at a thunk calling `__grimace_lazy_bind` (`runtime/lazy.c`), which stores what it found there.
- `tests/lazy` calls a few lazy imports and reports what it found. Run on the devices, the MIPS CE 1.0 build binds on CE 2.0 and reports missing functions on CE 1.0, and the SH3 CE 1.0 build binds on an HP 620LX (CE 2.0).

## Packages

`grimace_package()` packages programs three ways: a CAB, which wceload installs on CE 2.0 and later; a self-installing program, which also installs on CE 1.0; and a zip of the same files.

```cmake
grimace_package("My App" PROVIDER "Me"
    TARGETS myapp mylib
    FILES readme.txt
    SHORTCUTS myapp "My App")
```

`cmake --build build --target packages` writes `build/packages/MyApp-<target>.cab`, `MyApp-<target>-setup.exe` and `.zip`. `<target>` is the architecture and version (`mips-ce2`), plus the platform or device if set.

| Option | |
| --- | --- |
| `DIRECTORY` | where they go, `%CE1%\<name>` (`\Program Files\<name>`) by default. wceload on the Handheld PC 2000 asks first, starting there |
| `SHORTCUTS` | pairs of a target and the name of its shortcut in the Start menu's Programs (`%CE11%`) |
| `FORMATS` | any of `cab`, `zip` and `exe`, all three by default |

- The self-installer is `installer/ceinstall.c`, from CEM Porter, with the files, shortcuts and registry values appended as a `.cepk` section by `tools/selfinstall.py`. Run it on the device to install, with `/q` to install without asking, and run it again to remove the program.
- `tools/selfinstall.py MyApp.cab MyApp.exe --stub ceinstall.exe` converts a CAB, uncompressed or MSZIP, to a self-installer for CE 1.0, with `ceinstall.exe` built for the CAB's processor.
- `tools/cab.py` writes uncompressed cabinets with the setup file (`.000`) wceload reads. The examples' CAB installs with the Jornada 720's own wceload, with its files and shortcuts.

### Every target at once

A toolchain file builds for one target, so `cmake/GrimaceMatrix.cmake` builds a project for several, each in its own folder, and gathers their packages:

```cmake
cmake_minimum_required(VERSION 3.20)
project(myapp_all NONE)
include(/path/to/grimace/cmake/GrimaceMatrix.cmake)

grimace_matrix(NAME "My App" SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/app PACKAGE
    TARGETS mips-ce1 sh3-ce1 x86-ce2 arm-ce3)
```

- Targets are `<arch>-ce<version>`, with `ce101` for 1.01 and `ce52` for 5.2, and an optional `-<device>`: `mips-ce1-velo1`.
- `GRIMACE_MIPS_LLVM`, `GRIMACE_SH3_LLVM`, `GRIMACE_X86_LLVM` and `GRIMACE_ARM_LLVM` (settings or environment variables) choose each architecture's LLVM. `CMAKE_ARGS` passes more settings to every build.
- With `PACKAGE`, `build/packages` gets every target's CAB, self-installer and zip, and `<NAME>.ini` for the desktop Application Manager. It lists one CAB per processor, the build for the lowest version, which runs on later ones too.
- `make packages` builds `examples/matrix`, the examples for every target.

## Headers

The Win32 headers are CeGCC's w32api (public domain) in `include/w32api`, patched to match the Windows CE 1.0, 2.0 and 3.0 SDKs. `include/w32api/VENDOR.md` lists the patches. `include/` wraps them:

- `#include <windows.h>` gives the API as the target has it. `UNICODE` is defined, and it brings in the C runtime headers and the object store database (`windbase.h`).
- Calling a function the target's ROM doesn't export is a compile error, e.g. `'GetModuleFileNameW' is unavailable: not in Windows CE 1.0`.
- On CE 3.0 and later, declarations generated from the Handheld PC 2000 SDK (`include/grimace/sdk`) add COM and OLE, the shell, WinInet, crypto and security, the address book and mail store, and others. Define `GRIMACE_NO_SDK` to leave them out.
- `STRICT` is on, so handle types are distinct. Define `NO_STRICT` for SDK code that mixes them.
- CE exports some wide functions without the `W` (`FindResource`, `GetVersionEx`, `GetClassLong`, the `Ras` and `acm` functions): either name works.
- Every function the ROMs export has a docstring and named parameters, from `docs/api`, with the versions that have it where they differ.
- `windowsx.h` has the SDK's message crackers. Include it yourself, except on CE 1.01, whose `windows.h` includes it.

grimace's own device headers, from Microsoft's public reference unless noted:

| Header | Declares |
| --- | --- |
| `ceddk.h`, `msgqueue.h`, `pm.h` | what CE 6.0 drivers use, checked against Platform Builder 6.0 |
| `pkfuncs.h` | `KernelIoControl`, `InterruptInitialize`, `InterruptDone`, `InterruptDisable`, `WaitForAPIReady`, `CacheSync` and `CacheRangeFlush`, without the OAL's IOCTL and `SYSINTR_` values, which no public source gives. The `SH_` API set numbers are the Handheld PC 2000 SDK's before CE 6.0. On CE 6.0 they're 64 higher: a PW-A7300 has sets 80, 81, 83, 84, 85 and 86 registered, `SH_WMGR` (81) is confirmed by Platform Builder's build, and the others are named by that pattern. CE 6.0 has no `SH_INIT`, `SH_TAPI`, `SH_PATCHER`, `SH_IMM` or `SH_WNET` yet |
| `pm.h` | also the power manager's `SetPowerRequirement`, `ReleasePowerRequirement`, `SetSystemPowerState`, `PowerPolicyNotify` and the power IOCTLs |
| `aygshell.h` | Windows Mobile's shell: `SHFullScreen`, `SHInitDialog`, `SHHandleWMActivate`, `SHHandleWMSettingChange`, `SHSipPreference`, `SHCreateMenuBar` and others |
| `cs.h`, `csmedia.h` | the camera driver interface for `CAM1:` and `PIN1:`: property sets, `IOCTL_CS_PROPERTY`, `IOCTL_STREAM_INSTANTIATE`, `IOCTL_CS_BUFFERS`, stream descriptors, message queue buffers and video formats. The public KS layouts under CE's names, with values and layouts confirmed against the Juno SB's camera driver |
| `winddi.h` | the display driver interface: `DrvEnableDriver`, `DRVENABLEDATA`, the `ENGCALLBACKS` table drivers call GDI through (CE exports no `Eng*` functions), and the structures and constants. From Microsoft's public Windows CE 5.0 reference and mingw-w64's public domain `winddi.h`, not Platform Builder or any GPE or BSP source. Where the reference contradicts itself, the doc comments say which reading the header takes |

`tests/ce6` builds a program, a control panel applet, a stream driver DLL and a display driver DLL. `tests/wm` builds programs using the Windows Mobile headers for the Juno SB, and asserts the camera structures' sizes and offsets.

## Runtime

`grimace::runtime` is always linked, and a program only gets the parts it calls.

| Part | Gives |
| --- | --- |
| `runtime/compiler-rt` | builtins: soft float, 64-bit division |
| `runtime/libc` | the C library CE 3.0's coredll exports, for targets whose coredll lacks it. A function is built from here unless every device the target covers exports it, so CE 1.0 and 2.0 programs get `memcpy`, `memmove`, `memset` and `memcmp` (a word at a time where the pointers allow), stdio, `printf` and `scanf`, `malloc`, the string, `ctype` and `stdlib` functions, maths and `setjmp`. CE 3.0 and later use the ROM's |
| `runtime/c99` | the C99 and C11 functions Microsoft's C library lacks, over whichever one the target has (below) |
| `runtime/c99/time.c` | `time`, `timespec_get` (`TIME_UTC`, `TIME_MONOTONIC`), `clock`, `gmtime`, `localtime`, `mktime`, `strftime`, `asctime`, `ctime`, over `GetSystemTime` and `GetLocalTime`, in milliseconds. Local time is the device's current offset from UTC |
| `runtime/c99/atomic.c` | the `__atomic_*` calls clang makes for atomics (1 to 8 bytes on ARM and SH3, 8 bytes on MIPS and x86) and the generic ones for `_Atomic` types, over `InterlockedExchange` and `InterlockedTestExchange`. 8-byte and other sizes take a lock. CE is uniprocessor, so `__sync_synchronize` is a compiler barrier |
| `runtime/c99/emutls.c` | `thread_local` and `_Thread_local`, as emulated TLS over one CE TLS slot (below) |

`runtime/c99` and its headers:

| | |
| --- | --- |
| `stdio.h` | `snprintf`, `perror`, `rewind`, `setvbuf` (only CE 6.0's coredll acts on it), `setbuf`, `BUFSIZ`, `grimace_redirect_stdio` |
| `stdlib.h` | `strtoll`, `abort`, `_Exit`, `exit`, `atexit`, `getenv` (the environment starts empty), `bsearch`, `system` (runs nothing, returns -1) |
| `string.h` | `strdup`, `strndup`, `strnlen` |
| `math.h` | `float` and `long double` functions, and the classification macros (`isnan`, `fpclassify` and so on) |
| `inttypes.h` | `PRI*` and `SCN*` with `I64` for 64-bit types, which every target's `printf` and `scanf` read; `strtoimax` |
| `assert.h`, `memory.h`, `wctype.h`, `alloca.h`, `malloc.h` | as usual |
| `locale.h` | only the "C" locale |
| `signal.h` | `raise` calls the handler `signal` set, or exits |
| `fenv.h` | soft float: round to nearest, no exceptions |
| `errno.h`, `wchar.h` | `errno`, `wmemcpy` and the other wide memory functions |

- `exit` runs `atexit` handlers, and C++ destructors if there are any. Returning from `WinMain` runs them too.
- `grimace_redirect_stdio(L"\\log.txt")` sends `stdout` and `stderr` to a file, appending. It returns 0, 1 if only `stdout` moved, or -1. Each line is written as it ends: unbuffered on CE 6.0, line by line with `runtime/libc` on CE 1.0 and 2.0, and buffered on CE 3.0, whose coredll lacks `setvbuf`.
- `swprintf` keeps Microsoft's form. Define `GRIMACE_C99_SWPRINTF` for C99's.
- The maths in `runtime/libc` is musl's (MIT, `runtime/libc/math/README`). Microsoft's Bessel functions (`_j0` and so on) aren't there, so they're unavailable below CE 3.0.

`runtime/libc` behaves as CE 3.0's coredll does where they differ from C99, so a program prints and reads the same on every version:

- `printf` writes three exponent digits (`1e+006`), `1.#INF` and `1.#QNAN`, and rounds halfway decimals away from zero, from the exact binary value. `strtod` and `scanf` round correctly.
- Files open in text mode unless the mode has `b`, with `\r\n` on disk for `\n`.
- `stdout` and `stderr` go to the debug output a line at a time, and `stdin` is always at its end.
- Narrow file names and strings in wide functions are Latin-1.

`tests/libc/host.c` (`make check-libc-host`) checks the formatting, `strtod` and `scanf` engines against this computer's C library. `tests/libc/device` (`make check-libc`) checks the C library on a device: it passes on the Velo's CE 1.0 and 2.0 ROMs, an HP 620LX (SH3 CE 2.0) and the Jornada 720, whose ROM's C library it also checks.

### POSIX

`target_link_libraries(myapp PRIVATE grimace::posix)` adds a POSIX layer for ported code:

| Header | Gives |
| --- | --- |
| `unistd.h` | `read`, `write`, `lseek`, `close`, `fsync`, `ftruncate`, `truncate`, `dup`, `isatty`, `unlink`, `access`, `chdir`, `getcwd`, `rmdir`, `usleep`, `sleep`, `getpid`, `getuid` and the other ids (0) |
| `fcntl.h` | `open`, `creat` |
| `sys/stat.h` | `stat`, `fstat`, `mkdir` |
| `dirent.h` | `opendir`, `readdir`, `rewinddir`, `closedir` |
| `sys/time.h`, `time.h` | `gettimeofday`, `clock_gettime` (`CLOCK_REALTIME`, `CLOCK_MONOTONIC`), `nanosleep`, `gmtime_r`, `localtime_r` |
| `stdio.h` | `fileno`, `fdopen`, `fseeko`, `ftello`, and current-folder `fopen`, `freopen`, `remove` and `rename` |
| `stdlib.h` | `setenv`, `unsetenv`, `putenv` |
| `strings.h` | `strcasecmp`, `strncasecmp` |
| `signal.h` | `sigaction`, `sigset_t` and its functions, `kill` for the program itself, and POSIX signal numbers such as `SIGPIPE`, which only `raise` delivers |
| `sys/types.h`, `sys/param.h`, `limits.h`, `utime.h`, `pwd.h`, `sys/wait.h` | the types, `MAXPATHLEN`, `MIN`, `MAX`, `PATH_MAX`, `utime`, `getpwuid` (gives `NULL`), `wait` and `waitpid` (fail with `ECHILD`) |

- Descriptors 3 to 63 are files, always binary. Descriptors 1 and 2 write to the debug output as Latin-1, or to `grimace_redirect_stdio`'s file, and 0 is always at its end. None is a terminal.
- CE has no current folder, so `grimace::posix` keeps one, starting at the program's folder (`\` on CE 1.0, which can't say where that is). Relative paths, `.` and `..` resolve against it. Paths go through `mbstowcs`, and `/` becomes `\`.
- As glibc does, `string.h` includes `strings.h` and `stdlib.h` includes `alloca.h`.
- `fileno` gives a `FILE`'s descriptor, whose `read`, `write`, `lseek` and `fstat` go through the `FILE`. Closing that descriptor leaves the `FILE` open.
- `readdir` gives no `.` or `..`.
- `gettimeofday` and `clock_gettime` count milliseconds from `GetTickCount`, since many devices' clocks only give seconds, and go back to the clock if they drift more than 2 seconds from it.
- `off_t` is 32-bit. `st_mode` has no execute bits, and write permission unless the file is read-only. Times are UTC.
- The descriptor table isn't locked: open files from one thread at a time.

## C++

`.cpp` files are compiled as C++20 with `-fno-exceptions -fno-rtti -fno-threadsafe-statics`. `project(myapp C)` is enough: `Grimace` enables C++ itself.

| Library | Gives |
| --- | --- |
| `grimace::cxx_runtime` | linked for every C++ target: `operator new` and `delete` on `LocalAlloc`, `__cxa_atexit`, `__cxa_pure_virtual` |
| `grimace::rtti` | compiles the target with `-frtti`, for `dynamic_cast` and `typeid`. libc++ and the C++ runtime are built with RTTI, so their classes have type information, and `runtime/cxx/rtti.cpp` has `__dynamic_cast`. Programs without RTTI don't link any of it |
| `grimace::iostreams` | `<iostream>`, `<sstream>` and `<fstream>`, from a second build of libc++'s streams with only the "C" locale, whose narrow characters are Latin-1. `std::cout` and `std::cerr` write to `stdout` and `stderr`. A program that uses them is about 140 KB bigger; others don't change |

- On CE 3.0 and later the C++ standard library works: strings, containers, `<algorithm>`, `optional`, `variant`, smart pointers, `std::function`, ranges, `std::format`, `<chrono>`'s `system_clock` and `steady_clock`, `<system_error>` and `<stdexcept>`'s exception classes. There are no threads, locales (except with `grimace::iostreams`), `std::filesystem` or exceptions. See `examples/stl-text` and `examples/stl-collections`. Elsewhere, `include/new` and `include/initializer_list` are the only standard library headers.
- `thread_local` works on ARM, MIPS and x86. A thread's variables aren't freed when it exits. clang's SuperH target doesn't support thread-local storage.
- The headers' functions have C linkage in C++. Functions a DLL exports need `extern "C"`.
- `include/grimace/cxx` is a header-only C++ layer: RAII handles, results instead of exceptions, a checked `format`, and a window base. See [its README](include/grimace/cxx/README.md).

## Debugging

[ce-debug](https://github.com/grimace-sdk/ce-debug) is the debugger for grimace programs: an agent on the device, built with grimace, for copying files, listing folders, starting and stopping programs and taking screenshots, over TCP, serial, USB or an emulator's host mailbox.

### GDB and VS Code

Each program gets a GDB script beside its `.elf`, `<name>.elf.gdb`. It loads the symbols, sets the program to run as `\Windows\<name>.exe`, and defines `grimace-load`, which uploads the `.exe` and the project's DLLs (all but `EXCLUDE_FROM_ALL` ones) to `\Windows` through debugmgr, where `LoadLibraryW` finds them by name.

The emulators' GDB stubs serve these: [velo-emu](https://github.com/Gadgetoid/velo-emu) for MIPS CE 1.0 and 2.0, the SH3 emulator for SH3 CE 2.0, and [jornada-72x-emu](https://github.com/Gadgetoid/jornada-72x-emu) for ARM CE 3.0. `tools/grimace-emulator --target <target>` starts the one for `mips-ce1`, `mips-ce2`, `sh3-ce2` or `arm-ce3` with its GDB stub on port 2001, 2002, 2003 or 2720, or reports one already listening there. It finds velo-emu in `VELO_EMU`, with CE 1.0's ROM at `rom/nk.bin` there and CE 2.0's in `VELO_CE2_ROM`; the SH3 emulator in `VELO_SH3_EMU` with `VELO_SH3_ROM`; and jornada-72x-emu in `JORNADA_EMU`. `--state FILE` starts from a saved emulator state, copied so the file is kept, and `--headless` runs velo-emu without a window. The agent socket for ce-debug is `build/debugmgr/<target>/agent.sock` here.

```sh
tools/grimace-emulator --target mips-ce1 &
gdb -x build/mips-ce1/hello/hello.elf.gdb -ex "target extended-remote :2001" -ex "set target-wide-charset UTF-16LE" -ex grimace-load -ex "break WinMain" -ex run
```

Set the character set after connecting, since connecting resets it, so `WCHAR` strings show as text.

For VS Code, copy `vscode/settings.json`, `vscode/launch.json` and `vscode/tasks.json` into a project's `.vscode`. The settings have IntelliSense take the include paths, defines and compiler flags from CMake Tools, so hover, completion and signature help see the headers as the compiler does, with `_WIN32_WCE`, the architecture's defines and 16-bit `wchar_t`. `examples/CMakePresets.json` has a Debug preset per target to copy. The launch configurations need:

- the C/C++ and CMake Tools extensions, with the project configured with the Debug preset for the target you pick
- `gdb` with MIPS, SH3 or ARM support, and Python for OS symbols (Homebrew's `gdb` on macOS, `gdb-multiarch` on Linux)
- `GRIMACE_TOOLCHAIN` (this folder) and the emulator's variables above in VS Code's environment

F5 starts the emulator if it isn't running, uploads the selected target's `.exe` and runs it, stopping at breakpoints. The emulator keeps running between sessions. The emulators have no hardware watchpoints, so GDB uses software ones.

### OS symbols

For a CE image built with Platform Builder, GDB can also show the kernel's and the system DLLs' functions and source lines. Philips' Velo ROMs have no symbols, so this only applies to self-built images.

Set `GRIMACE_OS_SYMBOLS` (`GRIMACE_SH3_OS_SYMBOLS` for SH3) to a folder of ELF symbol files for the image, one per module, when configuring the project: `-DGRIMACE_OS_SYMBOLS=folder`, or in the environment, which also covers VS Code. The folder holds:

- `nk.elf`: the kernel, at its run-time addresses
- an ELF for each DLL, at its address in ROM, named as the emulator's GDB stub reports it: the name in the ROM's module table, with `.elf` for `.dll` (`coredll.elf`)
- `rom.elf`: the kernel and all the DLLs in one file
- optionally `rom.sha256`: the image's SHA-256, as `shasum -a 256` prints it, which `tools/grimace-emulator` checks against the ROM it starts

Make them from the PDBs and map files Platform Builder leaves in the image's release folder. They're derived from Microsoft's binaries, so keep them out of anything you publish.

With it set, the `.gdb` scripts load `nk.elf` and add the folder to the shared-library search path after the project's folders, and `grimace-load` runs `sharedlibrary`, so GDB reads each system DLL's symbols as the stub reports it. The stub names a DLL as the program loaded it, so a file in the folder whose name only differs in case is loaded too. If no system DLL has been read by the first stop, because the stub has no module list, `tools/gdbsymbols.py` replaces `nk.elf` with `rom.elf`, which works as ROM DLLs don't move. That needs GDB with Python. Source lines name files on the machine the image was built on, so GDB shows the line number without the text. Without the setting, the scripts are unchanged.

## Notes

- CE 1.0 has no `GetModuleFileNameW`, and `LoadLibraryW` doesn't search the program's folder. DLLs normally go in `\Windows`, or pass a full path (see `examples/dll`).
- A DLL's import tables go in a 0x200 byte space before its IAT. `mkpe.py` says if a DLL needs more: `target_link_options(mylib PRIVATE --defsym=GRIMACE_IMPORT_RESERVE=0x400)`.
- `tools/grimace-cc` filters clang's "MIPS-I support is experimental" warning on MIPS.
- Zero-initialised data takes no space in the file: `.data`'s virtual size covers it, as Microsoft's linker does.
- Programs and DLLs have no `.pdata`, so CE can't unwind their frames: `__try` doesn't work in them, and a fault inside a window procedure ends the process.
- The ARM target follows Microsoft's CE ARM calling standard as eVC3 compiles it; `make check-abi` checks it.
- ARM CE 5.2 and 6.0 target ARMV4I: code is built for ARMv4T and returns with `bx`, as do the import stubs, so Thumb code in the ROM can call it and be called.

## Make

| Target | |
| --- | --- |
| `examples-<target>` | build into `build/<target>`, for `mips-ce1`, `mips-ce2`, `sh3-ce1`, `sh3-ce101`, `sh3-ce2`, `x86-ce2`, `arm-ce3`, `arm-ce52` and `arm-ce6` |
| `examples` | all targets |
| `packages-<target>`, `packages` | package the examples for a target, or build and package them for every target (`examples/matrix`) |
| `check` | `check-abi`, `check-cxx-headers`, `check-stdlib`, `check-lazy`, `check-libc`, `check-ce6`, `check-wm`, `examples` and `check-tables` |
| `check-abi` | the ARM calling standard |
| `check-cxx-headers-<arch>` | the headers declare C functions with C linkage in C++, and the C++ layer compiles, for each CE version of `mips`, `sh3`, `x86` or `arm` |
| `check-stdlib` | the C++ standard library, RTTI, iostreams and constructor order on CE 3.0 |
| `check-lazy`, `check-lazy-<target>` | build `tests/lazy` |
| `check-libc`, `check-libc-<target>`, `check-libc-host` | build `tests/libc/device`, and run `tests/libc/host.c` here |
| `check-ce6` | build `tests/ce6` for ARM CE 6.0 and each Brain, and check with `tests/check-imports.py` that every import is in the device's lists |
| `check-wm` | build `tests/wm` for the Juno SB and check its imports |
| `check-tables` | check that every linked program's and DLL's constructor and destructor tables are word-aligned |
| `check-headers` | compare the headers with each SDK's (`GRIMACE_REFERENCE`, a folder with `include/ce100`, `ce101`, `ce200` and `ce300`) |
| `headers` | regenerate `include/` from a CeGCC checkout's w32api (`GRIMACE_W32API`) and the SDKs (`GRIMACE_REFERENCE`) |
| `docstrings` | rewrite the docstrings from `docs/api` |
| `exports` | read `exports/jornada720-ce3` from a Jornada 720 ROM image (`GRIMACE_ROM`) |

- `GRIMACE_MIPS_LLVM`, `GRIMACE_SH3_LLVM`, `GRIMACE_X86_LLVM` or `GRIMACE_ARM_LLVM` choose an LLVM for that architecture's targets and checks.
- `DEVICE` or `PLATFORM` builds for one device or platform, into `build/<target>-<device>`: `make examples-mips-ce1 DEVICE=velo1`.
- `tools/cerom.py` reads CE 3.0, 5.2 and 6.0 ROMs, and `tools/imgfs.py` Windows Mobile's IMGFS, where most of its DLLs are.

`make headers` runs, in order:

| Tool | Writes |
| --- | --- |
| `tools/vendor-w32api.py` | copies and patches w32api |
| `tools/mksdk.py` | `include/grimace/sdk` from the Handheld PC 2000 SDK, for the functions the Jornada 720 exports that w32api lacks |
| `tools/mkheaders.py` | the wrappers, and from `exports/` a file per target (each architecture and version, platform and device) in `grimace/unavailable`, `grimace/aliases` and `grimace/lazy`, which `grimace/unavailable.h`, `aliases.h` and `lazy-imports.h` include for the target being built |
| `tools/mkconstants.py`, `tools/mksdkconstants.py` | `grimace/constants.h` and `grimace/types.h` (CE 1.0, 1.01 and 2.0) and `grimace/constants-sdk.h` (CE 3.0 and later), with the SDK constants, macros and types the headers lack |
| `tools/mkdocs.py` | the docstrings from `docs/api` above each declaration, naming its parameters. Entries in `docs/api/sdk` fill in macros, messages and structures `docs/api` doesn't cover |

## Licence

MIT, see `LICENSE`. `include/w32api` is public domain (`README.w32api`), and `runtime/compiler-rt` is LLVM's, under Apache 2.0 with LLVM exceptions (`LICENSE.TXT`).
