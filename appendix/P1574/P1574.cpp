//
// 题目：P1574 超级数
// https://www.luogu.com.cn/problem/P1574
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 LIM{100000000000000000LL};   // 1e17
int pr[18]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};

vector<pair<i64,i64>>cand;   // {n, d(n)}：所有指数非递增的候选数
vector<i64>H;                // supernumber 序列(升序)

// 按素数递增分配"非递增指数":cur=当前数,d=当前约数个数,maxe=上一个素数用过的指数上限
void dfs(int idx,i64 cur,i64 d,int maxe)
{
	cand.push_back({cur,d});
	if(idx>=15)
		return;
	i64 p=pr[idx];
	for(int e=1;e<=maxe;++e)
	{
		if(cur>LIM/p)
			break;
		cur*=p;
		dfs(idx+1,cur,d*(e+1),e);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	dfs(0,1,1,60);
	sort(cand.begin(),cand.end());
	i64 best=0;
	for(auto[v,d]:cand)
		if(d>best)
		{
			best=d;
			H.push_back(v);
		}
	int m;
	cin>>m;
	for(int i=0;i<m;++i)
	{
		i64 a;
		cin>>a;
		i64 ans=*(upper_bound(H.begin(),H.end(),a)-1);
		cout<<ans<<'\n';
	}
	return 0;
}
