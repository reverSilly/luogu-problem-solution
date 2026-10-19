import subprocess, random, time, os

SRC = os.path.dirname(os.path.abspath(__file__))
TMP = "/tmp/P1551_bin"

def compile_all():
    os.makedirs(TMP, exist_ok=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1551.cpp"),
                    "-o", os.path.join(TMP, "P1551")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1551_brute.cpp"),
                    "-o", os.path.join(TMP, "P1551_brute")], check=True)

def run(bin_name, inp):
    p = subprocess.run([os.path.join(TMP, bin_name)], input=inp, text=True,
                       capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍
    for tc in range(300):
        n = rng.randint(1, 30)
        m = rng.randint(0, 30)
        q = rng.randint(1, 30)
        lines = [f"{n} {m} {q}"]
        for _ in range(m):
            lines.append(f"{rng.randint(1,n)} {rng.randint(1,n)}")
        for _ in range(q):
            lines.append(f"{rng.randint(1,n)} {rng.randint(1,n)}")
        inp = "\n".join(lines) + "\n"
        out_std = run("P1551", inp)
        out_bf = run("P1551_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 手工边界:n=1 自问自己
    assert run("P1551", "1 0 1\n1 1\n") == "Yes" == run("P1551_brute", "1 0 1\n1 1\n")
    # 手工用例:6 人 5 关系 3 询问(按题面描述手工构造)
    hand3 = "6 5 3\n1 2\n1 5\n3 4\n5 2\n1 3\n1 4\n2 3\n5 6\n"
    assert run("P1551", hand3) == run("P1551_brute", hand3) == "Yes\nYes\nNo"

    # 3. 最坏规模计时:n=m=p=5000
    n, m, q = 5000, 5000, 5000
    lines = [f"{n} {m} {q}"]
    for i in range(1, m + 1):                     # 连成若干链段
        lines.append(f"{i} {max(1, i-1)}")
    for i in range(q):
        lines.append(f"{rng.randint(1,n)} {rng.randint(1,n)}")
    worst = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1551", worst)
    dt = time.perf_counter() - t0
    print(f"P1551 check PASS! 300 random + 2 hand cases; worst n=m=p=5000 ({len(res.splitlines())} lines) in {dt:.4f}s")

if __name__ == "__main__":
    main()
