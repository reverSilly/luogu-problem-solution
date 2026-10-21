import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1593.cpp", "-o", "P1593"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1593_brute.cpp", "-o", "P1593_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 官方样例与 p % 9901 == 1 / p % 9901 == 0 边界测试
    cases = [
        (2, 3),
        (1, 0),
        (50000000, 0),
        (9901, 5),            # p % 9901 == 0
        (9902, 100),          # p % 9901 == 1 (无逆元退化为 k+1)
        (19803, 50),          # 2 * 9901 + 1
    ]
    for a, b in cases:
        inp = f"{a} {b}\n"
        assert run("P1593", inp) == run("P1593_brute", inp)

    # 2. 随机对拍 300 组
    rng = random.Random(20261021)
    for _ in range(300):
        a = rng.randint(1, 50000000)
        b = rng.randint(0, 50000000)
        inp = f"{a} {b}\n"
        assert run("P1593", inp) == run("P1593_brute", inp)

    print("P1593 check passed! Boundary (p=9901, 9902, b=0) + 300 random cases verified")

if __name__ == "__main__":
    main()
