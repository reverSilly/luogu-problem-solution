import subprocess, sys, math

# n<=9: 与全排列枚举对拍; n=1..20: 与容斥式 D(n)=sum_{k=0..n} (-1)^k n!/k! 对拍
for n in range(1, 21):
    data = f"{n}\n"
    a = subprocess.run(["/tmp/P1595"], input=data, capture_output=True, text=True).stdout.split()
    fact = math.factorial(n)
    d = sum((-1) ** k * (fact // math.factorial(k)) for k in range(n + 1))
    if a != [str(d)]:
        print("MISMATCH vs inclusion-exclusion", n, a, d)
        sys.exit(1)
    if n <= 9:
        b = subprocess.run(["/tmp/P1595_brute"], input=data, capture_output=True, text=True).stdout.split()
        if a != b:
            print("MISMATCH vs brute", n, a, b)
            sys.exit(1)
print("n=1..20 ok (n=1..9 also vs permutation enumeration)")
