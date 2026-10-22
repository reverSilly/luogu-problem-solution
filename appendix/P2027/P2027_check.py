#!/usr/bin/env python3
# P2027 对拍：用 Python 参考解释器作为暴力，随机生成小 BF 程序比对输出
import random
import subprocess
import sys

STD = "/tmp/P2027"


def ref_run(code, buf):
    #8 位有符号内存
    mem = [0] * 30000
    ptr = 0
    ip = 0
    #括号匹配
    stk = []
    cl = {}
    op = {}
    for i, c in enumerate(code):
        if c == '[':
            stk.append(i)
        elif c == ']':
            j = stk.pop()
            cl[j] = i
            op[i] = j
    out = []
    pc = 0
    step = 0
    while pc < len(code) and step < 1000000:
        step += 1
        c = code[pc]
        if c == '+':
            mem[ptr] = mem[ptr] + 1 if mem[ptr] < 127 else -128
        elif c == '-':
            mem[ptr] = mem[ptr] - 1 if mem[ptr] > -128 else 127
        elif c == '>':
            ptr += 1
        elif c == '<':
            ptr -= 1
        elif c == ',':
            v = ord(buf[ip]) if ip < len(buf) else -1
            ip += 1
            if v > 127:
                v -= 256
            mem[ptr] = v
        elif c == '.':
            out.append(mem[ptr] & 0xFF)
        elif c == '[':
            if mem[ptr] == 0:
                pc = cl[pc]
        elif c == ']':
            pc = op[pc] - 1
        pc += 1
    return bytes(out)


def main():
    random.seed(20260922)
    cases = 0
    for it in range(300):
        n = random.randint(1, 40)
        prog = []
        depth = 0
        for _ in range(n):
            r = random.random()
            if r < 0.15 and depth == 0:
                prog.append('[')
                depth = 1
                #给循环配一个 ']'
                continue
            if r < 0.3 and depth == 1:
                prog.append(']')
                depth = 0
                continue
            #循环会重复执行，故不生成指针移动指令，保证指针不越界
            prog.append(random.choice('+-.,'))
        while depth:
            prog.append(']')
            depth = 0
        code = ''.join(prog)
        buf = ''.join(random.choice('abcXYZ 019') for _ in range(random.randint(0, 6)))
        raw = (code + ' $ ' + buf + ' $').encode()
        r = subprocess.run([STD], input=raw, capture_output=True, timeout=20)
        got = r.stdout.rstrip(b'\n')
        exp = ref_run(code, buf)
        cases += 1
        if got != exp:
            print("MISMATCH")
            print("code:", repr(code), "buf:", repr(buf))
            print("std:", got)
            print("exp:", exp)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
