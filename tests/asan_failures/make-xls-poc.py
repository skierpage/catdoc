#!/usr/bin/python3
"""Make a minimal .xls file containing the given BIFF records, for tests.

Usage: make-xls-poc.py OUTPUT.xls HEX...

Each HEX argument is the bytes of one or more BIFF records (2-byte type,
2-byte length, data; little-endian) as hexadecimal digits. They are placed
after a BIFF8 BOF record in the Workbook stream of an OLE file, so crafted
or malformed records can be fed to xls2csv. For example a MULRK record
with row 0, columns 0 to 0 and one RK value:
    make-xls-poc.py one-cell.xls bd000c00 0000 0000 0000 00002440 0000
"""
import struct
import sys

SECTOR = 512
END, FAT, FREE = 0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFF


def dir_entry(name, kind, child, start, size):
    utf16 = (name + "\0").encode("utf-16-le")
    return (utf16.ljust(64, b"\0") + struct.pack("<HBB", len(utf16), kind, 1)
            + struct.pack("<III", FREE, FREE, child) + bytes(36)
            + struct.pack("<III", start, size, 0))


out, records = sys.argv[1], bytes.fromhex("".join(sys.argv[2:]))
bof = bytes.fromhex("0908 1000 0006 0500 bb0d cc07 00000000 06000000")
stream = bof + records
# Pad to 4096 bytes, so that the stream is not stored in the mini stream
stream = stream.ljust(max(4096, -(-len(stream) // SECTOR) * SECTOR), b"\0")
nsect = len(stream) // SECTOR
# Sector 0 is the FAT, sector 1 the directory, the stream starts at sector 2
fat = [FAT, END] + list(range(3, 2 + nsect)) + [END]
assert len(fat) <= SECTOR // 4, "too many records for a one-sector FAT"
fat += [FREE] * (SECTOR // 4 - len(fat))
header = (bytes.fromhex("d0cf11e0a1b11ae1") + bytes(16)
          + struct.pack("<HHHHH", 0x3E, 3, 0xFFFE, 9, 6) + bytes(6)
          + struct.pack("<IIIIIIIII", 0, 1, 1, 0, 4096, END, 0, END, 0)
          + struct.pack("<109I", 0, *[FREE] * 108))
directory = (dir_entry("Root Entry", 5, 1, END, 0)
             + dir_entry("Workbook", 2, FREE, 2, len(stream))
             + dir_entry("", 0, FREE, 0, 0) * 2)
with open(out, "wb") as f:
    f.write(header + struct.pack("<%dI" % len(fat), *fat) + directory + stream)
