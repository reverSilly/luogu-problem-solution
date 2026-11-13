#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(chrono::steady_clock::now().time_since_epoch().count());
	int n=rn()%60+1;
	cout<<n<<'\n';
	vector<int>p(80);
	iota(p.begin(),p.end(),1);
	shuffle(p.begin(),p.end(),rn);
	for(int i=0;i<n;++i)
		cout<<rn()%30+1<<' '<<p[i]<<'\n';
	return 0;
}
