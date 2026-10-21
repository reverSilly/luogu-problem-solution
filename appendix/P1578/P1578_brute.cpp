#include<bits/stdc++.h>
using namespace std;

int main()
{
	int L,W,n;
	cin>>L>>W>>n;
	vector<pair<int,int>> pts(n);
	vector<int> xs={0,L},ys={0,W};
	for(int i=0;i<n;++i)
	{
		cin>>pts[i].first>>pts[i].second;
		xs.push_back(pts[i].first);
		ys.push_back(pts[i].second);
	}
	sort(xs.begin(),xs.end());
	xs.erase(unique(xs.begin(),xs.end()),xs.end());
	sort(ys.begin(),ys.end());
	ys.erase(unique(ys.begin(),ys.end()),ys.end());

	long long ans=0;
	for(size_t i=0;i<xs.size();++i)
		for(size_t j=i+1;j<xs.size();++j)
		{
			int x1=xs[i],x2=xs[j];
			// 收集落入开区间 (x1, x2) 的点的 y 坐标
			vector<int> mid_y={0,W};
			for(auto [px,py]:pts)
				if(px>x1&&px<x2)
					mid_y.push_back(py);
			sort(mid_y.begin(),mid_y.end());
			for(size_t k=0;k+1<mid_y.size();++k)
				ans=max(ans,1LL*(x2-x1)*(mid_y[k+1]-mid_y[k]));
		}
	cout<<ans<<'\n';
	return 0;
}
