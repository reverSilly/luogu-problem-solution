#include<bits/stdc++.h>
using namespace std;

//暴力：用链表存剩余果子，迭代器一步一步往上爬
int main()
{
	int n,m;
	cin>>n>>m;
	list<int>fr;
	for(int i=1;i<=n;++i)
		fr.push_back(i);
	//cur：松鼠当前脚下那颗果子（链表中的迭代器）
	list<int>::iterator cur=fr.begin();
	int ans=0;
	for(int i=1;i<=m;++i)
	{
		int d=i*i*i%5+1;
		list<int>::iterator tar=cur;
		int step=0;
		while(step<d&&tar!=fr.end())
		{
			++tar;
			++step;
		}
		if(tar==fr.end())
		{
			//跳出最前面：回到最下面的果子，重做第 i 跳
			cur=fr.begin();
			tar=cur;
			step=0;
			while(step<d&&tar!=fr.end())
			{
				++tar;
				++step;
			}
		}
		if(tar==fr.end())
		{
			//数据保证能跳到第 m 次，此分支不会出现
			ans=0;
			break;
		}
		ans=*tar;
		list<int>::iterator nxt=tar;
		++nxt;
		fr.erase(tar);
		cur=nxt;
	}
	cout<<ans<<'\n';
	return 0;
}
