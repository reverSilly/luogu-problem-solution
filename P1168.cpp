#include<bits/stdc++.h>
using namespace std;

priority_queue<int>L;							// 较小的一半，大根堆
priority_queue<int,vector<int>,greater<>>R;		// 较大的一半，小根堆

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,x;
	cin>>n;
	for(int i=1;i<=n;++i)
	{
		cin>>x;
		L.push(x);
		R.push(L.top());
		L.pop();
		if(L.size()<R.size())
		{
			L.push(R.top());
			R.pop();
		}
		if(i&1)
			cout<<L.top()<<'\n';
	}
	return 0;
}
