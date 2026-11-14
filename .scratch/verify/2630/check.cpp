#include<bits/stdc++.h>
using namespace std;

// ---- 与正解相同的 BFS ----
int que[13][9];string pat[13];int scnt;
void apply(int*a,int*b,int op)
{
	for(int r=0;r<3;++r)for(int c=0;c<3;++c)
	{
		int rr,cc;
		if(op==0){rr=2-c;cc=r;}
		else if(op==1){rr=c;cc=2-r;}
		else if(op==2){rr=r;cc=2-c;}
		else{rr=2-r;cc=c;}
		b[r*3+c]=a[rr*3+cc];
	}
}
bool same(int*a,int*b){for(int i=0;i<9;++i)if(a[i]!=b[i])return false;return true;}
bool seen(int*a){for(int i=0;i<scnt;++i)if(same(que[i],a))return true;return false;}
string bfs_solve(int*src,int*dst)
{
	for(int i=0;i<9;++i)que[0][i]=src[i];
	pat[0]="";scnt=1;
	for(int head=0;head<scnt;++head)
		for(int op=0;op<4;++op)
		{
			int nxt[9];apply(que[head],nxt,op);
			if(same(nxt,dst))return pat[head]+char('A'+op);
			if(!seen(nxt))
			{
				for(int i=0;i<9;++i)que[scnt][i]=nxt[i];
				pat[scnt]=pat[head]+char('A'+op);++scnt;
			}
		}
	return "Poland cannot into space!!!";
}

// ---- 独立暴力：按 (长度,字典序) 枚举所有序列，深度上限 6 ----
string brute_solve(int*src,int*dst)
{
	for(int len=1;len<=6;++len)
	{
		int tot=1;for(int i=0;i<len;++i)tot*=4;
		for(int code=0;code<tot;++code)
		{
			int cur[9],tmp[9];for(int i=0;i<9;++i)cur[i]=src[i];
			string s;int x=code;
			for(int i=0;i<len;++i){int d=x%4;x/=4;s+=char('A'+d);}
			reverse(s.begin(),s.end());	// 高位在前 => 字典序
			for(char ch:s){apply(cur,tmp,ch-'A');for(int i=0;i<9;++i)cur[i]=tmp[i];}
			if(same(cur,dst))return s;
		}
	}
	return "Poland cannot into space!!!";
}

int main()
{
	mt19937 rn(12345);
	int mismatch=0,reachable=0,unreachable=0,eqcase=0;
	for(int t=0;t<4000;++t)
	{
		int src[9],dst[9];
		int mode=t%4;
		for(int i=0;i<9;++i)src[i]=rn()%3;	// 小值域制造对称/碰撞
		if(mode==0){for(int i=0;i<9;++i)dst[i]=src[i];++eqcase;}	// src==dst
		else if(mode==1){for(int i=0;i<9;++i)dst[i]=rn()%3;}		// 独立随机，多半不可达
		else if(mode==2){int cur[9];for(int i=0;i<9;++i)cur[i]=src[i];int k=rn()%5;	// 可达：随机变换
			for(int j=0;j<k;++j){int tmp[9];apply(cur,tmp,rn()%4);for(int i=0;i<9;++i)cur[i]=tmp[i];}
			for(int i=0;i<9;++i)dst[i]=cur[i];}
		else{for(int i=0;i<9;++i)src[i]=7;for(int i=0;i<9;++i)dst[i]=7;}	// 全同
		string a=bfs_solve(src,dst),b=brute_solve(src,dst);
		if(a!=b){++mismatch;if(mismatch<=10){cerr<<"MISMATCH t="<<t<<"\n src:";for(int i=0;i<9;++i)cerr<<src[i];cerr<<"\n dst:";for(int i=0;i<9;++i)cerr<<dst[i];cerr<<"\n bfs="<<a<<" brute="<<b<<"\n";}}
		if(b=="Poland cannot into space!!!")++unreachable;else++reachable;
	}
	cerr<<"reachable="<<reachable<<" unreachable="<<unreachable<<" eqcase="<<eqcase<<" mismatch="<<mismatch<<"\n";
	return 0;
}