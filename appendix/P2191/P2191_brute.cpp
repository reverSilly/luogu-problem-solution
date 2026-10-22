// P2191 小Z的情书
// 暴力：与标程同样只旋转透明纸，但用"顺时针写回"的坐标公式实现旋转，与标程的转置写法相互独立
#include<bits/stdc++.h>
using namespace std;

int n;
char msk[1005][1005],tx[1005][1005];

int main()
{
	scanf("%d",&n);
	for(int i=0;i<n;++i)
		scanf("%s",msk[i]);
	for(int i=0;i<n;++i)
		scanf("%s",tx[i]);
	string ans;
	for(int t=0;t<4;++t)
	{
		// 按当前朝向的行优先顺序读出 O 位置对应的字母
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(msk[i][j]=='O')
					ans+=tx[i][j];
		// 顺时针旋转 90 度：dst[j][n-1-i] = src[i][j]
		char tmp[1005][1005];
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				tmp[j][n-1-i]=msk[i][j];
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				msk[i][j]=tmp[i][j];
	}
	printf("%s\n",ans.c_str());
	return 0;
}
