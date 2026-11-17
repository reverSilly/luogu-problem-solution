// 题目：P2949 [USACO09OPEN] Work Scheduling G
// 按截止时间升序 + 小根堆反悔贪心：没满就直收；满了且新工作更赚，就换掉堆里最不赚的
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
struct job{int d,p;}a[mxn];
priority_queue<int,vector<int>,greater<>>Q;
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].d>>a[i].p;
	sort(a,a+n,[](job x,job y){return x.d<y.d;});
	for(int i=0;i<n;++i)
	{
		if((int)Q.size()<a[i].d)	// 截止前还有空时段：直接做
		{
			Q.push(a[i].p);
			ans+=a[i].p;
		}
		else if(a[i].p>Q.top())	// 满了：反悔，踢掉已选里利润最小的
		{
			ans-=Q.top();Q.pop();
			Q.push(a[i].p);
			ans+=a[i].p;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
