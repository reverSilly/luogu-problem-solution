#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 step(i64 v,int k)             // 用字符串补零,与标程的取模法互相独立
{
	string s=to_string(v);
	while((int)s.size()<k)
		s='0'+s;
	string a=s,b=s;
	sort(a.begin(),a.end());          // 升序 -> 较小数
	sort(b.rbegin(),b.rend());        // 降序 -> 较大数
	return stoll(b)-stoll(a);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	while(cin>>n)
	{
		string s=to_string(n);
		int k=max(1,(int)s.size());
		vector<i64>seq;
		i64 v=n;
		while(true)                   // 线性查找是否出现过
		{
			bool found=false;
			for(i64 x:seq)
				if(x==v)
				{
					found=true;break;
				}
			if(found)
				break;
			seq.push_back(v);
			v=step(v,k);
		}
		int st=0;
		for(int i=0;i<(int)seq.size();++i)
			if(seq[i]==v)
			{
				st=i;break;
			}
		for(int i=st;i<(int)seq.size();++i)
			cout<<seq[i]<<(i+1<(int)seq.size()?' ':'\n');
	}
	return 0;
}
