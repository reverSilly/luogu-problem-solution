import subprocess, random, time, string

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1563.cpp", "-o", "/tmp/P1563"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1563_brute.cpp", "-o", "/tmp/P1563_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 官方样例 (题面文字描述的那组)
    sample = ("7 3\n0 singer\n0 reader\n0 mengbier\n1 thinker\n"
              "1 archer\n0 writer\n1 mogician\n0 3\n1 1\n0 2\n")
    assert run("/tmp/P1563", sample) == "writer", "official sample failed"

    # 2. 随机对拍: 模运算跳步 vs 逐格行走
    for tc in range(300):
        n = rng.randint(1, 10)
        m = rng.randint(1, 20)
        names = set()
        while len(names) < n:
            ln = rng.randint(1, 10)
            names.add("".join(rng.choice(string.ascii_lowercase) for _ in range(ln)))
        names = list(names)
        lines = [f"{n} {m}"]
        for i in range(n):
            lines.append(f"{rng.randint(0, 1)} {names[i]}")
        for _ in range(m):
            a = rng.randint(0, 1)
            s = rng.randint(1, n - 1) if n > 1 else 1
            lines.append(f"{a} {s}")
        inp = "\n".join(lines) + "\n"
        out_std = run("/tmp/P1563", inp)
        out_bf = run("/tmp/P1563_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 3. 最坏规模计时: n=m=1e5
    n = 100000
    m = 100000
    lines = [f"{n} {m}"]
    for i in range(n):
        lines.append(f"{i % 2} p{i}")
    for i in range(m):
        lines.append(f"{i % 2} {random.Random(i).randint(1, n - 1)}")
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("/tmp/P1563", max_inp)
    dt = time.perf_counter() - t0
    print(f"PASS P1563! official sample + 300 random cases + n=m=1e5 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
