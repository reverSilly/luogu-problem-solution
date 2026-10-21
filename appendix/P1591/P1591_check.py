import random, subprocess, sys, math

random.seed(11)
for it in range(150):
    t = random.randint(1, 10)
    cases = [(random.randint(1, 1000), random.randint(0, 9)) for _ in range(t)]
    if it == 0:
        cases = [(1, 1), (5, 0), (10, 0), (1000, 0), (1000, 3)]
    t = len(cases)
    data = f"{t}\n" + "".join(f"{n} {a}\n" for n, a in cases)
    truth = "".join(str(str(math.factorial(n)).count(str(a))) + "\n" for n, a in cases)
    a1 = subprocess.run(["/tmp/P1591"], input=data, capture_output=True, text=True).stdout
    a2 = subprocess.run(["/tmp/P1591_brute"], input=data, capture_output=True, text=True).stdout
    if a1 != truth or a2 != truth:
        print("MISMATCH", data, a1, truth, a2)
        sys.exit(1)
print("all 150 cases ok (vs math.factorial)")
