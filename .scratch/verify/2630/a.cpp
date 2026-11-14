// 题目：P2630 图像变换
// 思路：A/B/C/D 都是正方形对称 ⇒ 生成群 D4 只有 8 元 ⇒ 可达图像至多 8 张。
//       10^8 是烟雾弹：有解时最短序列 <=7（实际 <=2）。
//       于是 BFS：按 A,B,C,D 字母序扩展，首次生成目标即"最短且字典序最小"。
//       "非空"：只在生成后继==目标时接受（源点空串从不接受），t==s 时自动取长度 1/2 回归。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxs{13};			// 轨道至多 8 个状态，+5 松弛

int src[9],dst[9];
int que[mxs][9];
string pat[mxs];
int scnt;

void apply(int*a,int*b,int op)	// op:0=A顺时针 1=B逆时针 2=C左右 3=D上下
{
	for(int r=0;r<3;++r)
		for(int c=0;c<3;++c)
		{
			int rr,cc;
			if(op==0){rr=2-c;cc=r;}		// new[r][c]=old[2-c][r]
			else if(op==1){rr=c;cc=2-r;}	// new[r][c]=old[c][2-r]
			else if(op==2){rr=r;cc=2-c;}	// new[r][c]=old[r][2-c]
			else{rr=2-r;cc=c;}		// new[r][c]=old[2-r][c]
			b[r*3+c]=a[rr*3+cc];
		}
}

bool same(int*a,int*b)
{
	for(int i=0;i<9;++i)
		if(a[i]!=b[i])return false;
	return true;
}

bool seen(int*a)
{
	for(int i=0;i<scnt;++i)
		if(same(que[i],a))return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<9;++i)cin>>src[i];
	for(int i=0;i<9;++i)cin>>dst[i];

	for(int i=0;i<9;++i)que[0][i]=src[i];
	pat[0]="";scnt=1;
	for(int head=0;head<scnt;++head)
		for(int op=0;op<4;++op)	// 按 A,B,C,D 字典序扩展
		{
			int nxt[9];
			apply(que[head],nxt,op);
			if(same(nxt,dst))	// 首次生成目标即最短且字典序最小（天然非空）
			{
				cout<<pat[head]+char('A'+op)<<'\n';
				return 0;
			}
			if(!seen(nxt))
			{
				for(int i=0;i<9;++i)que[scnt][i]=nxt[i];
				pat[scnt]=pat[head]+char('A'+op);
				++scnt;
			}
		}
	cout<<"Poland cannot into space!!!\n";
	return 0;
}
