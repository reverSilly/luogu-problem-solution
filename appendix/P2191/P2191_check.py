import random, subprocess, sys

# P2191 小Z的情书
# 暴力：只旋转透明纸（字母纸固定），顺时针 90 度，读四遍
def gen(rnd):
    n = rnd.randint(1, 12)
    rows = []
    for _ in range(n):
        r = "".join(rnd.choice("O#") for _ in range(n))
        rows.append(r)
    txt = []
    for _ in range(n):
        txt.append("".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(n)))
    return f"{n}\n" + "\n".join(rows) + "\n" + "\n".join(txt) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2191"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2191_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
