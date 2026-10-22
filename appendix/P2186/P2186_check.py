import random, subprocess, sys

# P2186 小Z的栈函数
# 暴力：与标程等价的逐条重放实现（用字符串表匹配操作名、vector 当栈），
#       与标程的整数编号 + 静态数组写法相互独立，用于校验实现一致性
OPS = ["NUM", "POP", "INV", "DUP", "SWP", "ADD", "SUB", "MUL", "DIV", "MOD"]

def gen(rnd):
    k = rnd.randint(1, 12)
    lines = []
    for _ in range(k):
        op = rnd.choice(OPS)
        if op == "NUM":
            lines.append(f"NUM {rnd.randint(-2000000000, 2000000000)}")
        else:
            lines.append(op)
    lines.append("END")
    n = rnd.randint(1, 6)
    xs = [rnd.randint(-10**9, 10**9) for _ in range(n)]
    return "\n".join(lines) + "\n" + str(n) + "\n" + "\n".join(str(x) for x in xs) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 1000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2186"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2186_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
