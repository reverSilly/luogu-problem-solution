// 题目：P2728 [USACO3.2] 纺车的轮子 Spinning Wheels
// 思路：整数秒 t 时第 i 轮缺口角度 = (theta+d+omega_i*t)%360，宽度 phi 含两端共 phi+1 个角度。
//       用 bitset<360> 标记每轮透光角度集合，五轮取交，非空即有光通过。
//       配置以 360 秒为周期（omega*360 ≡ 0 mod 360），故只需枚举 t=0..359，否则 none。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxw{5},mxg{6},mxd{360};

int w[mxw],ng[mxw],th[mxw][mxg],ph[mxw][mxg];
bitset<mxd> cur;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<5;++i)
	{
		cin>>w[i]>>ng[i];
		for(int j=0;j<ng[i];++j)
			cin>>th[i][j]>>ph[i][j];
	}
	for(int t=0;t<360;++t)	// 周期 360：t 与 t+360 各轮位置完全相同
	{
		cur.set();	// 全 1，准备逐轮取交
		for(int i=0;i<5;++i)
		{
			bitset<mxd> b;	// 默认全 0：本轮当前不透光
			for(int j=0;j<ng[i];++j)
				for(int d=0;d<=ph[i][j];++d)	// 宽度含两端 ⇒ d 取 0..phi
					b.set((th[i][j]+d+w[i]*t)%360);
			cur&=b;
		}
		if(cur.any())
		{
			cout<<t<<'\n';
			return 0;
		}
	}
	cout<<"none\n";
	return 0;
}