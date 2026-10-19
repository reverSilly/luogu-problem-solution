// 题目：P1517 [ECNA 1988] 高精求小数幂
// https://www.luogu.com.cn/problem/P1517
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1505};      // P^n 位数 <= 5*250=1250 (+5 松弛)

int dig[mxn];                 // 大数(十进制小端;dig[0] 为个位)
int len;                      // 当前位数

void mul(int m)               // 大数 *= 小整数 m
{
	int carry=0;
	for(int i=0;i<len;++i)
	{
		i64 cur=1ll*dig[i]*m+carry;
		dig[i]=cur%10;
		carry=cur/10;
	}
	while(carry)
	{
		dig[len++]=carry%10;
		carry/=10;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	int n;
	while(cin>>s>>n)                    // 读到文件末尾(多组数据)
	{
		int P=0,k=0;                    // P=去掉小数点后的整数, k=小数位数
		bool dot=false;
		for(char c:s)
		{
			if(c=='.')
				dot=true;
			else if(c>='0'&&c<='9')
			{
				P=P*10+(c-'0');
				if(dot)
					++k;                // 只统计小数点之后的位数
			}
		}
		len=1;dig[0]=1;
		for(int i=0;i<n;++i)
			mul(P);                     // R^n = P^n / 10^(k*n)
		string S;
		for(int i=len-1;i>=0;--i)
			S.push_back(char('0'+dig[i]));
		int e=k*n;                      // 结果的小数位数
		if(e==0)                        // 整数,不带小数点
		{
			cout<<S<<'\n';
			continue;
		}
		string ip,fp;
		if(len<=e)
			fp=string(e-len,'0')+S;     // 整数部分为 0(结果<1),省略
		else
		{
			ip=S.substr(0,len-e);
			fp=S.substr(len-e);
		}
		while(!fp.empty()&&fp.back()=='0')
			fp.pop_back();              // 小数末尾去 0
		if(ip.empty())                  // 结果 <1 时省略整数 0
			cout<<'.'<<fp<<'\n';
		else if(fp.empty())
			cout<<ip<<'\n';
		else
			cout<<ip<<'.'<<fp<<'\n';
	}
	return 0;
}
