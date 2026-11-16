// P2771 轻量验证（性质隐蔽：Euler 示性数；边界易错：重复边/点、仅一点相触的区域）
// formula：题解答案 E-V+1（E=不同单位线段数，V=不同格点数）。
// flood：独立算法 —— 在"单元格中心"的四邻接图上 flood fill 数连通块，
//        相邻单元格被围栏阻挡当且仅当对应单位线段被建过；答案 = 连通块数 - 1（减去无界外部）。
// 四邻接（而非八邻接）天然正确处理"两区域仅在一个格点相触"的捏点情形。
#include<bits/stdc++.h>
using namespace std;

constexpr int ofs{85},sz{175};	// 验证用 N≤80，坐标 ∈[-80,80]

bool pv[sz][sz],peh[sz][sz],pev[sz][sz],cv[sz][sz];

int formula(const string&s)
{
	memset(pv,0,sizeof pv);
	memset(peh,0,sizeof peh);
	memset(pev,0,sizeof pev);
	int x=ofs,y=ofs,V=1,E=0;
	pv[x][y]=true;
	for(char c:s)
	{
		int nx=x,ny=y;
		if(c=='N')++ny;
		else if(c=='S')--ny;
		else if(c=='E')++nx;
		else --nx;
		if(!pv[nx][ny])
		{
			pv[nx][ny]=true;
			++V;
		}
		if(nx==x)
		{
			int yy=min(y,ny);
			if(!pev[x][yy])
			{
				pev[x][yy]=true;
				++E;
			}
		}
		else
		{
			int xx=min(x,nx);
			if(!peh[xx][y])
			{
				peh[xx][y]=true;
				++E;
			}
		}
		x=nx;y=ny;
	}
	return E-V+1;
}

int flood(const string&s)
{
	int N=s.size();
	memset(cv,0,sizeof cv);
	int cnt=0;
	for(int x=-N-1;x<=N;++x)
		for(int y=-N-1;y<=N;++y)
		{
			if(cv[x+ofs][y+ofs])
				continue;
			++cnt;
			queue<pair<int,int>>Q;
			cv[x+ofs][y+ofs]=true;
			Q.push({x,y});
			while(!Q.empty())
			{
				auto[u,v]=Q.front();Q.pop();
				if(u+1<=N&&!pev[u+1+ofs][v+ofs]&&!cv[u+1+ofs][v+ofs])
				{
					cv[u+1+ofs][v+ofs]=true;
					Q.push({u+1,v});
				}
				if(u-1>=-N-1&&!pev[u+ofs][v+ofs]&&!cv[u-1+ofs][v+ofs])
				{
					cv[u-1+ofs][v+ofs]=true;
					Q.push({u-1,v});
				}
				if(v+1<=N&&!peh[u+ofs][v+1+ofs]&&!cv[u+ofs][v+1+ofs])
				{
					cv[u+ofs][v+1+ofs]=true;
					Q.push({u,v+1});
				}
				if(v-1>=-N-1&&!peh[u+ofs][v+ofs]&&!cv[u+ofs][v-1+ofs])
				{
					cv[u+ofs][v-1+ofs]=true;
					Q.push({u,v-1});
				}
			}
		}
	return cnt-1;
}

mt19937 rn(20260602);
const char dir[4]={'N','E','S','W'};

string genwalk(int N)	// 带惯性的随机游走：大量自交与围圈
{
	string s;
	int d=rn()%4;
	for(int i=0;i<N;++i)
	{
		if(rn()%100<55)
			d=rn()%4;
		s+=dir[d];
	}
	return s;
}

string genloop(int N)	// 随机矩形周界拼接（可能截断，前缀仍是合法游走）
{
	string s;
	while((int)s.size()<N)
	{
		int w=1+rn()%5,h=1+rn()%5;
		bool rev=rn()%2;
		for(int i=0;i<h;++i)s+=rev?'S':'N';
		for(int i=0;i<w;++i)s+=rev?'W':'E';
		for(int i=0;i<h;++i)s+=rev?'N':'S';
		for(int i=0;i<w;++i)s+=rev?'E':'W';
	}
	return s.substr(0,N);
}

int main()
{
	vector<pair<string,int>>hand={{"N",0},{"NESW",1},{"NS",0},{"NESWWNES",2},{"NESWNWNES",2},{"NNNNEEEESSSSWWWW",1}};
	for(auto[s,exp]:hand)
	{
		int f=formula(s),g=flood(s);
		if(f!=exp||g!=exp)
		{
			printf("HAND FAIL %s formula=%d flood=%d expect=%d\n",s.c_str(),f,g,exp);
			return 1;
		}
	}
	for(int it=0;it<4000;++it)
	{
		int N=1+rn()%80;
		string s=(rn()%2?genwalk(N):genloop(N));
		int f=formula(s),g=flood(s);
		if(f!=g||f<0)
		{
			printf("MISMATCH N=%d formula=%d flood=%d\n%s\n",(int)s.size(),f,g,s.c_str());
			return 1;
		}
	}
	printf("6 hand cases + 4000 random walks: formula E-V+1 == flood-fill regions-1\n");
	return 0;
}