#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1900"
BRU = "/tmp/P1900_brute"
SRC = "/tmp/luogu-ps/appendix/P1900/P1900.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1900/P1900_brute.cpp"

def compile_all():
    for src, dst in ((SRC, STD), (BRUS, BRU)):
        r = subprocess.run(
            ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-Werror", "-o", dst, src],
            capture_output=True, text=True,
        )
        if r.returncode != 0:
            sys.stderr.write(r.stderr)
            return 1
    return 0

def run(bin, inp):
    p = subprocess.run([bin], input=inp, text=True, capture_output=True)
    if p.returncode != 0:
        raise RuntimeError(p.stderr)
    return p.stdout

def fail(inp, a, b):
    print("FAIL")
    print("INPUT:")
    print(inp[:2000])
    print("STD:")
    print(a[:2000])
    print("BRUTE:")
    print(b[:2000])
    return 1

def main():
    if compile_all():
        return 1
    rng = random.Random(1900)
    tests = 0
    sample = "100 10\n1 2 3 4 5 6 7 11 12 13\n"
    exp = "13\n1 3 5 7 9 20 31 75 86 97\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    small = [
        "1 1\n1\n",
        "20 3\n1 2 6\n",
        "97 1\n13\n",
    ]
    for inp in small:
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(15):
        n = rng.randint(1, 3000)
        # first get count via std with a dummy query, then real queries within range
        dummy = f"{n} 1\n1\n"
        out = run(STD, dummy)
        cnt = int(out.splitlines()[0])
        k = rng.randint(1, min(20, cnt))
        ss = [rng.randint(1, cnt) for _ in range(k)]
        # a_s < n is guaranteed only when the value is < n; if last self == n, avoid cnt when it equals n
        vals = list(map(int, out.splitlines()[1].split()))
        # dummy only asked s=1. Rebuild queries safely: ask only indices whose values we will compare with brute fully.
        inp = f"{n} {k}\n" + " ".join(map(str, ss)) + "\n"
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        lines = oa.splitlines()
        if int(lines[0]) != cnt:
            return fail(inp, oa, f"cnt {cnt}")
        got = list(map(int, lines[1].split()))
        if any(v >= n and not (v < n or v == n) for v in got):
            return fail(inp, oa, "range")
        tests += 1
    for n in (10000, 100000):
        k = 8
        ss = [1, 2, 3, 4, 5, 6, 7, 13]
        inp = f"{n} {k}\n" + " ".join(map(str, ss)) + "\n"
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    n = 10000000
    k = 5000
    ss = [rng.randint(1, 13) for _ in range(k)]
    inp = f"{n} {k}\n" + " ".join(map(str, ss)) + "\n"
    t0 = time.perf_counter()
    oa = run(STD, inp)
    dt = time.perf_counter() - t0
    lines = oa.splitlines()
    if len(lines) != 2:
        return fail("large", oa[:200], "")
    cnt = int(lines[0])
    got = list(map(int, lines[1].split()))
    if len(got) != k or any(v < 1 or v > n for v in got):
        return fail("large", oa[:200], "")
    first13 = [1, 3, 5, 7, 9, 20, 31, 42, 53, 64, 75, 86, 97]
    if any(first13[s - 1] != got[i] for i, s in enumerate(ss)):
        return fail("large prefix", " ".join(map(str, got[:20])), " ".join(map(str, ss[:20])))
    if not (1 <= cnt <= n):
        return fail("large cnt", str(cnt), "")
    tests += 1
    print(f"max N=10000000 K=5000 time={dt:.4f}s cnt={cnt}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
