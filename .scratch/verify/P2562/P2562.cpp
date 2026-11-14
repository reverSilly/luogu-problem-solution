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