// 题目：P2952 [USACO09OPEN] Cow Line S
// 双端队列模拟：数组 + 左右指针，初始放在正中，两端各留 1e5 松弛
// D 操作只移动指针，O(1)；编号按 A 命令出现顺序递增
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int s,cnt,l{100000},r{100000},a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for(int i=0,k;i<s;++i)
	{
		char op,side;
		cin>>op>>side;
		if(op=='A')
		{
			if(side=='L')
				a[--l]=++cnt;
			else
				a[r++]=++cnt;
		}
		else
		{
			cin>>k;
			if(side=='L')
				l+=k;	// 左端弹出 k 头：指针右移即可
			else
				r-=k;
		}
	}
	for(int i=l;i<r;++i)
		cout<<a[i]<<'\n';
	return 0;
}
