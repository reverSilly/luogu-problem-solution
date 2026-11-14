// 题目：P2562 [AHOI2002] Kitty猫基因编码
// 思路：ABC 编码是分治递归——全 0→A，全 1→B，混合→C+左半编码+右半编码。
//       半开区间 [l,r)，先序往全局 ans 追加，顺序恰是 C+左+右。
//       长度是 2 的幂 ⇒ 劈半整除；单字符必为纯串 ⇒ 递归一定终止。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{260};

string s,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int n=s.size();
	function<void(int,int)>sth{[&](int l,int r){	// [l,r) 半开区间
		bool all0=true,all1=true;
		for(int i=l;i<r;++i)
		{
			if(s[i]!='0')all0=false;
			if(s[i]!='1')all1=false;
		}
		if(all0){ans+='A';return;}
		if(all1){ans+='B';return;}
		ans+='C';
		int mid=(l+r)>>1;
		sth(l,mid);
		sth(mid,r);
	}};
	sth(0,n);
	cout<<ans<<'\n';
	return 0;
}
