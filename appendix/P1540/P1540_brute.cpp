// 朴素暴力:用 vector 直接线性扫描内存,内存满则删除最早进入的元素
#include<bits/stdc++.h>
using namespace std;

int m,n;

int main()
{
	if(!(cin>>m>>n))
		return 0;
	vector<int> mem;                 // 按进入顺序存放内存中的单词
	int ans=0;
	for(int i=0;i<n;++i)
	{
		int x;
		cin>>x;
		bool found=false;
		for(int v:mem)
			if(v==x)
				found=true;
		if(found)
			continue;
		++ans;
		if((int)mem.size()==m)
			mem.erase(mem.begin());   // 移出最早进入的单词
		mem.push_back(x);
	}
	cout<<ans<<'\n';
	return 0;
}
