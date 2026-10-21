import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout

def isprime(n):
    if n < 2:
        return False
    d = 2
    while d * d <= n:
        if n % d == 0:
            return False
        d += 1
    return True

JUNK = list(" \t-.,;:!@#%^&*()[]{}~`'\"\\|/<>?=_+alsdjfkghqwertyuiopzxcvbnm")
JUNK = [c for c in JUNK if not c.isdigit()]

def messy(n):
    s = str(n)
    out = []
    if random.random() < 0.3:
        out.append("0" * random.randint(1, 3))   # 允许前导零
    for ch in s:
        if random.random() < 0.6:
            out.append("".join(random.choice(JUNK) for _ in range(random.randint(0, 3))))
        out.append(ch)
    if random.random() < 0.5:
        out.append("".join(random.choice(JUNK) for _ in range(random.randint(0, 3))))
    return "".join(out).replace("\n", " ") or "x" + s

def check_semantics(out, nums):
    lines = out.split("\n")
    i = 0
    blocks = []
    while i < len(lines):
        if lines[i] == "Enter the number=":
            j = i + 1
            blk = []
            while j < len(lines) and lines[j] != "" and lines[j] != "Enter the number=":
                blk.append(lines[j])
                j += 1
            blocks.append(blk)
            i = j
        else:
            i += 1
    if len(blocks) != len(nums):
        return f"block count {len(blocks)} != {len(nums)}"
    for blk, n in zip(blocks, nums):
        if not blk:
            return f"empty block for {n}"
        head = blk[0]
        if n < 2:
            if head != "Prime? No!" or len(blk) != 1:
                return f"n<2 wrong: {n} {blk}"
            continue
        if n > 40000000:
            if head != "Prime? No!" or blk[1:] != ["The number is too large!"]:
                return f"too large wrong: {n} {blk}"
            continue
        if head == "Prime? Yes!":
            if not isprime(n) or len(blk) != 1:
                return f"claimed prime but is not: {n}"
            continue
        if head != "Prime? No!" or len(blk) != 2:
            return f"head wrong: {n} {blk}"
        eq = blk[1]
        left, _, right = eq.partition("=")
        if left != str(n):
            return f"left not n: {eq}"
        prod, facs = 1, []
        for term in right.split("*"):
            b, _, e = term.partition("^")
            b = int(b)
            e = int(e) if e else 1
            if not isprime(b):
                return f"nonprime factor {b} in {eq}"
            prod *= b ** e
            facs.append((b, e))
        if prod != n:
            return f"product mismatch: {eq} -> {prod} != {n}"
        if [f[0] for f in facs] != sorted(f[0] for f in facs) or len(set(f[0] for f in facs)) != len(facs):
            return f"factors not increasing/distinct: {eq}"
    return None

random.seed(1619)
pool = [0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 25, 49, 97, 100, 101, 121, 1000, 1024, 4096, 65536,
        999983, 1000000, 39999989, 40000000, 40000001, 99999999, 123456789, 2 ** 31,
        998244353, 999999937]
N = 120
for it in range(N):
    nums = []
    k = random.randint(1, 6)
    for _ in range(k):
        if random.random() < 0.45:
            nums.append(random.choice(pool))
        elif random.random() < 0.8:
            nums.append(random.randint(0, 40000000))
        else:
            nums.append(random.randint(0, 200))
    data = "\n".join(messy(n) for n in nums) + "\n"
    if random.random() < 0.7:
        data += "no digits in this line\n"
    a = run("/tmp/P1619", data)
    b = run("/tmp/P1619_brute", data)
    if a != b:
        print("MISMATCH on", repr(data))
        print("--std--\n" + a)
        print("--brute--\n" + b)
        sys.exit(1)
    err = check_semantics(a, nums)
    if err:
        print("SEMANTIC FAIL:", err, repr(data))
        sys.exit(1)
print(f"P1619 OK: {N} files, 正解=朴素解 且 数学语义（素性/分解乘积）全部校验通过")
