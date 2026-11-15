"""
Store the export lists as layers, so each device records only how it differs.

exports/shared/<arch>-ce<version> holds what most lists for that architecture and version export, as changes from the
previous version's layer for the architecture, or in full for its first version. Each exports/<list> holds a device's
changes from its version's layer. A layer's <dll>.txt has +name and -name lines, or plain names where its base has no
such DLL; its MANIFEST names its base and, under without, the base's DLLs it lacks. tools/manifests.py rebuilds each list.

    python3 tools/exports.py ceporter <ce-porter>/data/compat   import CEM Porter's lists for CE 1 to 6 devices
    python3 tools/exports.py layer                              recompute every layer from the lists
"""
import argparse
import collections
import glob
import json
import os
import re
import shutil
import sys

import manifests

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXPORTS = os.path.join(ROOT, "exports")
SHARED = os.path.join(EXPORTS, "shared")
ARCHITECTURES = {"ARM": "arm", "MIPS": "mips", "SH": "sh3"}
VERSIONS = {1: "1", 1.01: "1.01", 2: "2", 2.01: "2.01", 2.11: "2.11", 2.12: "2.12", 3: "3", 4.1: "4.1", 4.2: "4.2", 4.21: "4.21",
            5: "5", 5.01: "5.1", 5.02: "5.2", 6: "6"}
DLL_NAME = re.compile(r"[a-z0-9_.+-]+")
EXISTING_DEVICES = {"jornada720": ("jornada720-ce3", "jornada720"), "philips_velo_1_ce1": ("velo1-ce1", "velo1"),
                    "philips_velo_1_ce2": ("velo1-ce2", "velo1")}


def platform_of(os_name):
    name = os_name.split(" by ")[0]
    if "Smartphone" in name:
        return "smartphone"
    if name.startswith(("Pocket PC", "Windows Mobile")):
        return "pocketpc"
    if name.startswith("Palm-size"):
        return "palmsize"
    if name == "Handheld PC 2000":
        return "hpc2000"
    if name.startswith("Handheld PC Professional"):
        return "hpcpro"
    if name in ("Handheld PC", "Windows CE 1.0", "Windows CE 2.0"):
        return "hpc"
    return "embedded"


def read_list(folder):
    fields = {}
    for line in open(os.path.join(folder, "MANIFEST")):
        key, _, value = line.partition(":")
        fields[key.strip()] = value.strip()
    return fields


def write_layer(folder, fields, lists, base):
    shutil.rmtree(folder, ignore_errors=True)
    os.makedirs(folder)
    without = sorted(dll for dll in base if dll not in lists)
    if without:
        fields["without"] = " ".join(without)
    with open(os.path.join(folder, "MANIFEST"), "w") as manifest:
        manifest.writelines("%s: %s\n" % (key, value) for key, value in fields.items() if value or key == "device")
    for dll, names in sorted(lists.items()):
        if dll in base:
            lines = ["+" + name for name in sorted(names - base[dll])] + ["-" + name for name in sorted(base[dll] - names)]
        else:
            lines = sorted(names)
        if lines or dll not in base:
            with open(os.path.join(folder, dll + ".txt"), "w") as listing:
                listing.writelines(line + "\n" for line in lines)


def majority(lists):
    result = {}
    for dll in set().union(*(names.keys() for names in lists)):
        holders = [names[dll] for names in lists if dll in names]
        if len(holders) * 2 > len(lists):
            counts = collections.Counter(name for names in holders for name in names)
            result[dll] = {name for name, count in counts.items() if count * 2 > len(holders)}
    return result


def all_lists():
    found = {}
    for manifest in manifests.manifests():
        fields = read_list(manifest.folder)
        fields.pop("base", None)
        fields.pop("without", None)
        found[manifest.name] = (fields, {dll: set(names) for dll, names in manifest.dlls().items() if DLL_NAME.fullmatch(dll)})
    return found


