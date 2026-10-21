#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1853"
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
        print("runtime",bin,r.returncode); sys.exit(1)
    return r.stdout.decode()

def pack(s,yr,bonds):
    lines=[f"{s} {yr} {len(bonds)}"]
    for a,b in bonds:
        lines.append(f"{a} {b}")
    return "\n".join(lines)+"\n"

def main():
    compile_all()
    rng=random.Random(1853)
    cases=[]
    cases.append(pack(10000,4,[(4000,400),(3000,250)]))
    cases.append(pack(1000,2,[(1000,100)]))
    cases.append(pack(2500,2,[(1000,100)]))
    cases.append(pack(999,2,[(1000,100)]))
    cases.append(pack(5000,3,[(1000,50),(2000,200),(3000,100)]))
    for _ in range(50):
        d=rng.randint(1,3)
        bonds=[]
        for _i in range(d):
            a=rng.choice([1000,2000,3000,4000])
            b=rng.randint(1,a//10)
            bonds.append((a,b))
        s=rng.choice([0,500,1000,1999,2000,3500,4000,8000])
        if s==0:
            s=rng.randint(1,3000)
        yr=rng.randint(2,4)
        cases.append(pack(s,yr,bonds))
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data); print("std",a,"bru",b); sys.exit(1)
    if norm(run(STD,cases[0]))!="14050":
        print("sample",run(STD,cases[0])); sys.exit(1)
    if norm(run(STD,cases[1]))!="1200":
        print("hand",run(STD,cases[1])); sys.exit(1)
    bonds=[(1000*(i+1), max(1,(1000*(i+1))//10)) for i in range(10)]
    data=pack(1000000,40,bonds)
    t0=time.perf_counter()
    out=run(STD,data)
    dt=time.perf_counter()-t0
    # upper bound growth
    s=1000000
    for _ in range(40):
        s=s+(s//1000)*100
    ans=int(out.strip())
    if ans>s or ans<1000000:
        print("ans out of range",ans,s); sys.exit(1)
    print(f"MAX s=1e6 n=40 d=10 time={dt:.3f}s ans={ans}")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
