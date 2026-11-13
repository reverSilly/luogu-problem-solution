// 题目：P2383 狗哥玩木棒
// 思路：能被 4 整除且最长棒不超过 tot/4 后，问题变成"把木棒分成 4 组、每组和恰为 side"。
//       降序排序后 DFS 往四个桶里放，配合三个剪枝：
//       ① 容量剪枝 ② 等高桶等价（last）③ 空桶失败即整体失败（依赖"非空桶恒为前缀"）
//       另有缺口剪枝：剩余容量比最短剩余木棒还小，永远填不满。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int m,side,a[mxn],cap[4];

// 木棒已按长度降序排列，从第 idx 根开始往下放
bool dfs(int idx)
{
	if(idx==m)						// 全部放完且从未超限 ⇒ 四桶都恰好装满 side
		return true;
	int mn=a[m-1];					// 剩余木棒中最短的一根
	for(int k=0;k<4;++k)
	{
		int r=side-cap[k];
		if(r>0&&r<mn)				// 这个缺口剩下的木棒都填不满
			return false;
	}
	int s=a[idx],last=-1;			// last：刚试过并失败的桶高度，相同高度不再重试
	for(int k=0;k<4;++k)
	{
		if(cap[k]==last)			// 等高桶等价，跳过
			continue;
		if(cap[k]+s>side)
			continue;
		last=cap[k];
		cap[k]+=s;
		if(dfs(idx+1))
			return true;
		cap[k]-=s;
		if(cap[k]==0)				// 放进空桶都失败：其余空桶与它等价
			return false;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	while(n--)
	{
		cin>>m;
		int tot=0,mx=0;
		for(int i=0;i<m;++i)
		{
			cin>>a[i];
			tot+=a[i];
			mx=max(mx,a[i]);
		}
		if(tot%4!=0)				// 边长不是整数
		{
			cout<<"no\n";
			continue;
		}
		side=tot/4;
		if(mx>side)					// 最长棒放不进任何一边
		{
			cout<<"no\n";
			continue;
		}
		sort(a,a+m,greater<int>());	// 先放最受约束的大棒
		cap[0]=cap[1]=cap[2]=cap[3]=0;
		cout<<(dfs(0)?"yes\n":"no\n");
	}
	return 0;
}
