import subprocess, sys

# P1617 校验：与 Python 独立实现逐值比对 0..9999 全部 10000 个数
ONES = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
TEENS = ["ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"]
TENS = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"]

def two(x):
    if x < 10:
        return ONES[x]
    if x < 20:
        return TEENS[x - 10]
    if x % 10 == 0:
        return TENS[x // 10]
    return TENS[x // 10] + "-" + ONES[x % 10]

def say(n):
    tail = n % 100
    n //= 100
    hundred = n % 10
    thousand = n // 10 % 10
    s = ""
    if thousand:
        s += ONES[thousand] + " thousand"
    if hundred:
        s += ("" if not s else " ") + ONES[hundred] + " hundred"
    if tail:
        if s:
            s += " and " if tail <= 10 else " "
        s += two(tail)
    return s if s else "zero"

# 手工核对过的边界值（先自检 Python 实现）
manual = {0: "zero", 7: "seven", 10: "ten", 11: "eleven", 20: "twenty", 21: "twenty-one",
          100: "one hundred", 101: "one hundred and one", 110: "one hundred and ten",
          111: "one hundred eleven", 121: "one hundred twenty-one", 999: "nine hundred ninety-nine",
          1000: "one thousand", 1001: "one thousand and one", 1010: "one thousand and ten",
          1011: "one thousand eleven", 1100: "one thousand one hundred",
          1101: "one thousand one hundred and one", 1234: "one thousand two hundred thirty-four",
          9999: "nine thousand nine hundred ninety-nine"}
for k, v in manual.items():
    if say(k) != v:
        print("PY-SELF MISMATCH", k, say(k), v)
        sys.exit(1)

bad = 0
for n in range(10000):
    p = subprocess.run(["/tmp/P1617"], input=f"{n}\n", capture_output=True, text=True)
    got = p.stdout.strip()
    exp = say(n)
    if got != exp:
        print("MISMATCH", n, repr(got), repr(exp))
        bad += 1
        if bad > 5:
            sys.exit(1)
if bad:
    sys.exit(1)
print("P1617 OK: 10000 cases (0..9999 全量)")
