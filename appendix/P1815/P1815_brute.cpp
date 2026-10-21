//
// 题目：P1815 [ECNA 2001 Practice] 蠕虫游戏
// https://www.luogu.com.cn/problem/P1815
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

int hx[140],hy[140];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	while(cin>>n)
	{
		if(!n)
			break;
		string s;
		cin>>s;
		for(int i=0;i<20;++i)
		{
			hx[i]=25;
			hy[i]=11+i;
		}
		int head{19};
		bool dead{false};
		for(int i=0;i<n;++i)
		{
			int nr{hx[head]};
			int nc{hy[head]};
			char mv{s[i]};
			if(mv=='E')
				++nc;
			else if(mv=='W')
				--nc;
			else if(mv=='S')
				++nr;
			else
				--nr;
			if(nr<1||nr>50||nc<1||nc>50)
			{
				cout<<"The worm ran off the board on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			bool hit{false};
			for(int j=head-18;j<=head;++j)
			{
				if(hx[j]==nr&&hy[j]==nc)
				{
					hit=true;
					break;
				}
			}
			if(hit)
			{
				cout<<"The worm ran into itself on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			++head;
			hx[head]=nr;
			hy[head]=nc;
		}
		if(!dead)
			cout<<"The worm successfully made all "<<n<<" moves.\n";
	}
	return 0;
}
