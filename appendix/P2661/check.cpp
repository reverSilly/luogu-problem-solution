// P2661 轻量验证（非题解代码）：
// 真值 = 直接模拟游戏：逐轮同步传播「已知生日集合」，第 r 轮若某个 T_u=v 的 u
//        已知 v 的生日，则 v 本轮听到自己的生日，游戏在第 r 轮结束。
// 被测 = 题解的全局时间戳游走求函数图最小环。
// 覆盖：n=2..6 穷举所有 T（T_i≠i，共 16739 组）+ 500 组随机 n≤150。
// 仅用 cin/cout 与内存随机数，不做文件 I/O。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205},inf{1145141919};

int n,t[mxn],vis[mxn],clk;
vector<int>in[mxn];
bitset<256>know[mxn],nk[mxn];

int solve()	// 与题解完全一致：全局时间戳游走求最小环
{
	fill_n(vis,n,0);
	clk=0;
	int ans=inf;
	for(int i=0;i<n;++i)
	{
		if(vis[i])
			continue;
		int st=clk,u=i;
		while(!vis[u])
		{
			vis[u]=++clk;
			u=t[u];
		}
		if(vis[u]>st)
			ans=min(ans,clk-vis[u]+1);
	}
	return ans;
}

int sim()	// 真值：逐轮模拟
{
	for(int v=0;v<n;++v)
	{
		know[v].reset();
		know[v].set(v);
	}
	for(int r=1;r<=n;++r)
	{
		for(int v=0;v<n;++v)
			for(int u:in[v])
				if(know[u].test(v))
					return r;	// 本轮 v 将从 u 处听到自己的生日
		for(int v=0;v<n;++v)
		{
			nk[v]=know[v];
			for(int u:in[v])
				nk[v]|=know[u];
		}
		for(int v=0;v<n;++v)
			know[v]=nk[v];
	}
	return inf;	// 最小环 ≤ n，理论上到不了这里
}

mt19937 rng(20260207);
long long tot;

void run()
{
	for(int v=0;v<n;++v)
		in[v].clear();
	for(int u=0;u<n;++u)
		in[t[u]].push_back(u);
	int a=solve(),b=sim();
	++tot;
	if(a!=b)
	{
		cout<<"MISMATCH n="<<n<<" solve="<<a<<" sim="<<b<<" T:";
		for(int i=0;i<n;++i)
			cout<<' '<<t[i]+1;
		cout<<'\n';
		exit(0);
	}
}

void dfs(int i)	// 穷举 T_i ≠ i
{
	if(i==n)
	{
		run();
		return;
	}
	for(int v=0;v<n;++v)
		if(v!=i)
		{
			t[i]=v;
			dfs(i+1);
		}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(n=2;n<=6;++n)
		dfs(0);
	cout<<"exhaustive n=2..6 done, cases="<<tot<<'\n';
	for(int cs=0;cs<500;++cs)
	{
		n=2+rng()%149;
		for(int i=0;i<n;++i)
		{
			int x=rng()%(n-1);
			if(x>=i)
				++x;
			t[i]=x;
		}
		run();
	}
	cout<<"random 500 done, total="<<tot<<", all match\n";
	return 0;
}