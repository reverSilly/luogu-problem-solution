import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1537.cpp", "-o", "P1537"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1537_brute.cpp", "-o", "P1537_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout

def main():
    compile_all()
    # 1. 题面样例与边界
    sample = "1 0 1 2 0 0\n1 0 0 0 1 1\n0 0 0 0 0 0\n"
    assert run("P1537", sample) == run("P1537_brute", sample)

    # 2. 随机对拍 500 组
    rng = random.Random(20261018)
    lines = []
    for _ in range(500):
        arr = [rng.randint(0, 30) for _ in range(6)]
        if sum(arr) == 0:
            arr[0] = 2
        lines.append(" ".join(map(str, arr)))
    lines.append("0 0 0 0 0 0")
    batch_inp = "\n".join(lines) + "\n"
    assert run("P1537", batch_inp) == run("P1537_brute", batch_inp)

    # 3. 极限性能测试 sum(N_i) = 20000
    max_lines = []
    for _ in range(20):
        arr = [3333, 3333, 3333, 3333, 3334, 3334]
        max_lines.append(" ".join(map(str, arr)))
    max_lines.append("0 0 0 0 0 0")
    t0 = time.perf_counter()
    run("P1537", "\n".join(max_lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1537 check passed! 500 random cases + 20 max-size cases in {dt:.4f}s")

if __name__ == "__main__":
    main()
