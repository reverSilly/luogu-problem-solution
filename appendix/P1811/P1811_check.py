#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1811"
BRU = "/tmp/P1811_brute"
SRC = os.path.join(here, "P1811.cpp")
BSRC = os.path.join(here, "P1811_brute.cpp")

SAMPLE_IN = "4 4 2\n1 2\n2 3\n3 4\n1 3\n1 2 3\n1 3 4\n"
SAMPLE_OUT = "4\n1 3 2 3 4\n"

def compile_all():
    for src, out in ((SRC, SOL), (BSRC, BRU)):
        r = subprocess.run(["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-o", out, src],
                           capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            print("COMPILE FAIL", src)
            return 1
    return 0

def run(exe, data, timeout=30):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=timeout)
    if p.returncode != 0:
        print("RUNTIME", exe, p.returncode, p.stderr[:500])
        print(data[:800])
        return None
    return p.stdout

def parse_graph(data):
    lines = data.strip().splitlines()
    n, m, k = map(int, lines[0].split())
    g = [set() for _ in range(n + 1)]
    idx = 1
    for _ in range(m):
        a, b = map(int, lines[idx].split())
        idx += 1
        g[a].add(b)
        g[b].add(a)
    ban = set()
    for _ in range(k):
        a, b, c = map(int, lines[idx].split())
        idx += 1
        ban.add((a, b, c))
    return n, g, ban

def check_out(data, out):
    n, g, ban = parse_graph(data)
    if out is None:
        return False, "runtime"
    lines = out.splitlines()
    if len(lines) == 1 and lines[0].strip() == "-1":
        return True, None
    if len(lines) != 2:
        return False, "lines"
    try:
        S = int(lines[0].strip())
        nodes = list(map(int, lines[1].split()))
    except Exception as e:
        return False, str(e)
    if S < 0 or len(nodes) != S + 1:
        return False, "len"
    if nodes[0] != 1 or nodes[-1] != n:
        return False, "ends"
    if any(x < 1 or x > n for x in nodes):
        return False, "range"
    for i in range(S):
        a, b = nodes[i], nodes[i + 1]
        if b not in g[a]:
            return False, f"noedge {a}->{b}"
        if i >= 1:
            pre = nodes[i - 1]
            if (pre, a, b) in ban:
                return False, f"ban {pre} {a} {b}"
    return True, S

def main():
    if compile_all():
        return 1
    tests = 0
    got = run(SOL, SAMPLE_IN)
    bg = run(BRU, SAMPLE_IN)
    if got != SAMPLE_OUT:
        print("SAMPLE EXACT FAIL")
        print(repr(got))
        return 1
    ok, s1 = check_out(SAMPLE_IN, got)
    ok2, s2 = check_out(SAMPLE_IN, bg)
    if not ok or not ok2 or s1 != s2 or s1 != 4:
        print("SAMPLE VALID FAIL", ok, s1, ok2, s2, repr(bg))
        return 1
    tests += 1
    edges = [
        "1 0 0\n",
        "2 1 0\n1 2\n",
        "2 1 1\n1 2\n1 2 1\n",
        "3 2 1\n1 2\n2 3\n1 2 3\n",
        "3 3 1\n1 2\n2 3\n1 3\n1 2 3\n",
        "4 3 0\n1 2\n2 3\n3 4\n",
        "3 0 0\n",
        "4 4 1\n1 2\n2 3\n3 4\n1 4\n1 4 3\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        oka, sa = check_out(data, a)
        okb, sb = check_out(data, b)
        if not oka or not okb or sa != sb:
            print("EDGE FAIL")
            print(data)
            print("sol", repr(a), sa, oka)
            print("bru", repr(b), sb, okb)
            return 1
        # if one says -1 both must
        if a.strip() == "-1" and b.strip() != "-1":
            print("NEG mismatch")
            return 1
    rng = random.Random(1811011)
    t0 = time.time()
    for t in range(250):
        n = rng.randint(1, 8)
        m = rng.randint(0, min(14, max(0, n * 2)))
        lines = []
        edgeset = set()
        elines = []
        for _ in range(m):
            a = rng.randint(1, n)
            b = rng.randint(1, n)
            elines.append(f"{a} {b}")
            edgeset.add((min(a, b), max(a, b), a, b))
        k = rng.randint(0, 8)
        blines = []
        for _ in range(k):
            a = rng.randint(1, n)
            b = rng.randint(1, n)
            c = rng.randint(1, n)
            blines.append(f"{a} {b} {c}")
        data = "\n".join([f"{n} {m} {k}"] + elines + blines) + "\n"
        aout, bout = run(SOL, data), run(BRU, data)
        tests += 1
        oka, sa = check_out(data, aout)
        okb, sb = check_out(data, bout)
        if not oka or not okb or sa != sb:
            print("MISMATCH", t)
            print(data)
            print("sol", repr(aout), sa)
            print("bru", repr(bout), sb)
            return 1
    rnd_time = time.time() - t0
    rng2 = random.Random(13)
    n, m, k = 3000, 20000, 100000
    lines = [f"{n} {m} {k}"]
    # backbone path so the graph is not empty of routes
    for i in range(1, n):
        lines.append(f"{i} {i+1}")
    extra = m - (n - 1)
    for _ in range(extra):
        a = rng2.randint(1, n)
        b = rng2.randint(1, n)
        lines.append(f"{a} {b}")
    for _ in range(k):
        a = rng2.randint(1, n)
        b = rng2.randint(1, n)
        c = rng2.randint(1, n)
        lines.append(f"{a} {b} {c}")
    big = "\n".join(lines) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=30)
    tmax = time.time() - t1
    if out is None:
        print("LARGE FAIL runtime")
        return 1
    first = out.splitlines()[0] if out.strip() else ""
    if first != "-1" and not first.isdigit():
        print("LARGE FAIL out", out[:200])
        return 1
    print(f"TIME random={rnd_time:.3f}s large_n={n} time={tmax:.3f}s ans={first}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
