//
// P1611 暴力（对拍用）：枚举所有 (n,m) 对, 用字符串判断 m 是否为 n 的循环移位
#include<bits/stdc++.h>
using namespace std;

int main()
{
	int A,B;
	cin>>A>>B;
	long long ans=0;
	for(int n=A;n<=B;++n)
		for(int m=n+1;m<=B;++m)
		{
			string a=to_string(n),b=to_string(m);
			if(a.size()!=b.size())
				continue;
			bool ok=false;
			for(size_t k=1;k<a.size()&&!ok;++k)
				if(a.substr(k)+a.substr(0,k)==b)
					ok=true;
			if(ok)
				++ans;
		}
	cout<<ans<<'\n';
	return 0;
}
