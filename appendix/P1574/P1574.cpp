//
// 题目：P1574 超级数
// https://www.luogu.com.cn/problem/P1574
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr long long lim{100000000000000000LL}; // 10^17
constexpr int primes[15]{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

struct Cand
{
	long long val,divs;
	friend bool operator<(Cand a,Cand b)
	{
		return a.val!=b.val?a.val<b.val:a.divs>b.divs;
	}
};

vector<Cand> cands;
vector<long long> super_nums;

void dfs(int idx,int max_e,long long cur_val,long long cur_divs)
{
	cands.push_back({cur_val,cur_divs});
	if(idx>=15)
		return;
	long long p=primes[idx];
	for(int e=1;e<=max_e;++e)
	{
		if(cur_val>lim/p)
			break;
		cur_val*=p;
		dfs(idx+1,e,cur_val,cur_divs*(e+1));
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	dfs(0,60,1,1);
	sort(cands.begin(),cands.end());

	long long best_d=0;
	for(const auto& c:cands)
		if(c.divs>best_d)
		{
			best_d=c.divs;
			super_nums.push_back(c.val);
		}

	int m;
	if(!(cin>>m))
		return 0;
	for(int i=0;i<m;++i)
	{
		long long a;
		cin>>a;
		auto it=upper_bound(super_nums.begin(),super_nums.end(),a);
		--it;
		cout<<*it<<'\n';
	}
	return 0;
}
