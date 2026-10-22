import random, subprocess, sys

small = ["zero","one","two","three","four","five","six","seven","eight","nine","ten",
         "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"]
tens = ["twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"]

def under100(x):
    if x < 20:
        return small[x]
    s = tens[x // 10 - 2]
    if x % 10:
        s += " " + small[x % 10]
    return s

def under1000(x):
    if x < 100:
        return under100(x)
    s = small[x // 100] + " hundred"
    if x % 100:
        s += " " + under100(x % 100)
    return s

def gen(rnd):
    x = rnd.randint(-999999999, 999999999)
    neg = x < 0
    x = abs(x)
    if x == 0:
        words = ["zero"]
    else:
        words = []
        if x >= 1000000:
            words.append(under1000(x // 1000000))
            words.append("million")
            x %= 1000000
        if x >= 1000:
            words.append(under1000(x // 1000))
            words.append("thousand")
            x %= 1000
        if x:
            words.append(under1000(x))
        if not words:
            words = ["zero"]
    if neg:
        words = ["negative"] + words
    return " ".join(words) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2108"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2108_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH", inp.strip(), "std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
