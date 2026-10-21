#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1815"
BRU = "/tmp/P1815_brute"
SRC = os.path.join(here, "P1815.cpp")
BSRC = os.path.join(here, "P1815_brute.cpp")

SAMPLE_IN = (
    "18\nNWWWWWWWWWWSESSSWS\n"
    "20\nSSSWWNENNNNNWWWWSSSS\n"
    "30\nEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n"
    "13\nSWWWWWWWWWNEE\n"
    "0\n"
)
SAMPLE_OUT = (
    "The worm successfully made all 18 moves.\n"
    "The worm ran into itself on move 9.\n"
    "The worm ran off the board on move 21.\n"
    "The worm successfully made all 13 moves.\n"
)

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
        print("RUNTIME", exe, p.returncode, p.stderr[:400])
        print(data[:500])
        return None
    return p.stdout

def oracle(data):
    # independent history simulation
    lines = data.strip().splitlines()
    out = []
    i = 0
    while i < len(lines):
        n = int(lines[i]); i += 1
        if n == 0:
            break
        s = lines[i].strip(); i += 1
        hx = [25] * 140
        hy = [0] * 140
        for t in range(20):
            hy[t] = 11 + t
        head = 19
        dead = None
        for step, mv in enumerate(s):
            nr, nc = hx[head], hy[head]
            if mv == "E":
                nc += 1
            elif mv == "W":
                nc -= 1
            elif mv == "S":
                nr += 1
            elif mv == "N":
                nr -= 1
            if not (1 <= nr <= 50 and 1 <= nc <= 50):
                dead = f"The worm ran off the board on move {step+1}."
                break
            hit = False
            for j in range(head - 18, head + 1):
                if hx[j] == nr and hy[j] == nc:
                    hit = True
                    break
            if hit:
                dead = f"The worm ran into itself on move {step+1}."
                break
            head += 1
            hx[head] = nr
            hy[head] = nc
        if dead is None:
            out.append(f"The worm successfully made all {n} moves.")
        else:
            out.append(dead)
    return "\n".join(out) + ("\n" if out else "")

def gen(rng):
    cases = rng.randint(1, 4)
    parts = []
    for _ in range(cases):
        n = rng.randint(1, 60)
        s = "".join(rng.choice("ESWN") for _ in range(n))
        parts.append(str(n))
        parts.append(s)
    parts.append("0")
    return "\n".join(parts) + "\n"

def main():
    if compile_all():
        return 1
    tests = 0
    got, bg = run(SOL, SAMPLE_IN), run(BRU, SAMPLE_IN)
    if got != SAMPLE_OUT or bg != SAMPLE_OUT or oracle(SAMPLE_IN) != SAMPLE_OUT:
        print("SAMPLE FAIL")
        print("sol", repr(got))
        print("bru", repr(bg))
        print("orc", repr(oracle(SAMPLE_IN)))
        return 1
    tests += 1
    edges = [
        "0\n",
        "1\nE\n0\n",
        "1\nW\n0\n",
        "1\nN\n0\n",
        "1\nS\n0\n",
        "30\n" + "N" * 30 + "\n0\n",
        "30\n" + "S" * 30 + "\n0\n",
        "25\n" + "E" * 25 + "\n0\n",
        "40\n" + "W" * 40 + "\n0\n",
        "2\nEN\n3\nSSS\n0\n",
    ]
    for data in edges:
        a, b, c = run(SOL, data), run(BRU, data), oracle(data)
        tests += 1
        if a is None or b is None or a != b or a != c:
            print("EDGE FAIL")
            print(data)
            print("sol", repr(a))
            print("bru", repr(b))
            print("orc", repr(c))
            return 1
    rng = random.Random(1815015)
    t0 = time.time()
    for t in range(200):
        data = gen(rng)
        a, b, c = run(SOL, data), run(BRU, data), oracle(data)
        tests += 1
        if a is None or b is None or a != b or a != c:
            print("MISMATCH", t)
            print(data)
            print("sol", repr(a))
            print("bru", repr(b))
            print("orc", repr(c))
            return 1
    elapsed = time.time() - t0
    # max-length single case timing
    big = "99\n" + ("ENWS" * 24 + "E" * 3) + "\n0\n"
    assert len(big.splitlines()[1]) == 99
    t1 = time.time()
    out = run(SOL, big)
    tmax = time.time() - t1
    if out is None or out != run(BRU, big) or out != oracle(big):
        print("LARGE FAIL", out)
        return 1
    print(f"TIME random={elapsed:.3f}s n=99 time={tmax:.3f}s ans={out.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
