#!/usr/bin/env python3
"""P1579 哥德巴赫猜想(升级版):标程 vs 暴力基准 对拍脚本。

流程:
  1. 编译 P1579.cpp(标程)与 P1579_brute.cpp(暴力基准),g++ -O2 -std=c++17
  2. 官方样例回归:2009 -> 3 3 2003(样例取自洛谷原题页)
  3. 随机对拍(默认 3000 组):题面数据范围为奇数 n,9<n<20000;随机段取 n<=1500,
     是因为暴力基准是 O(n^2) 全枚举,大 n 下单组要几百毫秒;完整数据范围由
     第 5 步"全量扫描"覆盖(所有 9995 个奇数 n 一个不漏)
  4. 大随机(40 组,n in [10001,19999])+ 边界用例(最小 n=11、最大 n=19999、
     循环最坏 n=19991 等),std 与 brute 逐字节对比
  5. 全量扫描:所有奇数 n in [11,19999],标程输出须通过第三方合法性校验
     (三个正整数均为质数[试除法独立判定]且和为 n、格式为 "a b c"),
     且与"无剪枝首命中"参考实现一致(该参考不从 a 起枚举 b,专门检验
     标程里 b>=a 剪枝的合法性)
  6. 最坏规模计时:n=19991(实测内层迭代最多,2277 次)与 n=19999

任何一组不一致立即打印该组输入与双方输出并 sys.exit(1)。

用法:python3 P1579_check.py [随机组数]
"""
import os,random,re,subprocess,sys,time

here=os.path.dirname(os.path.abspath(__file__))
std,brute='P1579','P1579_brute'
sample_in,sample_out='2009','3 3 2003'     # 洛谷官方样例
LIM=20005                                  # 覆盖题面 n<20000(+5 松弛)

def run(prog,inp):
	r=subprocess.run([os.path.join(here,prog)],input=inp,capture_output=True,text=True,timeout=60)
	if r.returncode:
		raise RuntimeError(f'{prog} 返回 {r.returncode}: {r.stderr[:400]}')
	return r.stdout

def fail(inp,a,b):
	print(f'[对拍失败] 输入:\n{inp}标程输出: {a!r}\n暴力输出: {b!r}')
	sys.exit(1)

# ---- 第三方工具:与两个 C++ 程序互相独立的判质/参考实现 ----
isp=bytearray([1])*LIM                     # Python 埃氏筛
isp[0]=isp[1]=0                            # 1 不是质数
for i in range(2,LIM):
	if isp[i]:
		for j in range(i*i,LIM,i):
			isp[j]=0
PR=[i for i in range(2,LIM) if isp[i]]

def tdprime(x):                            # 试除法判质,不复用上面的筛
	if x<2:
		return False
	i=2
	while i*i<=x:
		if x%i==0:
			return False
		i+=1
	return True

def valid(out,n):                          # 题面格式 + 三个质数 + 和为 n
	if not re.fullmatch(r'\d+ \d+ \d+',out):
		return False
	x,y,z=map(int,out.split())
	return x+y+z==n and all(tdprime(v) for v in (x,y,z))

def ref(n):                                # 无剪枝首命中参考:b 从 2 起,不用 a<=b<=c 引理
	for a in PR:
		if a>n-4:                           # 另外两个质数最小 2+2
			break
		for b in PR:
			if b>n-a-2:                     # c=n-a-b>=2
				break
			if isp[n-a-b]:
				return f'{a} {b} {n-a-b}'
	return None

def gen(rnd):
	# 题面:仅一行,一个正奇数 n,9<n<20000
	return f"{rnd.randrange(11,1501,2)}\n"

def main():
	rounds=int(sys.argv[1]) if len(sys.argv)>1 else 3000
	subprocess.run(['g++','-O2','-std=c++17','-o',os.path.join(here,std),
	                os.path.join(here,std+'.cpp')],check=True)
	subprocess.run(['g++','-O2','-std=c++17','-o',os.path.join(here,brute),
	                os.path.join(here,brute+'.cpp')],check=True)
	print('[编译] g++ -O2 -std=c++17:标程与暴力基准均成功')

	got=run(std,sample_in+'\n')
	ok=(got==sample_out+'\n')
	print(f'[样例回归] 官方样例 2009 -> 期望 {sample_out!r},实得 {got.strip()!r} -> {"PASS" if ok else "FAIL"}')
	assert ok,'官方样例不过,后续对拍无意义'

	rnd=random.Random(20261020)
	t0=time.time()
	for i in range(1,rounds+1):
		inp=gen(rnd)
		a,b=run(std,inp),run(brute,inp)
		if a!=b:
			fail(inp,a,b)
	print(f'[随机对拍] {rounds} 组(奇数 n in [11,1500])std==brute,耗时 {time.time()-t0:.1f}s')

	t0=time.time()
	for i in range(40):
		n=rnd.randrange(10001,20000,2)
		inp=f'{n}\n'
		a,b=run(std,inp),run(brute,inp)
		if a!=b:
			fail(inp,a,b)
	print(f'[大随机对拍] 40 组(奇数 n in [10001,19999])std==brute,耗时 {time.time()-t0:.1f}s')

	# 边界:最小 n、最大 n、循环最坏 n、官方样例 n、答案首质数为 3 的小 n
	edge=[11,13,15,17,25,27,31,2009,9999,19991,19997,19999]
	t0=time.time()
	for n in edge:
		inp=f'{n}\n'
		a,b=run(std,inp),run(brute,inp)
		if a!=b:
			fail(inp,a,b)
		assert valid(a.strip(),n),f'标程输出不合法: n={n} out={a!r}'
	print(f'[边界用例] n={edge} std==brute 且输出合法,耗时 {time.time()-t0:.1f}s')

	t0=time.time()
	for n in range(11,20000,2):
		a=run(std,f'{n}\n')
		if not valid(a.strip(),n):
			print(f'[全量扫描失败] n={n} 标程输出 {a!r} 不满足"三个质数之和为 n"')
			sys.exit(1)
		r=ref(n)
		if a.strip()!=r:
			print(f'[全量扫描失败] n={n}\n标程输出: {a!r}\n参考输出: {r!r}')
			sys.exit(1)
	print(f'[全量扫描] 9995 个奇数 n in [11,19999] 全部合法且与无剪枝参考一致,耗时 {time.time()-t0:.1f}s')

	# 最坏规模计时:实测内层迭代最多的 n=19991(2277 次)与最大 n=19999
	for n in (19991,19999):
		reps=200
		t0=time.perf_counter()
		for _ in range(reps):
			run(std,f'{n}\n')
		dt=(time.perf_counter()-t0)/reps
		print(f'[最坏规模计时] n={n}:{reps} 次平均 {dt*1000:.2f} ms/次(含进程启动与筛法)')

	print('P1579 check passed! 样例回归 + 3000 组随机 + 40 组大随机 + 12 个边界 '
	      '+ 9995 个奇数全量扫描,全部 std==brute/参考一致')

if __name__=='__main__':
	main()
