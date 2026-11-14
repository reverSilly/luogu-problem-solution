#include<bits/stdc++.h>
using namespace std;
// 独立模拟：直接用向量淘汰实现约瑟夫(k=2,先淘汰2号)，再按题意发金币，验证 N+k_final 公式
int josim(int k)	// 返回幸存者编号(1..k)，不用 2l+1 公式
{
	vector<int>p(k);
	iota(p.begin(),p.end(),1);
	int cur=0;	// 从1号开始报数
	while((int)p.size()>1)
	{
		int kill=(cur+1)%p.size();	// 当前报1，下一个报2被淘汰
		p.erase(p.begin()+kill);
		cur=kill%p.size();
	}
	return p[0];
}
int main()
{
	int n;
	cin>>n;
	int k=n,total=0;
	while(true)
	{
		int s=josim(k);
		if(s==k)	// 人数不再减少
		{
			total+=2*k;
			break;
		}
		total+=k-s;	// 编号>s 的人各得1金币离开
		k=s;
	}
	cout<<total<<'\n';
	return 0;
}