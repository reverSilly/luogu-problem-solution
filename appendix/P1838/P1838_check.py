#!/usr/bin/env python3
import random, subprocess, sys
PID="P1838"
ROOT="/tmp/luogu-ps"
STD_SRC=f"{ROOT}/appendix/{PID}/{PID}.cpp"
BRU_SRC=f"{ROOT}/appendix/{PID}/{PID}_brute.cpp"
STD=f"/tmp/{PID}"
BRU=f"/tmp/{PID}_brute"

def norm(s):
    return "\n".join(ln.rstrip() for ln in s.strip().splitlines())

def compile_all():
    for src,dst in ((STD_SRC,STD),(BRU_SRC,BRU)):
        r=subprocess.run(["g++","-O2","-std=c++17","-Wall","-Wextra","-Werror","-o",dst,src],capture_output=True,text=True)
        if r.returncode!=0:
            print(r.stderr)
            sys.exit(1)

def run(bin,data):
    r=subprocess.run([bin],input=data.encode(),capture_output=True,timeout=10)
    if r.returncode!=0:
        print("runtime",bin,r.returncode)
        sys.exit(1)
    return r.stdout.decode()

def check(data,idx):
    a,b=run(STD,data),run(BRU,data)
    if norm(a)!=norm(b):
        print("MISMATCH",idx)
        print(data)
        print("std:",a)
        print("brute:",b)
        sys.exit(1)

def main():
    compile_all()
    rng=random.Random(1838)
    samples=[
        "5237649\n",
        "539128647\n",
        "142596\n",
        "5\n",
        "123\n",
        "12\n",
        "147\n",
    ]
    cases=list(samples)
    cells=[str(i) for i in range(1,10)]
    for L in range(1,10):
        row=cells[:]
        rng.shuffle(row)
        cases.append("".join(row[:L])+"\n")
    for _ in range(180):
        row=cells[:]
        rng.shuffle(row)
        L=rng.randint(1,9)
        cases.append("".join(row[:L])+"\n")
    for i,data in enumerate(cases):
        check(data,i)
    # fixed expected
    exp={
        "5237649\n":"xiaoa wins.",
        "539128647\n":"drew.",
        "142596\n":"uim wins.",
        "147\n":"drew.",
        "5\n":"drew.",
        "12\n":"drew.",
    }
    for data,e in exp.items():
        if norm(run(STD,data))!=e:
            print("sample fail",data,run(STD,data))
            sys.exit(1)
    print(f"{len(cases)} tests OK")
    return 0

if __name__=="__main__":
    sys.exit(main())
