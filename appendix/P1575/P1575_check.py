import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1575.cpp", "-o", "P1575"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1575_brute.cpp", "-o", "P1575_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def py_eval(tokens):
    """用 Python eval 作为第三opinion：任何异常都视为 error。"""
    if not tokens:
        return "error"
    expr = " ".join(tokens)
    for a, b in [("true", "True"), ("false", "False")]:
        expr = expr.replace(a, b)
    try:
        r = eval(expr, {"__builtins__": {}})
    except Exception:
        return "error"
    return "true" if r else "false"

def gen_valid(rng, depth):
    if depth <= 0 or rng.random() < 0.3:
        return [rng.choice(["true", "false"])]
    kind = rng.random()
    if kind < 0.33:
        return ["not"] + gen_valid(rng, depth - 1)
    if kind < 0.66:
        n = rng.randint(2, 3)
        toks = []
        for i in range(n):
            if i:
                toks.append("and")
            toks += gen_valid(rng, depth - 1)
        return toks
    n = rng.randint(2, 3)
    toks = []
    for i in range(n):
        if i:
            toks.append("or")
        toks += gen_valid(rng, depth - 1)
    return toks

def main():
    compile_all()
    rng = random.Random(20261018)

    pool = ["true", "false", "and", "or", "not"]

    # 1. 随机乱序记号流(大量 error) + 合法表达式，共 300 组
    for tc in range(300):
        if tc % 3 == 2:
            tokens = gen_valid(rng, 4)
        else:
            k = rng.randint(0, 13)
            tokens = [rng.choice(pool) for _ in range(k)]
        inp = (" ".join(tokens)) + "\n"
        out_std = run("P1575", inp)
        out_bf = run("P1575_brute", inp)
        assert out_std == out_bf, f"Mismatch on {tokens!r}: std={out_std}, bf={out_bf}"
        assert out_std == py_eval(tokens), f"Python mismatch on {tokens!r}: std={out_std}, py={py_eval(tokens)}"

    # 2. 手工关键用例(含 Pascal 优先级 not>and>or、not 连缀、错误串)
    cases = [
        "true", "false", "not true", "not false", "not not true",
        "true and false", "true or false", "not true and false",
        "true and not false", "true and true or false and false",
        "true or false and false", "not true or true",
        "", "not", "and true", "true and", "true true",
        "true and and false", "not or true", "or", "not not not true",
        "true and true and true or false",
    ]
    for c in cases:
        inp = c + "\n"
        out_std = run("P1575", inp)
        out_bf = run("P1575_brute", inp)
        assert out_std == out_bf, f"Mismatch on {c!r}: std={out_std}, bf={out_bf}"
        toks = c.split()
        assert out_std == py_eval(toks), f"Python mismatch on {c!r}: std={out_std}, py={py_eval(toks)}"

    # 3. 最坏规模计时：接近 255 字符的长合法表达式
    long_expr = " or ".join(["not not true and false" for _ in range(12)])
    t0 = time.perf_counter()
    res = run("P1575", long_expr + "\n")
    dt = time.perf_counter() - t0
    assert res == py_eval(long_expr.split())

    print(f"P1575 check passed! 300 random + 22 handcrafted cases std==brute==python; "
          f"long valid expr -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
