//
// P1620 朴素解：状态图 BFS，状态 =(已用O, 已用X, 末尾字母, 当前连续长度)
// 用法：仅用于与 P1620.cpp 对拍，非提交代码（仅支持 <=20 的小数据）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxk{105};

int co,cx,mo,mx;
bool vis[mxk][mxk][2][mxk];
struct node{int o,x,last,run;};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	while(cin>>co>>cx>>mo>>mx)
	{
		memset(vis,0,sizeof vis);
		queue<node>Q;
		if(co>0&&mo>0)
		{
			vis[1][0][0][1]=true;
			Q.push({1,0,0,1});
		}
		if(cx>0&&mx>0)
		{
			vis[0][1][1][1]=true;
			Q.push({0,1,1,1});
		}
		int ans=0;
		while(!Q.empty())
		{
			node u=Q.front();
			Q.pop();
			ans=max(ans,u.o+u.x);
			// 接一个 O
			if(u.o<co)
			{
				int r=(u.last==0?u.run+1:1);
				if(r<=mo&&!vis[u.o+1][u.x][0][r])
				{
					vis[u.o+1][u.x][0][r]=true;
					Q.push({u.o+1,u.x,0,r});
				}
			}
			if(u.x<cx)
			{
				int r=(u.last==1?u.run+1:1);
				if(r<=mx&&!vis[u.o][u.x+1][1][r])
				{
					vis[u.o][u.x+1][1][r]=true;
					Q.push({u.o,u.x+1,1,r});
				}
			}
		}
		cout<<ans<<'\n';
	}
	return 0;
}
