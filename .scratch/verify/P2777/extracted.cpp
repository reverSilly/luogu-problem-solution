// 题目：P2777 [AHOI2016初中组] 自行车比赛
// 思路：选手 i 能夺冠 <=> 给他 n 分、其余人按"分高者得低分"反序分配 1..n-1 后，
//       他的总分仍严格大于其他人最大总分（交换论证两步）。
//       升序排序后令 f[j]=b[j]+n-1-j，则其他人最大值 M_i=max(pre[i],suf[i+1]+1)，
//       pre/suf 为 f 的前缀/后缀最大值 ⇒ 每个候选人 O(1) 判定。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};
constexpr int inf{1145141919};

int n,b[mxn],f[mxn],pre[mxn],suf[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>b[i];
	sort(b,b+n);
	for(int i=0;i<n;++i)
		f[i]=b[i]+n-1-i;	// 反序分配下升序第 i 人的"虚拟总分"
	pre[0]=-inf;			// 空集最大值 = -inf，不可用全局默认 0！
	for(int i=1;i<n;++i)
		pre[i]=max(pre[i-1],f[i-1]);
	suf[n]=-inf;
	for(int i=n-1;i>=0;--i)
		suf[i]=max(suf[i+1],f[i]);
	int ans=0;
	for(int i=0;i<n;++i)
		if(b[i]+n>max(pre[i],suf[i+1]+1))	// 严格大于 ⇒ 唯一冠军
			++ans;
	cout<<ans<<'\n';
	return 0;
}
