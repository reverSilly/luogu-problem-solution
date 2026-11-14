// 暴力：枚举 x*y 矩形的所有正方形铺法，求最小边长和 S。
// 与贪心公式 x+y-gcd(x,y) 对比。仅用于小范围验证。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int X,Y;
bool used[8][8];
i64 best;
unordered_map<unsigned long long,i64>memo;

i64 gcd(i64 a,i64 b){return b?gcd(b,a%b):a;}
i64 greedy(i64 x,i64 y){return x+y-gcd(x,y);}

unsigned long long enc()
{
	unsigned long long s=0;
	for(int y=0;y<Y;++y)
		for(int x=0;x<X;++x)
			if(used[y][x])
				s|=1ULL<<(y*X+x);
	return s;
}

// 剩余面积
int area()
{
	int c=0;
	for(int y=0;y<Y;++y)
		for(int x=0;x<X;++x)
			if(!used[y][x])++c;
	return c;
}

void dfs(i64 cur)
{
	int rem=area();
	if(rem==0)
	{
		best=min(best,cur);
		return;
	}
	// 下界：每个正方形边长 <= min(X,Y)，故剩余边长和 >= rem/min(X,Y)
	int shortSide=min(X,Y);
	i64 lb=(rem+shortSide-1)/shortSide;
	if(cur+lb>=best)
		return;
	unsigned long long st=enc();
	auto it=memo.find(st);
	if(it!=memo.end()&&it->second<=cur)
		return;
	memo[st]=cur;
	// 找最下最左的空格
	int px=-1,py=-1;
	for(int y=0;y<Y&&px<0;++y)
		for(int x=0;x<X;++x)
			if(!used[y][x]){px=x;py=y;break;}
	// 该位置能放的最大正方形边长
	int mx=0;
	for(int s=1;px+s<=X&&py+s<=Y;++s)
	{
		bool ok=true;
		for(int y=py;y<py+s&&ok;++y)
			for(int x=px;x<px+s;++x)
				if(used[y][x]){ok=false;break;}
		if(ok)mx=s;else break;
	}
	for(int s=mx;s>=1;--s)
	{
		for(int y=py;y<py+s;++y)
			for(int x=px;x<px+s;++x)
				used[y][x]=true;
		dfs(cur+s);
		for(int y=py;y<py+s;++y)
			for(int x=px;x<px+s;++x)
				used[y][x]=false;
	}
}

int main()
{
	int lim=8;
	bool allok=true;
	for(int x=1;x<=lim;++x)
		for(int y=1;y<=lim;++y)
		{
			X=x;Y=y;
			memset(used,0,sizeof used);
			memo.clear();
			best=greedy(x,y);	// 用贪心值做初始上界，搜索能否更小
			dfs(0);
			i64 g=greedy(x,y);
			if(best!=g)
			{
				printf("MISMATCH x=%d y=%d brute=%lld greedy=%lld\n",x,y,(long long)best,(long long)g);
				allok=false;
			}
		}
	printf(allok?"ALL MATCH up to %dx%d\n":"SOME MISMATCH\n",lim,lim);
	return 0;
}