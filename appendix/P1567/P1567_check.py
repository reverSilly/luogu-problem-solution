import subprocess, random, time

BINDIR = "/tmp"

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1567.cpp", "-o", f"{BINDIR}/P1567"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1567_brute.cpp", "-o", f"{BINDIR}/P1567_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"{BINDIR}/{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 0. 官方样例 -> 5
    samp = "10\n1 2 3 2 4 5 6 8 5 9\n"
    r = run("P1567", samp)
    assert r == "5", f"sample fail: got {r}"

    # 1. 随机小规模对拍
    for tc in range(400):
        n = rng.randint(1, 200)
        arr = [rng.randint(0, 20) for _ in range(n)]
        inp = f"{n}\n" + " ".join(map(str, arr)) + "\n"
        a = run("P1567", inp)
        b = run("P1567_brute", inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}, bf={b}"

    # 2. 最坏规模计时 N=1e6 严格上升
    n = 1000000
    inp = f"{n}\n" + " ".join(map(str, range(n))) + "\n"
    t0 = time.perf_counter()
    res = run("P1567", inp)
    dt = time.perf_counter() - t0

    print(f"P1567 PASS: sample=5, 400 random cases ok; worst N=1e6 strictly-increasing ans={res} (expect 1000000) in {dt:.4f}s")

if __name__ == "__main__":
    main()
