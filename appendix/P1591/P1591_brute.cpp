//
// P1591 朴素解：与标程同为大整数乘法, 但用 vector 反向存位、逐位短除取模
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>t;
	while(t--)
	{
		int n,a;
		cin>>n>>a;
		vector<int>v(1,1);       // 低位在前
		for(int i=2;i<=n;++i)
		{
			long long carry=0;
			for(size_t j=0;j<v.size();++j)
			{
				long long cur=v[j]*(long long)i+carry;
				v[j]=cur%10;
				carry=cur/10;
			}
			while(carry)
			{
				v.push_back(carry%10);
				carry/=10;
			}
		}
		int cnt=0;
		for(int x:v)
			if(x==a)
				++cnt;
		cout<<cnt<<'\n';
	}
	return 0;
}
