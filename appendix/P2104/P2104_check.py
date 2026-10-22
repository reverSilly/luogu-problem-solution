import random, subprocess, sys

# 语义: 纯数位串
#   '+'  : 末尾连续的 1 变 0, 再往上一个 0 变 1(无 0 则最高位进位)
#   '-'  : 末尾连续的 0 变 1, 再往上一个 1 变 0(无 1 则最高位退位)
#   '*'  : 末尾添一个 0
#   '/'  : 删掉末尾一位
# 题目保证 '+','-' 不会导致最高位的进位与退位, 即串长不变

def apply_op(s, c):
    if c == '+':
        i = len(s) - 1
        while i >= 0 and s[i] == '1':
            s = s[:i] + '0' + s[i+1:]
            i -= 1
        if i < 0:
            return None          # 最高位进位, 数据不会出现
        return s[:i] + '1' + s[i+1:]
    if c == '-':
        i = len(s) - 1
        while i >= 0 and s[i] == '0':
            s = s[:i] + '1' + s[i+1:]
            i -= 1
        if i < 0:
            return None          # 最高位退位, 数据不会出现
        return s[:i] + '0' + s[i+1:]
    if c == '*':
        return s + '0'
    if c == '/':
        if len(s) <= 1:
            return None          # 删空, 不造这种数据
        return s[:-1]

def gen(rnd):
    n = rnd.randint(1, 20)
    s = "".join(rnd.choice("01") for _ in range(n))
    if rnd.random() < 0.6:
        s = "1" + s[1:]           # 大多数情况是规范表示
    m = rnd.randint(1, 20)
    ops = ''
    cur = s
    while len(ops) < m:
        c = rnd.choice("+-*/")
        nxt = apply_op(cur, c)
        if nxt is None or not nxt:
            continue
        ops += c
        cur = nxt
    return f"{n} {m}\n{s}\n{ops}\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2104"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2104_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
