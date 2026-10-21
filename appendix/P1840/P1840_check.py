#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1840"
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

def run(bin,data,tl=30):
    r=subprocess.run([bin],input=data.encode(),capture_output=True,timeout=tl)
    if r.returncode!=0:
        print("runtime",bin,r.returncode); sys.exit(1)
    return r.stdout.decode()

def main():
    compile_all()
    rng=random.Random(1840)
    cases=[]
    cases.append("10 3\n3 3\n5 7\n2 8\n")
    cases.append("1 1\n1 1\n")
    cases.append("5 3\n1 5\n1 5\n2 3\n")
    cases.append("8 4\n1 1\n8 8\n3 6\n2 7\n")
    for _ in range(80):
        n=rng.randint(1,1500)
        m=rng.randint(1,80)
        lines=[f"{n} {m}"]
        for _j in range(m):
            l=rng.randint(1,n)
            r=rng.randint(l,n)
            lines.append(f"{l} {r}")
        cases.append("\n".join(lines)+"\n")
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data[:300]); print(a); print(b); sys.exit(1)
    if norm(run(STD,cases[0]))!="9\n6\n3":
        print("sample"); sys.exit(1)
    n=m=200000
    rng=random.Random(1)
    lines=[f"{n} {m}"]
    for _ in range(m):
        l=rng.randint(1,n)
        r=rng.randint(l,n)
        lines.append(f"{l} {r}")
    data="\n".join(lines)+"\n"
    t0=time.perf_counter()
    out=run(STD,data,tl=30)
    dt=time.perf_counter()-t0
    if len(out.strip().splitlines())!=m:
        print("max lines"); sys.exit(1)
    print(f"MAX n=m=2e5 time={dt:.3f}s")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
