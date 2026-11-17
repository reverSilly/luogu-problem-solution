// 题目：P2989 [USACO10MAR] Need For Speed S
// 最优集合恰为按 F_i/M_i 降序排列后、使总比值严格增大的前缀；交叉相乘精确比较
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

struct part{int f,m,i;};

int n;
i64 F,M;	// 当前总力/总质量：F 可达 1e10，乘积可达 1e13，必须 i64
part a[mxn];
vector<int>ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>F>>M>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i].f>>a[i].m;
		a[i].i=i;
	}
	sort(a,a+n,[](const part&x,const part&y){return (i64)x.f*y.m>(i64)y.f*x.m;});
	for(int i=0;i<n;++i)
	{
		if((i64)a[i].f*M>F*(i64)a[i].m)	// 严格改进才装；相等会增质量不增比值
		{
			F+=a[i].f;M+=a[i].m;
			ans.push_back(a[i].i);
		}
		else break;	// 前缀比值单峰，一旦不改进后面永远不改进
	}
	if(ans.empty())
	{
		cout<<"NONE\n";
		return 0;
	}
	sort(ans.begin(),ans.end());
	for(int x:ans)
		cout<<x+1<<'\n';
	return 0;
}
