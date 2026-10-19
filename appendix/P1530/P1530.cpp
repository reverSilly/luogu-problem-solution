//
// 题目：P1530 [USACO2.4] 分数化小数 Fractions to Decimals
// https://www.luogu.com.cn/problem/P1530
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // 余数 < D <= 1e5 (+5 松弛)

int n,d;
int pos[mxn];                     // pos[r]=余数 r 首次出现时的小数位下标, -1 表示未出现

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>d;
	int ip=n/d,rem=n%d;           // 整数部分 / 首个余数
	string s;                     // 小数数字串(先不含括号)
	fill_n(&pos[0],mxn,-1);
	while(rem&&pos[rem]==-1)      // 余数为 0 或余数重复则停
	{
		pos[rem]=s.size();
		rem*=10;
		s.push_back(char('0'+rem/d));
		rem%=d;
	}
	string full;
	if(rem==0)                    // 有限小数
	{
		if(s.empty())
			s="0";                // 整除:整数 x 写作 x.0
		full=to_string(ip)+"."+s;
	}
	else                          // 无限循环小数:在循环节起点插括号
	{
		int st=pos[rem];
		full=to_string(ip)+"."+s.substr(0,st)+"("+s.substr(st)+")";
	}
	for(int i=0;i<(int)full.size();i+=76)
		cout<<full.substr(i,76)<<'\n';
	return 0;
}
