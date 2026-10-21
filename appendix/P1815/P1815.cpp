//
// 题目：P1815 [ECNA 2001 Practice] 蠕虫游戏
// https://www.luogu.com.cn/problem/P1815
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

int occ[52][52];

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
		memset(occ,0,sizeof(occ));
		deque<pair<int,int>> body;
		for(int c=11;c<=30;++c)
		{
			occ[25][c]=1;
			body.push_front({25,c});
		}
		bool dead{false};
		for(int i=0;i<n;++i)
		{
			int hr{body.front().first};
			int hc{body.front().second};
			int nr{hr};
			int nc{hc};
			char mv{s[i]};
			if(mv=='E')
				++nc;
			else if(mv=='W')
				--nc;
			else if(mv=='S')
				++nr;
			else
				--nr;
			int tr{body.back().first};
			int tc{body.back().second};
			body.pop_back();
			occ[tr][tc]=0;
			if(nr<1||nr>50||nc<1||nc>50)
			{
				cout<<"The worm ran off the board on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			if(occ[nr][nc])
			{
				cout<<"The worm ran into itself on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			occ[nr][nc]=1;
			body.push_front({nr,nc});
		}
		if(!dead)
			cout<<"The worm successfully made all "<<n<<" moves.\n";
	}
	return 0;
}
