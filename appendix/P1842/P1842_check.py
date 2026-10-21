#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1842"
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
            print(r.stderr); sys.exit(1)

def run(bin,data,tl=20):
    r=subprocess.run([bin],input=data.encode(),capture_output=True,timeout=tl)
    if r.returncode!=0:
        print("runtime",bin); sys.exit(1)
    return r.stdout.decode()

def pack(cows):
    lines=[str(len(cows))]
    for w,s in cows:
        lines.append(f"{w} {s}")
    return "\n".join(lines)+"\n"

def main():
    compile_all()
    rng=random.Random(1842)
    cases=[]
    fixed=[
        [(10,3),(2,5),(3,3)],
        [(5,10)],
        [(1,1)],
        [(1,1000000000)],
        [(10000,1),(10000,1)],
        [(4,1),(1,100),(2,2),(8,3)],
        [(9,9),(1,1),(1,2)],
    ]
    for c in fixed:
        cases.append(pack(c))
    for _ in range(120):
        n=rng.randint(1,7)
        cows=[]
        for _i in range(n):
            w=rng.randint(1,30)
            s=rng.choice([1,2,5,rng.randint(1,50),10**9])
            cows.append((w,s))
        cases.append(pack(cows))
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data); print(a,b); sys.exit(1)
    if norm(run(STD,cases[0]))!="2":
        print("sample",run(STD,cases[0])); sys.exit(1)
    if norm(run(STD,cases[1]))!="-10":
        print("neg"); sys.exit(1)
    rng=random.Random(2)
    n=50000
    lines=[str(n)]
    for _ in range(n):
        lines.append(f"{rng.randint(1,10000)} {rng.randint(1,10**9)}")
    data="\n".join(lines)+"\n"
    t0=time.perf_counter()
    out=run(STD,data)
    dt=time.perf_counter()-t0
    if not out.strip().lstrip('-').isdigit():
        print("bad max"); sys.exit(1)
    print(f"MAX n=5e4 time={dt:.3f}s")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
