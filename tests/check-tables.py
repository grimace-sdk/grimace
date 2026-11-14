"""
Check that the constructor and destructor tables' bounds in linked programs and DLLs are word-aligned and in order.
"""
import argparse
import glob
import os
import subprocess
import sys

TABLES = ["init_array", "fini_array", "ctors_priority", "ctors", "dtors_priority", "dtors"]


def symbols(nm, path):
    found = {}
    for line in subprocess.run([nm, path], capture_output=True, text=True, check=True).stdout.splitlines():
        parts = line.split()
        if len(parts) == 3:
            found[parts[2]] = int(parts[0], 16)
    return found


def main():
    parser = argparse.ArgumentParser(description="Check the constructor tables in every .elf under the given folders")
    parser.add_argument("folders", nargs="+")
    parser.add_argument("--nm", default="llvm-nm")
    arguments = parser.parse_args()
    checked = 0
    problems = 0
    for folder in arguments.folders:
        for path in sorted(glob.glob(os.path.join(folder, "**", "*.elf"), recursive=True)):
            table = symbols(arguments.nm, path)
            for name in TABLES:
                start, end = table.get("__%s_start" % name), table.get("__%s_end" % name)
                if start is None or end is None:
                    continue
                checked += 1
                if start % 4 or end % 4 or start > end:
                    problems += 1
                    print("check-tables: %s: __%s_start %#x, __%s_end %#x" % (path, name, start, name, end))
    print("check-tables: %d tables checked, %d problems" % (checked, problems))
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
