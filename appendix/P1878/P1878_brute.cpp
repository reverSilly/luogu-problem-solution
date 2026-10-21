//
// 题目：P1878 舞蹈课
// https://www.luogu.com.cn/problem/P1878
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

struct pe
{
	int id,a;
	char g;
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	string s;
	cin>>s;
	vector<pe> v(n);
	for(int i=0;i<n;++i)
	{
		v[i].id=i+1;
		v[i].g=s[i];
		cin>>v[i].a;
	}
	vector<pair<int,int>> ans;
	while(true)
	{
		int bi=-1,bd=1145141919;
		int sz=(int)v.size();
		for(int i=0;i+1<sz;++i)
		{
			if(v[i].g==v[i+1].g)
				continue;
			int d=v[i].a-v[i+1].a;
			if(d<0)
				d=-d;
			if(d<bd)
			{
				bd=d;
				bi=i;
			}
		}
		if(bi<0)
			break;
		int x=v[bi].id,y=v[bi+1].id;
		if(x>y)
			swap(x,y);
		ans.push_back({x,y});
		v.erase(v.begin()+bi,v.begin()+bi+2);
	}
	cout<<ans.size()<<'\n';
	for(auto [x,y]:ans)
		cout<<x<<' '<<y<<'\n';
	return 0;
}
