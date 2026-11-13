#include<bits/stdc++.h>
using namespace std;

// 暴力：枚举每根木棒放进 4 个桶中的哪一个，只做容量剪枝，无任何对称性剪枝
int m,a[25],side,s[4];

bool dfs(int i)
{
	if(i==m)
		return s[0]==side&&s[1]==side&&s[2]==side&&s[3]==side;
	for(int k=0;k<4;++k)
		if(s[k]+a[i]<=side)
		{
			s[k]+=a[i];
			if(dfs(i+1))
				return true;
			s[k]-=a[i];
		}
	return false;
}

int main()
{
	int n;
	cin>>n;
	while(n--)
	{
		cin>>m;
		int tot=0;
		for(int i=0;i<m;++i)
		{
			cin>>a[i];
			tot+=a[i];
		}
		s[0]=s[1]=s[2]=s[3]=0;
		if(tot%4!=0)
		{
			cout<<"no\n";
			continue;
		}
		side=tot/4;
		cout<<(dfs(0)?"yes\n":"no\n");
	}
	return 0;
}
