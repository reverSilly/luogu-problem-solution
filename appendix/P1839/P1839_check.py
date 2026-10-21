#!/usr/bin/env python3
import math, random, subprocess, sys, time
PID="P1839"
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

def run(bin,data,tl=30):
    r=subprocess.run([bin],input=data.encode(),capture_output=True,timeout=tl)
    if r.returncode!=0:
        print("runtime",bin,r.returncode,r.stderr.decode()[-400:])
        sys.exit(1)
    return r.stdout.decode()

def legal(a,b,n):
    if b==1:
        return a<=n
    if a==1:
        return True
    r=1
    for _ in range(b):
        if r>n//a:
            return False
        r*=a
    return r<=n

def make(n,qs):
    lines=[str(n),str(len(qs))]
    lines+=[f"{a} {b}" for a,b in qs]
    return "\n".join(lines)+"\n"

def main():
    compile_all()
    rng=random.Random(1839)
    cases=[]
    cases.append(make(9,[(2,2),(1,4)]))
    # edges
    for n in [1,2,3,4,5,8,9,15,16,31,32,100,255,256,1000,2000]:
        lim=int(math.isqrt(n))
        maxb=0
        p=1
        while p<=n//2:
            p*=2
            maxb+=1
        qs=[]
        for b in range(1,min(maxb+4,20)):
            qs.append((1,b))
        for a in [1,2,3,lim,lim+1 if lim+1<=n else n,n]:
            if legal(a,1,n):
                qs.append((a,1))
        for a in range(2,min(lim,12)+1):
            for b in range(2,6):
                if legal(a,b,n):
                    qs.append((a,b))
        # unique
        u=[]
        seen=set()
        for q in qs:
            if q not in seen:
                seen.add(q)
                u.append(q)
        cases.append(make(n,u[:80]))
    for _ in range(30):
        n=rng.randint(1,1500)
        qs=[]
        tries=0
        while len(qs)<12 and tries<400:
            tries+=1
            a=rng.randint(1,n)
            b=rng.randint(1,min(n,25))
            if legal(a,b,n):
                qs.append((a,b))
        if not qs:
            qs=[(1,1)]
        cases.append(make(n,qs))
    for i,data in enumerate(cases):
        a,b=run(STD,data),run(BRU,data,tl=60)
        if norm(a)!=norm(b):
            print("MISMATCH",i)
            print(data[:400])
            print("std",a)
            print("bru",b)
            sys.exit(1)
    exp=norm(run(STD,cases[0]))
    if exp!="Masha\nMissing":
        print("sample",exp)
        sys.exit(1)
    # max range timing, std only
    n=10**8
    qs=[]
    lim=int(math.isqrt(n))
    qs+=[(1,b) for b in (1,2,3,10,20,26,27,100,10**8)]
    qs+=[(a,1) for a in (1,2,lim-1,lim,lim+1,10**7,n-1,n)]
    qs+=[(2,b) for b in range(1,27)]
    qs+=[(3,b) for b in range(1,18)]
    qs+=[(lim,1),(lim,2)]
    rng2=random.Random(99)
    while len(qs)<100:
        a=rng2.choice([1,2,3,5,10,100,lim,lim+5,n//2,n])
        b=rng2.randint(1,30)
        if a<1: a=1
        if a>n and b==1:
            a=n
        if legal(a,b,n):
            qs.append((a,b))
        else:
            qs.append((1,rng2.randint(1,n)))
    data=make(n,qs[:100])
    t0=time.perf_counter()
    out=run(STD,data,tl=20)
    dt=time.perf_counter()-t0
    if len(out.strip().splitlines())!=100:
        print("max lines",len(out.splitlines()))
        sys.exit(1)
    print(f"MAX n=1e8 t=100 time={dt:.3f}s")
    print(f"{len(cases)} tests OK")
    return 0

if __name__=="__main__":
    sys.exit(main())
