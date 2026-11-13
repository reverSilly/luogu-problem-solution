// 题目：P2348 三国杀I（洗牌&发牌）
// 思路：洗牌是「下半叠与上半叠交替」的固定置换，奇偶位置分别对应两段下标；
//       只对下标数组模拟 m 次洗牌，再按 (p-1)+t*n 取第 p 位玩家的 4 张牌。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,k,m,p;
string suit[mxn],typ[mxn];
int cur[mxn],nxt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>m>>p;
	for(int i=0;i<k;++i)
	{
		cin>>suit[i]>>typ[i];
		cur[i]=i;
	}
	if(k<4*n)
	{
		cout<<"Error:cards not enough\n";
		return 0;
	}
	int h=k/2;
	for(int t=0;t<m;++t)
	{
		for(int d=0;d<k;++d)
			nxt[d]=(d&1)?cur[d/2]:cur[h+d/2];	//0-based：偶数下标取下半叠，奇数下标取上半叠
		for(int d=0;d<k;++d)
			cur[d]=nxt[d];
	}
	for(int t=0;t<4;++t)
	{
		int id=cur[(p-1)+t*n];
		cout<<suit[id]<<' '<<typ[id]<<'\n';
	}
	return 0;
}
