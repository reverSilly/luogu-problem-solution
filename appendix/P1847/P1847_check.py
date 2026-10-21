#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1847"
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

def main():
    compile_all()
    rng=random.Random(1847)
    U=2**32-1
    cases=[]
    cases.append("1 1\n1 1 10 10\n11 11\n")
    cases.append("2 3\n1 3 7 10\n5 6 8 9\n1 4\n6 7\n7 11\n")
    cases.append(f"1 2\n1 1 {U} {U}\n1 1\n{U} {U}\n")
    cases.append("1 1\n10 10 1 1\n5 5\n")
    cases.append("3 2\n1 1 2 2\n2 2 3 3\n1 1 5 1\n2 2\n4 1\n")
    for _ in range(60):
        m=rng.randint(1,25)
        n=rng.randint(1,25)
        lines=[f"{m} {n}"]
        for _j in range(m):
            x1,x2=sorted(rng.randint(1,40) for _k in range(2))
            y1,y2=sorted(rng.randint(1,40) for _k in range(2))
            if rng.random()<0.3:
                x1,x2=x2,x1
            if rng.random()<0.3:
                y1,y2=y2,y1
            lines.append(f"{x1} {y1} {x2} {y2}")
        for _j in range(n):
            if rng.random()<0.2:
                lines.append(f"{U} {rng.randint(1,5)}")
            else:
                lines.append(f"{rng.randint(1,40)} {rng.randint(1,40)}")
        cases.append("\n".join(lines)+"\n")
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i); print(data); print(a); print(b); sys.exit(1)
    if norm(run(STD,cases[0]))!="NO":
        print("sample"); sys.exit(1)
    if norm(run(STD,cases[1]))!="YES 1 1\nYES 2 2\nNO":
        print("hand",run(STD,cases[1])); sys.exit(1)
    rng=random.Random(8)
    m=n=2000
    lines=[f"{m} {n}"]
    for _ in range(m):
        x1=rng.randint(1,U); x2=rng.randint(1,U)
        y1=rng.randint(1,U); y2=rng.randint(1,U)
        lines.append(f"{x1} {y1} {x2} {y2}")
    for _ in range(n):
        lines.append(f"{rng.randint(1,U)} {rng.randint(1,U)}")
    data="\n".join(lines)+"\n"
    t0=time.perf_counter()
    a=run(STD,data,tl=20)
    b=run(BRU,data,tl=20)
    dt=time.perf_counter()-t0
    if norm(a)!=norm(b) or len(a.strip().splitlines())!=n:
        print("max mismatch"); sys.exit(1)
    print(f"MAX m=n=2000 time={dt:.3f}s")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
