// 题目：P2393 yyy loves Maths II
// 每个数最多 6 位小数 ⇒ 统一乘 1e6 化成整数，全程整数运算，零浮点误差
// 5e4 个数的缩放和最大 1.1667e19 > long long 上限 ⇒ 用 __int128 累加
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128;

constexpr int mxn{64};	// 单个数最长 1+9+1+6=17 字符，留足松弛

char buf[mxn];
i128 sum;

// 把十进制串精确放大 1e6 倍成整数（符号单独处理）
i128 parse(const char*s)
{
	int i=0,sgn=1;
	if(s[0]=='-')sgn=-1,++i;
	else if(s[0]=='+')++i;
	i64 ip=0;
	for(;s[i]&&s[i]!='.';++i)
		ip=ip*10+(s[i]-'0');
	i64 fp=0;
	if(s[i]=='.')
	{
		++i;
		for(int k=0;k<6;++k)	// 不足 6 位补 0，超过 6 位题目保证不存在
		{
			fp*=10;
			if(s[i]>='0'&&s[i]<='9')
				fp+=s[i++]-'0';
		}
	}
	return sgn*(i128)(ip*1000000+fp);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>buf)
		sum+=parse(buf);
	// sum 以 1e-6 为单位；四舍五入到 5 位小数 = 除以 10 后按 |余数|>=5 进位
	i128 q=sum/10,r=sum%10;
	if(r>=5)++q;
	else if(r<=-5)--q;
	bool neg=q<0;
	if(neg)q=-q;
	cout<<(neg?"-":"")<<(i64)(q/100000)<<'.'<<setw(5)<<setfill('0')<<(i64)(q%100000)<<'\n';
	return 0;
}
