//
// 题目：P1653 [USACO05FEB] Ski Lift / 滑雪场缆车
// https://www.luogu.com.cn/problem/P1653
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};
constexpr int dr[4]{-1,1,0,0},dc[4]{0,0,-1,1};

int W,L,h[mxn][mxn],comp[mxn][mxn],ccnt=0;
bool has_in[mxn*mxn],has_out[mxn*mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>W>>L;
	for(int r=0;r<L;++r)
		for(int c=0;c<W;++c)
		{
			cin>>h[r][c];
			comp[r][c]=-1;
		}

	queue<pair<int,int>>Q;
	for(int r=0;r<L;++r)
		for(int c=0;c<W;++c)
			if(comp[r][c]==-1)
			{
				comp[r][c]=ccnt;
				Q.push({r,c});
				while(!Q.empty())
				{
					auto[ur,uc]=Q.front();Q.pop();
					for(int d=0;d<4;++d)
					{
						int vr=ur+dr[d],vc=uc+dc[d];
						if(vr>=0&&vr<L&&vc>=0&&vc<W&&comp[vr][vc]==-1&&h[vr][vc]==h[ur][uc])
						{
							comp[vr][vc]=ccnt;
							Q.push({vr,vc});
						}
					}
				}
				++ccnt;
			}

	if(ccnt<=1)
	{
		cout<<0<<'\n';
		return 0;
	}

	for(int r=0;r<L;++r)
		for(int c=0;c<W;++c)
			for(int d=0;d<4;++d)
			{
				int vr=r+dr[d],vc=c+dc[d];
				if(vr>=0&&vr<L&&vc>=0&&vc<W&&h[r][c]>h[vr][vc])
				{
					has_out[comp[r][c]]=true;
					has_in[comp[vr][vc]]=true;
				}
			}

	int src=0,snk=0;
	for(int i=0;i<ccnt;++i)
	{
		if(!has_in[i])
			++src;
		if(!has_out[i])
			++snk;
	}
	cout<<max(src,snk)<<'\n';
	return 0;
}
