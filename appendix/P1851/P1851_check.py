#!/usr/bin/env python3
import random, subprocess, sys, time
PID="P1851"
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
    rng=random.Random(1851)
    known={
        6:"220 284",
        206:"220 284",
        220:"220 284",
        221:"284 220",
        284:"284 220",
        285:"1184 1210",
        18000:"18416 17296",
        17296:"17296 18416",
        17297:"18416 17296",
    }
    cases=[f"{s}\n" for s in known]
    for _ in range(40):
        cases.append(f"{rng.randint(6,18000)}\n")
    t0=time.perf_counter()
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data)
        if norm(a)!=norm(b):
            print("MISMATCH",i,data,a,b); sys.exit(1)
        s=int(data.strip())
        if s in known and norm(a)!=known[s]:
            print("known fail",s,a); sys.exit(1)
    dt=time.perf_counter()-t0
    print(f"MAX s=18000 included time={dt:.3f}s")
    print(f"{len(cases)} tests OK")

if __name__=="__main__":
    main()
