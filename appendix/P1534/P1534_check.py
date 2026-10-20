import subprocess, random, time, os, tempfile, shutil

DIR = os.path.dirname(os.path.abspath(__file__))

def compile_all(tmp):
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1534.cpp"),
                    "-o", os.path.join(tmp, "std")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1534_brute.cpp"),
                    "-o", os.path.join(tmp, "brute")], check=True)

def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    tmp = tempfile.mkdtemp()
    try:
        compile_all(tmp)
        rng = random.Random(20261018)

        # 1. 官方样例
        sample = "7\n5 3\n6 2\n7 2\n5 3\n5 4\n0 4\n0 6\n"
        assert run(os.path.join(tmp, "std"), sample) == "-2", "official sample mismatch"

        # 2. 随机对拍：n 小、时长 0..8
        for tc in range(300):
            n = rng.randint(1, 3000)
            lines = [str(n)]
            for _ in range(n):
                lines.append(f"{rng.randint(0,8)} {rng.randint(0,8)}")
            inp = "\n".join(lines) + "\n"
            a = run(os.path.join(tmp, "std"), inp)
            b = run(os.path.join(tmp, "brute"), inp)
            assert a == b, f"Mismatch on case {tc}:\n{inp[:120]}\nstd={a}, bf={b}"

        # 3. 边界：n=1 全 0；n=1 全 8
        assert run(os.path.join(tmp, "std"), "1\n0 0\n") == "-8"
        assert run(os.path.join(tmp, "std"), "1\n8 8\n") == "8"

        # 4. 最坏规模计时
        n = 3000
        worst = str(n) + "\n" + "\n".join("8 8" for _ in range(n)) + "\n"
        t0 = time.perf_counter()
        res = run(os.path.join(tmp, "std"), worst)
        dt = time.perf_counter() - t0
        print(f"P1534 check passed! 300 random cases + official sample + worst n=3000 "
              f"(ans={res}) in {dt:.4f}s")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

if __name__ == "__main__":
    main()
