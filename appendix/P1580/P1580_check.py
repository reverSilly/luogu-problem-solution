import os, random, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)

STD, BF = "P1580", "P1580_brute"
CXX = ["g++", "-O2", "-std=c++17"]

# ---------------------------------------------------------------- 官方样例
# 取自洛谷题面页 https://www.luogu.com.cn/problem/P1580 ,逐行照抄(含行尾空格)
_ROT = ["yyy loves OI", "yyy loves Maths", "yyy loves Chemistry"]
_VALID = "@yyy loves Physics wo pei fu ni de chu ti xiao lv"

S1_IN = "\n".join([
    f"{_ROT[0]} : {_VALID}",
    f"{_ROT[1]} : {_VALID}",
    f"{_ROT[2]} : {_VALID}",
    "yyy loves Microelectronic : ni men wu liao me ",     # 官方样例行尾有一个空格
    f"{_ROT[0]} : {_VALID}",
    "",
]) + "\n"
S1_OUT = "Unsuccessful @yyy loves Physics attempt\n4\nyyy loves Microelectronic\n"

S2_IN = "\n".join(
    [f"{_ROT[i % 3]} : {_VALID}" for i in range(9)]
    + ["yyy loves Physics : ni men gou le ", ""]           # 被炸的人亲自发言(无 @)
) + "\n"
S2_OUT = "Successful @yyy loves Physics attempt\n"

# ---------------------------------------------------------------- 基础设施
def compile_all():
    subprocess.run(CXX + ["P1580.cpp", "-o", STD], check=True)
    subprocess.run(CXX + ["P1580_brute.cpp", "-o", BF], check=True)

def run(binary, inp):
    p = subprocess.run([f"./{binary}"], input=inp, text=True, capture_output=True)
    if p.returncode != 0:
        print(f"[FAIL] {binary} 异常退出 rc={p.returncode}")
        print("----- input -----"); print(inp)
        print("----- stderr -----"); print(p.stderr)
        sys.exit(1)
    return p.stdout

def die(inp, a, b, tag):
    print(f"[FAIL] {tag}: 标程与暴力输出不一致")
    print("----- input -----"); print(inp)
    print("----- P1580 -----"); print(a)
    print("----- P1580_brute -----"); print(b)
    sys.exit(1)

def check_expect(inp, exp, tag):
    for binary in (STD, BF):
        out = run(binary, inp)
        if out != exp:
            print(f"[FAIL] {tag}: {binary} 输出与期望不符")
            print("----- input -----"); print(inp)
            print("----- got -----"); print(out)
            print("----- expect -----"); print(exp)
            sys.exit(1)

def duel(inp, tag):
    a, b = run(STD, inp), run(BF, inp)
    if a != b:
        die(inp, a, b, tag)
    return a

# ---------------------------------------------------------------- 随机生成
BASE = ["yyy loves OI", "yyy loves Maths", "yyy loves Chemistry",
        "yyy loves Physics", "yyy loves Microelectronic"]
EXTRA = ["yyy loves Biology", "yyy loves Chinese", "yyy loves English",
         "yyy loves History", "yyy loves Geography"]
PADS = ["wo pei fu ni de chu ti xiao lv", "+1", "ni men wu liao me", "hhhh",
        "123", "soha nb", "a b c", "ni men gou le", ""]

def rand_pad(rng):
    r = rng.random()
    if r < 0.35:
        return ""
    w = rng.choice(PADS)
    if r < 0.85:
        return " " + w
    return " " + w + " " * rng.randint(1, 3)          # 尾随空白(官方样例里就有)

def gen_case(rng, max_n=60):
    pool = BASE + rng.sample(EXTRA, rng.randint(0, 3))
    tgt = rng.choice(pool)
    n = rng.randint(1, rng.choice([1, 3, 8, 25, max_n]))
    good_p = rng.choice([0.2, 0.45, 0.7, 0.9, 0.98])
    # 第一句:保证恰一个 @、艾特别人、不艾特自己(题面三条保证)
    lines = [f"{rng.choice([x for x in pool if x != tgt])} : @{tgt}{rand_pad(rng)}"]
    for _ in range(n - 1):
        sdr = rng.choice(pool)
        if rng.random() < good_p:
            content = f"@{tgt}{rand_pad(rng)}"                       # 维持队形
        else:
            kind = rng.randint(0, 5)
            if kind == 0:
                content = rand_pad(rng).lstrip(" ") or "ni men wu liao me"   # 零个 @
            elif kind == 1:
                content = f"@{rng.choice([x for x in pool if x != tgt])}{rand_pad(rng)}"  # 艾特别人
            elif kind == 2:
                content = f"@{tgt} @{tgt}{rand_pad(rng)}"            # 反复艾特同一人
            elif kind == 3:
                content = f"@{tgt} @{rng.choice(pool)}{rand_pad(rng)}"        # 一句艾特两人
            elif kind == 4:
                content = f"@{tgt} @{tgt} @{rng.choice(pool)}"        # 三个 @
            else:
                content = f"@{tgt}{rng.choice(['+1', ',', 'x'])}"     # 名字粘连无空格
        lines.append(f"{sdr} : {content}")
    return "\n".join(lines) + "\n\n"          # 题面保证:文件以一行空行结尾

