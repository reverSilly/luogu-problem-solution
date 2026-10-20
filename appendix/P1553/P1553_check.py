import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1553.cpp", "-o", "P1553"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1553_brute.cpp", "-o", "P1553_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

# 与规则独立书写的 Python 参考实现(第三路交叉验证)
def revint(t):
    s = t[::-1].lstrip('0')
    return s if s else "0"

def revdec(t):
    s = t[::-1].rstrip('0')
    return s if s else "0"

def ref(s):
    if s.endswith('%'):
        return revint(s[:-1]) + '%'
    if '.' in s:
        p = s.index('.')
        return revint(s[:p]) + '.' + revdec(s[p + 1:])
    if '/' in s:
        p = s.index('/')
        return revint(s[:p]) + '/' + revint(s[p + 1:])
    return revint(s)

def chk(case):
    exp = ref(case)
    a = run("P1553", case + "\n")
    b = run("P1553_brute", case + "\n")
    assert a == b == exp, f"case={case} std={a} brute={b} ref={exp}"
    return a

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 手工/官方样例边界
    cases = {
        "1234567": "7654321",
        "100": "1",
        "1000": "1",
        "0": "0",
        "100.0": "1.0",
        "0.0": "0.0",
        "0.05": "0.5",
        "7/30": "7/3",
        "1/10": "1/1",
        "50%": "5%",
        "100%": "1%",
        "3.1415": "3.5141",
        "0.5": "0.5",
        "10/100": "1/1",
    }
    for case, exp in cases.items():
        a = run("P1553", case + "\n")
        b = run("P1553_brute", case + "\n")
        assert a == b == exp, f"case={case} std={a} brute={b} exp={exp}"

    # 2. 随机数据: 四种类型各覆盖, std == brute == ref 三路一致
    def digits(maxlen):
        L = rng.randint(1, maxlen)
        return ''.join(rng.choice('0123456789') for _ in range(L))

    def std_int(maxlen):
        L = rng.randint(1, maxlen)
        return str(rng.randint(1, 9)) + ''.join(rng.choice('0123456789') for _ in range(L - 1))

    def dec_part(maxlen):
        if rng.random() < 0.15:
            return "0"
        L = rng.randint(1, maxlen)
        return ''.join(rng.choice('0123456789') for _ in range(L - 1)) + rng.choice('123456789')

    n_total = 0
    for tc in range(900):
        kind = tc % 4
        if kind == 0:                                  # 整数
            if rng.random() < 0.1:
                s = "0"
            else:
                s = std_int(20)
            if rng.random() < 0.1:
                s = '0' * rng.randint(1, 2) + s
        elif kind == 1:                                # 小数
            s = std_int(10) + '.' + dec_part(10)
        elif kind == 2:                                # 分数
            s = std_int(10) + '/' + std_int(10)
        else:                                          # 百分数
            s = std_int(19) + '%'
        chk(s)
        n_total += 1

    # 3. 最坏规模计时(最长整数 / 小数 / 分数 / 百分数)
    worst = [
        "9" * 20,
        std_int(10) + '.' + dec_part(10),
        std_int(10) + '/' + std_int(10),
        std_int(19) + '%',
    ]
    t0 = time.perf_counter()
    for _ in range(200):
        for s in worst:
            run("P1553", s + "\n")
    dt = time.perf_counter() - t0

    print(f"P1553 check PASS! {len(cases)} manual cases + {n_total} random cases + worst-size timing "
          f"(200 runs x 4 shapes) in {dt:.4f}s")

if __name__ == "__main__":
    main()
