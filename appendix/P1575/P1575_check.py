import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1575.cpp", "-o", "P1575"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1575_brute.cpp", "-o", "P1575_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 经典样例与 Hack 边界测试
    cases = [
        ("true or false and false", "true"),
        ("not true or false", "false"),
        ("not not true and true", "true"),
        ("true not false", "error"),
        ("true and", "error"),
        ("or true", "error"),
        ("not", "error"),
        ("true false", "error"),
        ("true and or false", "error"),
        ("not not not false", "true"),
    ]
    for expr, expected in cases:
        assert run("P1575", expr + "\n") == expected
        assert run("P1575_brute", expr + "\n") == expected

    # 2. 随机合法与非法表达式对拍 500 组
    rng = random.Random(20261021)
    vocab = ["true", "false", "and", "or", "not"]
    for _ in range(500):
        k = rng.randint(1, 25)
        expr = " ".join(rng.choices(vocab, k=k))
        assert run("P1575", expr + "\n") == run("P1575_brute", expr + "\n")

    print("P1575 check passed! 10 boundary/hack cases + 500 random expressions verified")

if __name__ == "__main__":
    main()
