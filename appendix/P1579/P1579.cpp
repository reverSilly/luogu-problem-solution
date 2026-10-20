#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};    // n<20000 (+5 松弛);c=n-a-b 最大只到 19995

bool np[mxn];                // np[i]=true 表示 i 非质数;0/1 一并标记(1 不是质数)
int pr[mxn],cnt;             // pr[]=升序质数表,cnt=质数个数(20000 内共 2262 个)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	np[0]=np[1]=true;                             // 题面黑体:1 不是质数
	for(int i=2;i<mxn;++i)                        // 埃氏筛
		if(!np[i])
		{
			pr[cnt++]=i;
			for(int j=i*2;j<mxn;j+=i)
				np[j]=true;
		}
	int n;
	cin>>n;
	for(int i=0;i<cnt&&pr[i]<=n-4;++i)            // 另外两个质数最小 2+2,a 不能超过 n-4
	{
		int a=pr[i];
		for(int j=i;j<cnt;++j)                    // b>=a:字典序最小解必满足 a<=b<=c
		{
			int b=pr[j];
			int c=n-a-b;
			if(c<2)                               // b 升则 c 降,后续更小;同时防负下标越界
				break;
			if(!np[c])                            // c<=19995,不会越界
			{
				cout<<a<<' '<<b<<' '<<c<<'\n';
				return 0;
			}
		}
	}
	return 0;
}
