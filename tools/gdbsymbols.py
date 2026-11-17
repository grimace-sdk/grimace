"""
Load the OS symbols into gdb as the program stops, sourced by the .gdb script grimace_add_executable() writes when
GRIMACE_OS_SYMBOLS is set. The script sets $grimace_os_symbols to a folder holding nk.elf, the kernel, and rom.elf, the
whole ROM, along with any of the ROM's DLLs as separate files.

The kernel's symbols load first. At each stop, any DLL that gdb lists as loaded without symbols is looked up in the
folder by name, ignoring case. When nothing matches, the kernel is swapped for the whole ROM instead.
"""
import os

import gdb

FOLDER = os.path.realpath(gdb.parse_and_eval("$grimace_os_symbols").string())


def loaded_files():
    return [os.path.realpath(objfile.filename) for objfile in gdb.objfiles() if objfile.filename]


def load_os_symbols(event):
    loaded = loaded_files()
    if os.path.join(FOLDER, "rom.elf") in loaded:
        return
    files = {name.lower(): name for name in os.listdir(FOLDER)}
    for line in gdb.execute("info sharedlibrary", to_string=True).splitlines():
        fields = line.split()
        if len(fields) < 2 or fields[-2] != "No":
            continue
        name = files.get(os.path.basename(fields[-1]).lower())
        if name and os.path.join(FOLDER, name) not in loaded:
            gdb.execute('add-symbol-file "%s"' % os.path.join(FOLDER, name))
            loaded.append(os.path.join(FOLDER, name))
    if [path for path in loaded if os.path.dirname(path) == FOLDER and os.path.basename(path) != "nk.elf"]:
        return
    gdb.execute('remove-symbol-file "%s"' % os.path.join(FOLDER, "nk.elf"))
    gdb.execute('add-symbol-file "%s"' % os.path.join(FOLDER, "rom.elf"))


gdb.execute('add-symbol-file "%s"' % os.path.join(FOLDER, "nk.elf"))
gdb.events.stop.connect(load_os_symbols)
