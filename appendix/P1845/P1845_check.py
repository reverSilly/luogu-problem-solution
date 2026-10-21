#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1845"
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
        print("runtime",bin,r.returncode,r.stderr.decode()[-300:]); sys.exit(1)
    return r.stdout.decode()

def grid(n,rows):
    return str(n)+"\n"+"\n".join(rows)+"\n"

def main():
    compile_all()
    rng=random.Random(1845)
    sample_rows=[
        "0000000000","0011110000","0000010000","0011111000","0010110100",
        "0010010110","0011110010","0100010010","0100000110","0100000000",
    ]
    cases=[]
    cases.append(grid(10,sample_rows))
    cases.append(grid(1,["0"]))
    cases.append(grid(1,["1"]))
    cases.append(grid(2,["00","00"]))
    cases.append(grid(2,["11","00"]))
    cases.append(grid(2,["11","11"]))
    cases.append(grid(3,["010","111","010"]))
    cases.append(grid(3,["101","000","101"]))
    # two images in one stream
    cases.append(grid(1,["1"])+grid(2,["01","10"]))
    for _ in range(70):
        n=rng.randint(1,12)
        rows=[]
        for i in range(n):
            rows.append("".join(rng.choice("0000000011") for _j in range(n)))
        cases.append(grid(n,rows))
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data); print("std",a); print("bru",b); sys.exit(1)
    if norm(run(STD,cases[0]))!="3\n3\n7\n12":
        print("sample",run(STD,cases[0])); sys.exit(1)
    rng=random.Random(5)
    n=1000
    rows=[]
    for i in range(n):
        rows.append("".join(rng.choice("01") for _j in range(n)))
    data=grid(n,rows)
    t0=time.perf_counter()
    out=run(STD,data,tl=20)
    dt=time.perf_counter()-t0
    lines=out.strip().splitlines()
    if not lines or not lines[0].strip().isdigit():
        print("bad max"); sys.exit(1)
    print(f"MAX n=1000 time={dt:.3f}s comps={lines[0].strip()}")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
