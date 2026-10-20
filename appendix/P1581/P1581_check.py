import os
import subprocess, random, sys, time

PR=[2,3,5,7,11]                    # 个位起各位的质数基(题面加数至万位)
SAMPLE_IN="1,0+2,1"                # 洛谷官方样例
SAMPLE_OUT="1,0,1"
WORST_IN="10,6,4,2,1+10,6,4,2,1"   # 最坏规模:两个加数都取满各位
WORST_OUT="1,10,6,4,2,0"           # 2309+2309=4618,手算混合进制结果
RAND_UNIFORM=3000                  # 1~5 位均匀随机
RAND_TINY=1000                     # 1~2 位短数随机(强化低位/连续进位)
TIME_REPS=200

def compile_all():
    subprocess.run(["g++","-O2","-std=c++17","P1581.cpp","-o","P1581"],check=True)
    subprocess.run(["g++","-O2","-std=c++17","P1581_brute.cpp","-o","P1581_brute"],check=True)

def run(b,inp):
    p=subprocess.run([f"./{b}"],input=inp,text=True,capture_output=True,check=True)
    return p.stdout.strip()

def fail(tag,inp,a,b):
    print(f"[{tag}] mismatch on input: {inp}")
    print(f"  std   = {a}")
    print(f"  brute = {b}")
    sys.exit(1)

def cmp(tag,inp,expect=None):
    a=run("P1581",inp)
    b=run("P1581_brute",inp)
    if a!=b:
        fail(tag,inp,a,b)
    if expect is not None and a!=expect:
        print(f"[{tag}] wrong answer on input: {inp}")
        print(f"  std   = {a}")
        print(f"  expect= {expect}")
        sys.exit(1)

def gen_addend(rng,maxlen):
    # 题面:加数 1~5 位,高位在前;个位起第 i 位的数字必须 < 第 i 个质数
    m=rng.randint(1,maxlen)
    ds=[]
    for i in range(m-1,-1,-1):              # 从高位往低位生成
        lo=1 if (i==m-1 and m>1) else 0     # 多位时最高位非零,不产生前导零
        ds.append(rng.randint(lo,PR[i]-1))
    return ",".join(map(str,ds))

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    compile_all()
    rng=random.Random(20261020)

    # 1. 官方样例回归
    cmp("sample",SAMPLE_IN,SAMPLE_OUT)

    # 2. 手算边界(期望值独立于两个程序)
    edges=[("1+1","1,0"),("0+0","0"),("0+1","1"),("1,0+1","1,1"),
           ("1,1+1,1","1,0,0"),("2,1+1,0","1,0,1"),("1,0,1+1,0,1","2,1,0"),
           ("1,1,1,1,1+1,1,1,1,1","2,2,3,0,0"),
           ("1+10,6,4,2,1","1,0,0,0,0,0"),("10,6,4,2,1+0","10,6,4,2,1"),
           (WORST_IN,WORST_OUT)]
    for inp,exp in edges:
        cmp("edge",inp,exp)

    # 3. 随机对拍:题面范围内 3000 组均匀 + 1000 组短数
    for tc in range(RAND_UNIFORM+RAND_TINY):
        maxlen=5 if tc<RAND_UNIFORM else 2
        inp=gen_addend(rng,maxlen)+"+"+gen_addend(rng,maxlen)
        cmp("random",inp)

    # 4. 最坏规模计时
    t0=time.perf_counter()
    for _ in range(TIME_REPS):
        out=run("P1581",WORST_IN)
    dt=(time.perf_counter()-t0)/TIME_REPS
    if out!=WORST_OUT:
        fail("worst",WORST_IN,out,WORST_OUT)

    total=RAND_UNIFORM+RAND_TINY+len(edges)+1
    print(f"P1581 check passed! sample+{len(edges)} edges+{RAND_UNIFORM+RAND_TINY} random "
          f"(={total} cases) all std==brute; worst {WORST_IN} -> {out} "
          f"in {dt*1000:.3f} ms/run (incl. process startup, {TIME_REPS} reps)")

if __name__=="__main__":
    main()
