//
// 题目：P1871 对撞机
// https://www.luogu.com.cn/problem/P1871
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

bool on[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	while(m--)
	{
		char op;
		int x;
		cin>>op>>x;
		if(op=='+')
		{
			if(on[x])
			{
				cout<<"Already on\n";
				continue;
			}
			int bad=0;
			for(int j=1;j<=n;++j)
			{
				if(on[j]&&std::gcd(j,x)>1)
				{
					bad=j;
					break;
				}
			}
			if(bad)
			{
				cout<<"Conflict with "<<bad<<'\n';
				continue;
			}
			on[x]=true;
			cout<<"Success\n";
		}
		else
		{
			if(!on[x])
			{
				cout<<"Already off\n";
				continue;
			}
			on[x]=false;
			cout<<"Success\n";
		}
	}
	return 0;
}
