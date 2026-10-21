//
// P1618 朴素解：枚举 1..9 的全排列，前三/中三/后三位组成三数
// 用法：仅用于与 P1618.cpp 对拍，非提交代码
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	int A,B,C;
	cin>>A>>B>>C;
	int d[9]={1,2,3,4,5,6,7,8,9};
	vector<array<int,3>>res;
	do
	{
		int x=d[0]*100+d[1]*10+d[2];
		int y=d[3]*100+d[4]*10+d[5];
		int z=d[6]*100+d[7]*10+d[8];
		if((i64)x*B==(i64)y*A&&(i64)x*C==(i64)z*A)
			res.push_back({x,y,z});
	}while(next_permutation(d,d+9));
	sort(res.begin(),res.end());
	if(res.empty())
		cout<<"No!!!"<<'\n';
	else
		for(auto&[x,y,z]:res)
			cout<<x<<' '<<y<<' '<<z<<'\n';
	return 0;
}
