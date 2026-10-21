#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1843"
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

def pack(n,a,b,ws):
    return f"{n} {a} {b}\n"+"\n".join(map(str,ws))+"\n"

def main():
    compile_all()
    rng=random.Random(1843)
    cases=[]
    cases.append(pack(3,2,1,[1,2,3]))
    cases.append(pack(1,1,1,[1]))
    cases.append(pack(1,5,1,[1]))
    cases.append(pack(2,1,100,[100,100]))
    cases.append(pack(3,1,1,[5,1,8]))
    for _ in range(100):
        n=rng.randint(1,15)
        a=rng.randint(1,20)
        b=rng.randint(1,20)
        ws=[rng.randint(1,200) for _i in range(n)]
        # distinct as statement says 保证互不相同, not required for correctness but follow
        ws=list(dict.fromkeys(ws))
        if not ws:
            ws=[1]
        cases.append(pack(len(ws),a,b,ws))
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data); print("std",a,"bru",b); sys.exit(1)
    if norm(run(STD,cases[0]))!="1":
        print("sample"); sys.exit(1)
    rng=random.Random(3)
    n=500000
    a=rng.randint(1,500000)
    b=rng.randint(1,500000)
    ws=[]
    used=set()
    while len(ws)<n:
        w=rng.randint(1,500000)
        if w not in used:
            used.add(w)
            ws.append(w)
    data=pack(n,a,b,ws)
    t0=time.perf_counter()
    out=run(STD,data,tl=20)
    dt=time.perf_counter()-t0
    if not out.strip().isdigit():
        print("bad max"); sys.exit(1)
    print(f"MAX n=5e5 time={dt:.3f}s ans={out.strip()}")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
