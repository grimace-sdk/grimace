"""
Read a Windows Mobile IMGFS partition: its modules and files, and the DLLs' export tables.

IMGFS keeps each file as XPRESS-compressed blocks listed by an index. A module (FEFEFFFF) has a header block with
its CE module header and section table, then one stream per section; a plain file (FEF6FFFF) has one stream. A name
of four characters or fewer sits in the entry; a longer one is in a name entry (FBFEFFFF) the entry points to,
filling all of its 48 bytes without a terminator when it's 24 characters long.
"""
import argparse
import os
import struct

MODULE = 0xFFFFFEFE
FILE = 0xFFFFF6FE
NAME = 0xFFFFFEFB
E32_SIZE = 112
O32_SIZE = 24
EXPORT_UNIT_OFFSET = 36


def u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def xpress_decompress(source, output_size):
    output = bytearray()
    position = 0
    nibble = None
    while len(output) < output_size:
        flags = u32(source, position)
        position += 4
        for bit in range(31, -1, -1):
            if len(output) >= output_size:
                break
            if not (flags >> bit) & 1:
                output.append(source[position])
                position += 1
                continue
            match = struct.unpack_from("<H", source, position)[0]
            position += 2
            length = match & 7
            distance = (match >> 3) + 1
            if length == 7:
                if nibble is None:
                    length = source[position] & 0xF
                    nibble = position
                    position += 1
                else:
                    length = source[nibble] >> 4
                    nibble = None
                if length == 15:
                    length = source[position]
                    position += 1
                    if length == 255:
                        length = struct.unpack_from("<H", source, position)[0]
                        position += 2
                        if length == 0:
                            length = u32(source, position)
                            position += 4
                        length -= 22
                    length += 15
                length += 7
            length += 3
            for _ in range(length):
                output.append(output[-distance])
    return bytes(output[:output_size])


class Imgfs:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        self.entries = self._entries()

    def _name(self, offset):
        length = struct.unpack_from("<H", self.data, offset + 0xC)[0]
        if not 0 < length <= 24:
            return None
        if length <= 4:
            raw = self.data[offset + 0x10:offset + 0x10 + 2 * length]
        else:
            pointer = u32(self.data, offset + 0x14)
            if pointer + 4 + 2 * length > len(self.data) or u32(self.data, pointer) != NAME:
                return None
            raw = self.data[pointer + 4:pointer + 4 + 2 * length]
        try:
            name = raw.decode("utf-16-le")
        except UnicodeDecodeError:
            return None
        return name if name.isprintable() and "\\" not in name else None

    def _entries(self):
        found = {}
        for offset in range(0, len(self.data) - 0x34, 4):
            kind = u32(self.data, offset)
            if kind not in (MODULE, FILE):
                continue
            name = self._name(offset)
            index, length = u32(self.data, offset + 0x2C), u32(self.data, offset + 0x30)
            if name and index + length <= len(self.data) and length % 8 == 0:
                found.setdefault(name.lower(), (name, kind, offset))
        return found

    def _stream(self, index, length):
        result = bytearray()
        for entry in range(length // 8):
            compressed, uncompressed, offset = struct.unpack_from("<HHI", self.data, index + entry * 8)
            if compressed == 0 and offset == 0:
                continue
            uncompressed = uncompressed or 0x10000
            block = self.data[offset:offset + compressed]
            result += block if compressed == uncompressed else xpress_decompress(block, uncompressed)
        return bytes(result)

    def streams(self, name):
        _, kind, offset = self.entries[name.lower()]
        parts = [self._stream(u32(self.data, offset + 0x2C), u32(self.data, offset + 0x30))]
        if kind == MODULE:
            section = u32(self.data, offset + 8)
            while section:
                parts.append(self._stream(u32(self.data, section + 0x1C), u32(self.data, section + 0x20)))
                section = u32(self.data, section + 8)
        return parts

    def exports(self, name):
        parts = self.streams(name)
        header = parts[0]
        objects = struct.unpack_from("<H", header, 0)[0]
        export_rva, export_size = struct.unpack_from("<II", header, EXPORT_UNIT_OFFSET)
        if not export_size:
            return []
        sections = [struct.unpack_from("<6I", header, E32_SIZE + O32_SIZE * index) for index in range(objects)]

        def read(rva, length):
            for index, (vsize, section_rva, psize, _, _, _) in enumerate(sections):
                if section_rva <= rva and rva + length <= section_rva + max(vsize, psize) and index + 1 < len(parts):
                    contents = parts[index + 1]
                    start = rva - section_rva
                    return (contents + bytes(max(0, start + length - len(contents))))[start:start + length]
            raise ValueError("%s: RVA %x is in no section" % (name, rva))

        directory = read(export_rva, 40)
        name_count, _, names, _ = struct.unpack_from("<IIII", directory, 24)
        name_rvas = struct.unpack("<%dI" % name_count, read(names, 4 * name_count))
        found = []
        for name_rva in name_rvas:
            text = bytearray()
            while True:
                character = read(name_rva + len(text), 1)
                if character == b"\0":
                    break
                text += character
            found.append(text.decode("latin-1"))
        return found


def main():
    parser = argparse.ArgumentParser(description="List an IMGFS partition, or write its DLLs' export lists")
    parser.add_argument("image")
    parser.add_argument("--exports", metavar="FOLDER", help="write <dll>.txt, one exported name per line, for each DLL")
    parser.add_argument("--dll", action="append", default=[], help="only these DLLs (default: all)")
    arguments = parser.parse_args()
    image = Imgfs(arguments.image)
    wanted = [name.lower() for name in arguments.dll]
    for key in sorted(image.entries):
        name, kind, _ = image.entries[key]
        if kind != MODULE or not key.endswith(".dll") or (wanted and key not in wanted):
            continue
        try:
            names = image.exports(name)
        except (ValueError, struct.error) as error:
            print("%-24s %s" % (name, error))
            continue
        print("%-24s %d exports" % (name, len(names)))
        if arguments.exports and names:
            os.makedirs(arguments.exports, exist_ok=True)
            with open(os.path.join(arguments.exports, key[:-4] + ".txt"), "w") as output:
                output.writelines(export + "\n" for export in sorted(names))


if __name__ == "__main__":
    main()
