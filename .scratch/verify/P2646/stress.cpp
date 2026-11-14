#include<bits/stdc++.h>
using namespace std;
using i64=long long;
// 临时对拍：O(L^3) 暴力枚举三元组 vs O(L) 三计数器 DP
mt19937 rn(999);
i64 fast(const string&s)
{
	i64 cz=0,czz=0,czzy=0;
	for(char c:s){if(c=='y')czzy+=czz;else if(c=='z'){czz+=cz;++cz;}}
	return czzy;
}
i64 brute(const string&s)
{
	i64 cnt=0;int L=s.size();
	for(int i=0;i<L;++i)for(int j=i+1;j<L;++j)for(int k=j+1;k<L;++k)
		if(s[i]=='z'&&s[j]=='z'&&s[k]=='y')++cnt;
	return cnt;
}
int main()
{
	for(int it=0;it<200000;++it)
	{
		int L=1+rn()%12;string s;
		for(int i=0;i<L;++i)s+=char('a'+rn()%26);
		// 偏向 z/y 以制造更多命中
		for(int i=0;i<L;++i)if(rn()%2)s[i]=(rn()%2?'z':'y');
		i64 f=fast(s),b=brute(s);
		if(f!=b){printf("MISMATCH s=%s fast=%lld brute=%lld\n",s.c_str(),f,b);return 1;}
	}
	puts("P2646 stress OK: 200000 cases, 0 mismatch");
	return 0;
}