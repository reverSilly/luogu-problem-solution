import random

def brute(s):
    n = len(s)
    k = n
    while True:
        seen = set()
        ok = True
        for x in s:
            r = x % k
            if r in seen:
                ok = False
                break
            seen.add(r)
        if ok:
            return k
        k += 1

def fast(s):
    n = len(s)
    D = max(s) - min(s)
    dif = set()
    for i in range(n):
        for j in range(i + 1, n):
            dif.add(abs(s[i] - s[j]))
    k = n
    while True:
        ok = True
        for m in range(k, D + 1, k):
            if m in dif:
                ok = False
                break
        if ok:
            return k
        k += 1

for test in range(5000):
    n = random.randint(1, 12)
    s = random.sample(range(1, 200), n)
    b = brute(s)
    f = fast(s)
    if b != f:
        print("MISMATCH", s, b, f)
        break
else:
    print("5000 random cases pass")
