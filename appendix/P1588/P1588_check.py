import random, subprocess, sys

# P1588 对拍：多组小数据 + 边界
for it in range(200):
    t = random.randint(1, 10)
    lines = [str(t)]
    for _ in range(t):
        x = random.randint(1, 300)
        y = random.randint(1, 300)
        lines.append(f"{x} {y}")
    # 固定边界
    if it == 0:
        lines = ["3", "1 1", "1 2", "2 1"]
    if it == 1:
        lines = ["2", "1 100000", "100000 1"]
    data = "\n".join(lines) + "\n"
    a = subprocess.run(["/tmp/P1588"], input=data, capture_output=True, text=True).stdout
    b = subprocess.run(["/tmp/P1588_brute"], input=data, capture_output=True, text=True).stdout
    if a.split() != b.split():
        print("MISMATCH", data, a, b)
        sys.exit(1)
print("all 200 cases ok")
