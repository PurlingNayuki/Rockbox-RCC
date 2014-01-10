#!/usr/bin/python
#
# Copyright 2004 Matt Mackall <mpm@selenic.com>
#
# inspired by perl Bloat-O-Meter (c) 1997 by Andi Kleen
#
# This software may be used and distributed according to the terms
# of the GNU General Public License, incorporated herein by reference.

import sys, os, re

def usage():
    sys.stderr.write("Usage: %s file1 file2\n" % sys.argv[0])
    sys.exit(-1)

if len(sys.argv) < 3:
    usage()

for f in sys.argv[1], sys.argv[2]:
    if not os.path.exists(f):
        sys.stderr.write("Error: file '%s' does not exist\n" % f)
        usage()

nm_args = " ".join([x for x in sys.argv[3:]])
def getsizes(file):
    sym = {}
    for l in os.popen("nm --size-sort %s %s" % (nm_args, file)).readlines():
        l = l.strip()
    # Skip empty lines
        if not len(l):
            continue
    # Skip archive members
        if len(l.split()) == 1 and l.endswith(':'):
          continue
        size, type, name = l.split()
        if type in "tTdDbBrR":
            if "." in name: name = "static." + name.split(".")[0]
            sym[name] = sym.get(name, 0) + int(size, 16)
    for l in os.popen("readelf -S " + file).readlines():
        x = l.split()
        if len(x)<6 or x[1] != ".rodata": continue
        sym[".rodata"] = int(x[5], 16)
    return sym

old = getsizes(sys.argv[1])
new = getsizes(sys.argv[2])
grow, shrink, add, remove, up, down = 0, 0, 0, 0, 0, 0
delta, common = [], {}

for a in old:
    if a in new:
        common[a] = 1

for name in old:
    if name not in common:
        remove += 1
        down += old[name]
        delta.append((-old[name], name))

for name in new:
    if name not in common:
        add += 1
        up += new[name]
        delta.append((new[name], name))

for name in common:
        d = new.get(name, 0) - old.get(name, 0)
        if d>0: grow, up = grow+1, up+d
        if d<0: shrink, down = shrink+1, down-d
        delta.append((d, name))

delta.sort()
delta.reverse()

print("%-48s %7s %7s %+7s" % ("function", "old", "new", "delta"))
for d, n in delta:
    if d: print("%-48s %7s %7s %+7d" % (n, old.get(n,"-"), new.get(n,"-"), d))
print("-" * 78)
total="(add/remove: %s/%s grow/shrink: %s/%s up/down: %s/%s)%%s\nTotal: %s bytes"\
    % (add, remove, grow, shrink, up, -down, up-down)
print(total % (" "*(80-len(total))))
