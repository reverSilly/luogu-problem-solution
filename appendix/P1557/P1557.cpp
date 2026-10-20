//
// 题目：P1557 Kruscal的加法
// https://www.luogu.com.cn/problem/P1557
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxd{6005};          // 结果位数上界:(2000 位数字)*(2000 倍)+松弛
constexpr int mxn{2005};          // 算式长度上界 (+5 松弛)

int S[mxd],si;                    // 累加和(低位在前);si==0 表示值 0
bool sneg;                        // 累加和符号,true 为负
int T[mxd],ti;                    // 单项"数值*系数"暂存(低位在前);ti==0 表示 0

void muls(i64 k)                  // T *= k,原地
{
	i64 c=0;
	for(int i=0;i<ti;++i)
	{
		i64 v=T[i]*k+c;
		T[i]=v%10;
		c=v/10;
	}
	while(c)
	{
		T[ti++]=c%10;
		c/=10;
	}
	while(ti>1&&T[ti-1]==0)
		--ti;
	if(ti==1&&T[0]==0)
		ti=0;
}
int cmpv(int*a,int an,int*b,int bn)   // 比较两个非负大数,返回 -1/0/1
{
	if(an!=bn)
		return an<bn?-1:1;
	for(int i=an-1;i>=0;--i)
		if(a[i]!=b[i])
			return a[i]<b[i]?-1:1;
	return 0;
}
void acc(bool tneg)               // 把 T(符号 tneg)累加进 S
{
	if(ti==0)
		return;                       // 加 0,无变化
	if(si==0)                         // S 尚为 0,直接落座
	{
		for(int i=0;i<ti;++i)
			S[i]=T[i];
		si=ti;
		sneg=tneg;
		return;
	}
	if(sneg==tneg)                    // 同号:绝对值相加,符号不变
	{
		int m=max(si,ti),c=0;
		for(int i=0;i<m;++i)
		{
			int v=(i<si?S[i]:0)+(i<ti?T[i]:0)+c;
			S[i]=v%10;
			c=v/10;
		}
		if(c)
			S[m++]=c;
		si=m;
	}
	else                              // 异号:大减小,符号取绝对值较大者
	{
		int c=cmpv(S,si,T,ti);
		if(c==0)
		{
			si=0;
			sneg=false;
			return;
		}
		if(c>0)                       // S 更大:S -= T,符号保持
		{
			int bo=0;
			for(int i=0;i<si;++i)
			{
				int v=S[i]-(i<ti?T[i]:0)-bo;
				if(v<0)
				{
					v+=10;
					bo=1;
				}
				else
					bo=0;
				S[i]=v;
			}
			while(si>1&&S[si-1]==0)
				--si;
		}
		else                          // T 更大:S = T - S,符号翻转为 tneg
		{
			int bo=0;
			for(int i=0;i<ti;++i)
			{
				int v=T[i]-(i<si?S[i]:0)-bo;
				if(v<0)
				{
					v+=10;
					bo=1;
				}
				else
					bo=0;
				S[i]=v;
			}
			si=ti;
			while(si>1&&S[si-1]==0)
				--si;
			sneg=tneg;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	cin>>s;
	int n=(int)s.size(),i=0;
	while(i<n)
	{
		i64 coeff{1};int tneg{0};
		if(s[i]=='+'||s[i]=='-')
		{
			int j=i,plus=0,minus=0;
			while(j<n&&(s[j]=='+'||s[j]=='-'))
			{
				if(s[j]=='+')
					++plus;
				else
					++minus;
				++j;
			}
			if(j==i+1&&j<n&&s[j]=='(')      // 括号型:+(k) / -(k)
			{
				tneg=(s[i]=='-');
				int k=j+1;i64 c{0};
				while(k<n&&isdigit((unsigned char)s[k]))
				{
					c=c*10+(s[k]-'0');
					++k;
				}
				if(k<n&&s[k]==')')
					++k;
				coeff=c;
				i=k;
			}
			else                            // 连续同号型:系数=净符号数
			{
				int net=plus-minus;
				if(net==0)
				{
					coeff=0;
					tneg=0;
				}
				else
				{
					tneg=(net<0);
					coeff=abs(net);
				}
				i=j;
			}
		}
		ti=0;
		while(i<n&&isdigit((unsigned char)s[i]))
		{
			T[ti++]=s[i]-'0';
			++i;
		}
		for(int a=0,b=ti-1;a<b;++a,--b)
			swap(T[a],T[b]);              // 读入为高位在前,翻成低位在前
		muls(coeff);
		acc(tneg);
	}

	if(si==0)
	{
		cout<<0<<'\n';
		return 0;
	}
	if(sneg)
		cout<<'-';
	for(int i=si-1;i>=0;--i)
		cout<<(char)('0'+S[i]);
	cout<<'\n';
	return 0;
}
