// 题目：P2459 [SDOI2007] 立体分割
// 输出允许实数坐标（误差 1e-9），切割不必落在整点：
// 沿 x 轴切成 n 片等厚薄板，第 i 片为 [i*x/n,(i+1)*x/n]×[0,y]×[0,z]
#include<bits/stdc++.h>
using namespace std;

int x,y,z,n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y>>z>>n;
	cout<<fixed<<setprecision(12);
	for(int i=0;i<n;++i)
		cout<<(double)i*x/n<<' '<<0<<' '<<0<<' '<<(double)(i+1)*x/n<<' '<<y<<' '<<z<<'\n';
	return 0;
}
