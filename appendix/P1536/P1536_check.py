import subprocess, random, time, os, tempfile, shutil

DIR = os.path.dirname(os.path.abspath(__file__))

def compile_all(tmp):
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1536.cpp"),
                    "-o", os.path.join(tmp, "std")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1536_brute.cpp"),
                    "-o", os.path.join(tmp, "brute")], check=True)

def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen_case(rng, nmax):
    n = rng.randint(1, nmax)
    m = rng.randint(0, min(n * n, 12))
    lines = [f"{n} {m}"]
    for _ in range(m):
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        lines.append(f"{u} {v}")   # 允许自环/重边/反向重边
    return "\n".join(lines) + "\n"

def main():
    tmp = tempfile.mkdtemp()
    try:
        compile_all(tmp)
        rng = random.Random(20261018)

        # 1. 官方样例：输出 1 0 2 998
        official = ("4 2\n1 3\n4 3\n3 3\n1 2\n1 3\n2 3\n5 2\n1 2\n3 5\n999 0\n0\n")
        assert run(os.path.join(tmp, "std"), official) == "1\n0\n2\n998", "official sample mismatch"

        # 2. 随机对拍：把若干组拼接，末尾补一行 0
        for tc in range(300):
            groups = rng.randint(1, 4)
            inp = ""
            for _ in range(groups):
                inp += gen_case(rng, 8)
            inp += "0\n"
            a = run(os.path.join(tmp, "std"), inp)
            b = run(os.path.join(tmp, "brute"), inp)
            assert a == b, f"Mismatch on case {tc}:\n{inp}\nstd={a}\nbf={b}"

        # 3. 边界：n=1,m=0 -> 0；纯重边
        assert run(os.path.join(tmp, "std"), "1 0\n0\n") == "0"
        assert run(os.path.join(tmp, "std"), "3 4\n1 2\n1 2\n2 1\n1 2\n0\n") == "1"

        # 4. 最坏规模计时：n=999, 随机稠密多组
        worst = ""
        for _ in range(20):
            n = 999
            m = 3000
            worst += f"{n} {m}\n"
            for _ in range(m):
                worst += f"{rng.randint(1,n)} {rng.randint(1,n)}\n"
        worst += "0\n"
        t0 = time.perf_counter()
        res = run(os.path.join(tmp, "std"), worst)
        dt = time.perf_counter() - t0
        print(f"P1536 check passed! 300 random cases + worst n=999*20groups "
              f"(last line={res.splitlines()[-1]}) in {dt:.4f}s")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

if __name__ == "__main__":
    main()
