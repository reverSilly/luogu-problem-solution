import random, subprocess, sys, re

random.seed(19)

def truth(prog):
    v = {'a': 0, 'b': 0, 'c': 0}
    for lhs, rhs in re.findall(r'([abc]):=([abc0-9]);', prog):
        v[lhs] = int(rhs) if rhs.isdigit() else v[rhs]
    return f"{v['a']} {v['b']} {v['c']}"

cases = ["a:=1;", "b:=a;", "a:=3;b:=a;c:=b;"]
for _ in range(200):
    k = random.randint(1, 25)
    stmts = []
    for _ in range(k):
        lhs = random.choice("abc")
        rhs = random.choice("abc0123456789")
        stmts.append(f"{lhs}:={rhs};")
    prog = "".join(stmts)
    if random.random() < 0.5:
        # 随机插入换行(题面允许多行), 不破坏语句
        out, buf = [], ""
        for ch in prog:
            buf += ch
            if ch == ';' and random.random() < 0.4:
                out.append(buf)
                buf = ""
        out.append(buf)
        prog = "\n".join(out)
    cases.append(prog)

for prog in cases:
    data = prog if prog.endswith("\n") else prog + "\n"
    a = subprocess.run(["/tmp/P1597"], input=data, capture_output=True, text=True).stdout.strip()
    b = subprocess.run(["/tmp/P1597_brute"], input=data, capture_output=True, text=True).stdout.strip()
    t = truth(prog)
    if a != b or a != t:
        print("MISMATCH", repr(prog), a, b, t)
        sys.exit(1)
print(f"all {len(cases)} cases ok")