def layer(lists):
    groups = collections.defaultdict(list)
    for name, (fields, dlls) in lists.items():
        groups[(fields["arch"], manifests.WIN32_WCE[fields["version"]])].append(name)
    shutil.rmtree(SHARED, ignore_errors=True)
    for name in lists:
        shutil.rmtree(os.path.join(EXPORTS, name), ignore_errors=True)
    previous = {}
    layers = {}
    for arch, version in sorted(groups):
        layers[(arch, version)] = majority([lists[name][1] for name in groups[(arch, version)]])
        version_name = [key for key, value in manifests.WIN32_WCE.items() if value == version][0]
        label = "%s-ce%s" % (arch, version_name.replace(".", ""))
        base_fields = {"arch": arch, "version": version_name}
        parent = previous.get(arch)
        if parent:
            base_fields["base"] = parent[0]
        write_layer(os.path.join(SHARED, label), base_fields, layers[(arch, version)], parent[1] if parent else {})
        previous[arch] = (label, layers[(arch, version)])
        for name in groups[(arch, version)]:
            fields, dlls = lists[name]
            fields = dict(fields)
            fields["base"] = label
            write_layer(os.path.join(EXPORTS, name), fields, dlls, layers[(arch, version)])


def ceporter(compat):
    devices = json.load(open(os.path.join(compat, "devices.json")))
    libraries = {}

    def names(dll, digest):
        if dll not in libraries:
            path = os.path.join(compat, "libraries", dll + ".json")
            libraries[dll] = json.load(open(path)) if os.path.exists(path) else {}
        return {entry[1] for entry in libraries[dll].get(digest, []) if isinstance(entry, list) and len(entry) > 1 and entry[1]}

    imported = {}
    for device in devices:
        if device["family"] not in ARCHITECTURES or device["ce"] not in VERSIONS or "Zune" in device["os"]["name"]:
            continue
        dlls = {}
        for library, digest in device["libraries"].items():
            exported = names(library, digest)
            name = library.lower()[:-4]
            if library.lower().endswith(".dll") and exported and DLL_NAME.fullmatch(name):
                dlls[name] = exported
        if not dlls:
            continue
        identifier = re.sub(r"[^a-z0-9_]", "_", device["id"].lower())
        folder, device_name = EXISTING_DEVICES.get(device["id"], (identifier, identifier))
        fields = {"arch": ARCHITECTURES[device["family"]], "version": VERSIONS[device["ce"]], "platform": platform_of(device["os"]["name"]),
                  "device": device_name,
                  "source": "%s, %s ROM %s in cerf-bundles/%s, read with CEM Porter's tools/compat-tables.mjs" % (
                      device["name"], device["os"]["name"], device["rom"], device["id"])}
        imported[folder] = (fields, dlls)
    merged = {}
    for folder, (fields, dlls) in sorted(imported.items()):
        key = (fields["arch"], fields["version"], fields["platform"], frozenset((dll, frozenset(names)) for dll, names in dlls.items()))
        if key in merged:
            merged[key][1]["device"] += " " + fields["device"]
            merged[key][1]["source"] += "; also " + fields["source"].split(",")[0]
        else:
            merged[key] = (folder, dict(fields), dlls)
    return {folder: (fields, dlls) for folder, fields, dlls in merged.values()}


def main():
    parser = argparse.ArgumentParser(description="Import export lists and store them as layers")
    commands = parser.add_subparsers(dest="command", required=True)
    importer = commands.add_parser("ceporter", help="import CEM Porter's lists for CE 1 to 6 devices, then relayer")
    importer.add_argument("compat", help="CEM Porter's data/compat folder")
    commands.add_parser("layer", help="recompute every layer from the lists")
    arguments = parser.parse_args()
    lists = all_lists()
    if arguments.command == "ceporter":
        for folder, (fields, dlls) in ceporter(arguments.compat).items():
            if folder in lists:
                existing_fields, existing = lists[folder]
                kept = {dll: names for dll, names in existing.items() if dll not in dlls}
                dlls.update(kept)
                fields["source"] += "; " + existing_fields["source"]
                print("exports: %s from CEM Porter, keeping %s" % (folder, " ".join(sorted(kept)) or "nothing else"))
            lists[folder] = (fields, dlls)
    layer(lists)
    print("exports: %d lists, %d shared layers" % (len(lists), len(glob.glob(os.path.join(SHARED, "*")))))


if __name__ == "__main__":
    sys.exit(main())
