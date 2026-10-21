//
// 题目：P1611 循环的数字
// https://www.luogu.com.cn/problem/P1611
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int A,B;
int pw[10]{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B;
	i64 ans=0;
	for(int n=A;n<=B;++n)
	{
		int len=0;
		for(int t=n;t;t/=10)
			++len;
		int rot[10],c=0;
		for(int k=1;k<len;++k)             // 把末 k 位搬到前面
		{
			int hi=n/pw[k],lo=n%pw[k];
			if(lo<pw[k-1])
				continue;                  // 搬过去后首位是 0, 位数变了
			int m=lo*pw[len-k]+hi;
			if(m<=n||m>B)
				continue;
			bool dup=false;
			for(int j=0;j<c;++j)       // 周期串的不同旋转会重样, 去重
				if(rot[j]==m)
					dup=true;
			if(!dup)
				rot[c++]=m;
		}
		ans+=c;
	}
	cout<<ans<<'\n';
	return 0;
}
