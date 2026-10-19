import subprocess, random, time, os, tempfile, shutil

DIR = os.path.dirname(os.path.abspath(__file__))

def compile_all(tmp):
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1538.cpp"),
                    "-o", os.path.join(tmp, "std")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1538_brute.cpp"),
                    "-o", os.path.join(tmp, "brute")], check=True)

def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout

def dash_groups(line):
    # 统计一行里由 '-' 组成的连续段个数
    return len([g for g in line.replace('\n', '').split(' ') if g and set(g) == {'-'}])

def main():
    tmp = tempfile.mkdtemp()
    try:
        compile_all(tmp)
        rng = random.Random(20261018)

        # 1. 官方样例: k=2, 1234567890 -> 打印并用七段管结构不变量核对
        k, s = 2, "1234567890"
        out = run(os.path.join(tmp, "std"), f"{k}\n{s}\n")
        lines = out.split("\n")[:-1]        # 去掉末尾空串
        assert len(lines) == 2 * k + 3, f"height wrong: {len(lines)}"
        # 上横:数字 2,3,5,6,7,8,9,0 共 8 段; 中横:2,3,4,5,6,8,9 共 7 段; 下横同上 7 段
        assert dash_groups(lines[0]) == 8, f"top rows {lines[0]!r}"
        assert dash_groups(lines[k + 1]) == 7, f"mid rows {lines[k + 1]!r}"
        assert dash_groups(lines[2 * k + 2]) == 7, f"bot rows {lines[-1]!r}"
        # 打印样例(用 · 可视化空格,便于人工核对)
        print("--- official sample k=2 s=1234567890 (·=space) ---")
        for ln in lines:
            print(ln.replace(' ', '·'))
        print("---")

        # 2. 随机对拍: 各种 k 与数字串
        for tc in range(300):
            k = rng.randint(1, 30)
            ln = rng.randint(1, 20)
            s = "".join(rng.choice("0123456789") for _ in range(ln))
            inp = f"{k}\n{s}\n"
            a = run(os.path.join(tmp, "std"), inp)
            b = run(os.path.join(tmp, "brute"), inp)
            assert a == b, f"Mismatch on case {tc} (k={k}, s={s})"

        # 3. 单数字 0..9, k=1 与 k=30 分别对拍(覆盖奇数/大尺寸)
        for k in (1, 30):
            for d in "0123456789":
                inp = f"{k}\n{d}\n"
                a = run(os.path.join(tmp, "std"), inp)
                b = run(os.path.join(tmp, "brute"), inp)
                assert a == b, f"Mismatch single digit k={k} d={d}"

        # 4. 最坏规模计时: k=30, |s|=255
        big = "".join(rng.choice("0123456789") for _ in range(255))
        max_inp = f"30\n{big}\n"
        t0 = time.perf_counter()
        res = run(os.path.join(tmp, "std"), max_inp)
        dt = time.perf_counter() - t0
        print(f"P1538 check passed! official sample + 300 random + 2*10 single-digit + "
              f"k=30,len=255 (lines={len(res.splitlines())}) in {dt:.4f}s")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

if __name__ == "__main__":
    main()