# ---------------------------------------------------------------- 各测试段
def sample_regression():
    check_expect(S1_IN, S1_OUT, "官方样例1")
    check_expect(S2_IN, S2_OUT, "官方样例2")
    print("样例回归: 官方样例 2/2 通过(标程与暴力均逐字节命中期望输出)")

def boundary_tests():
    # 1) 只有第一句:从未破队形,队形长度 1
    check_expect("yyy loves OI : @yyy loves Physics\n\n",
                 "Unsuccessful @yyy loves Physics attempt\n1\nGood Queue Shape\n",
                 "边界1-单句")
    # 2) 被炸的人中途合法发言(艾特自己)→轰炸成功,队形其实没破
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Maths : @yyy loves Physics\n"
                 "yyy loves Physics : @yyy loves Physics\n"
                 "yyy loves OI : @yyy loves Physics\n\n",
                 "Successful @yyy loves Physics attempt\n",
                 "边界2-目标合法发言")
    # 3) 被炸的人在破队形之后才发言 → 仍算失败(只认第一次破坏)
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Maths : @yyy loves Physics\n"
                 "yyy loves Chemistry : ni men wu liao me\n"
                 "yyy loves Physics : ni men gou le\n\n",
                 "Unsuccessful @yyy loves Physics attempt\n3\nyyy loves Chemistry\n",
                 "边界3-目标破后才发言")
    # 4) 被炸的人正是破坏者(第 2 句零个 @)→轰炸成功
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Physics : ni men gou le\n\n",
                 "Successful @yyy loves Physics attempt\n",
                 "边界4-目标即破坏者")
    # 5) 第 2 句反复艾特同一人(两个 @)→第一次破坏行号为 2
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Maths : @yyy loves Physics @yyy loves Physics\n"
                 "yyy loves OI : @yyy loves Physics\n\n",
                 "Unsuccessful @yyy loves Physics attempt\n2\nyyy loves Maths\n",
                 "边界5-重复艾特")
    # 6) 91 句长队形始终未破、被炸的人始终不说话(题面 Hint 里的真实队形长度)
    lines = [f"{_ROT[i % 3]} : {_VALID}" for i in range(91)]
    check_expect("\n".join(lines) + "\n\n",
                 "Unsuccessful @yyy loves Physics attempt\n91\nGood Queue Shape\n",
                 "边界6-91行队形")
    # 7) CRLF 行尾(评测机历史上出现过 \r\n)
    check_expect(S1_IN.replace("\n", "\r\n"), S1_OUT, "边界7-CRLF")
    # 8) 没有结尾空行,仅靠 EOF 结束
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Maths : @yyy loves Physics\n",
                 "Unsuccessful @yyy loves Physics attempt\n2\nGood Queue Shape\n",
                 "边界8-无尾空行")
    # 9) 名字与后文粘连(无空格)→视为艾特了别人
    check_expect("yyy loves OI : @yyy loves Physics\n"
                 "yyy loves Maths : @yyy loves Physics+1\n\n",
                 "Unsuccessful @yyy loves Physics attempt\n2\nyyy loves Maths\n",
                 "边界9-名字粘连")
    print("边界用例: 9/9 通过")

def random_duel(total=5000):
    rng = random.Random(20261020)
    for tc in range(total):
        inp = gen_case(rng)
        duel(inp, f"随机第 {tc + 1} 组")
    print(f"随机对拍: {total} 组随机数据全部 std==brute")

def medium_duel():
    # 中大规模:长队形 + 各种结局,进一步扫逻辑分支
    rng = random.Random(20261021)
    for tc in range(20):
        inp = gen_case(rng, max_n=rng.choice([300, 1500, 3000]))
        duel(inp, f"中规模第 {tc + 1} 组")
    print("中规模对拍: 20 组(n 最大 3000)全部 std==brute")

def worst_case_timing():
    # 题面:行数 <=5e4,行长 <=1e3,文件 <=4MB。取满 5e4 行、每行约 78 字符 ≈3.95MB
    # 全部是合法队形句且被炸的人始终不发言 → 两个程序都必须扫完整个文件(工作量最大)
    pad = "a" * 40
    inp = "\n".join([f"yyy loves Maths : @yyy loves Physics {pad}" for _ in range(50000)]) + "\n\n"
    size = len(inp.encode())
    assert size <= 4 * 1024 * 1024, f"输入超过 4MB: {size}"
    t0 = time.perf_counter()
    out = run(STD, inp)
    dt = time.perf_counter() - t0
    duel(inp, "最坏规模")
    qlen = out.splitlines()[1]
    assert qlen == "50000", qlen
    print(f"最坏规模计时: 50000 行 / {size / 1048576:.2f}MB,队形未破 → 标程耗时 {dt:.4f}s(输出队形长度 {qlen})")

def main():
    compile_all()
    sample_regression()
    boundary_tests()
    random_duel(5000)
    medium_duel()
    worst_case_timing()
    print("P1580 check passed!")

if __name__ == "__main__":
    main()
