// 题目：P2442 分数统计
// 思路：值域只有 100。把「值」这一维提到最外层离线扫描，
//       复用同一个前缀和数组 c[]，内存从 100n 塌缩到 n。
//       升序 + 严格大于 ⇒ 众数并列自动取最小；
//       cnt>0 ⇔ 该值出现（b_i>=1）⇒ 同一扫描顺手求出极差。
//       平均数全程整数：t=(100*S)/B，输出 t/100 . t%100（截断，非四舍五入）。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005},inf{1145141919};

int n,q,a[mxn],b[mxn],sb[mxn],c[mxn];
i64 s[mxn],ans[mxn];
struct{int o,u,v;}Q[mxn];
int bc[mxn],mn[mxn],mx[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=0;i<n;++i)cin>>a[i];
	for(int i=0;i<n;++i)cin>>b[i];
	for(int i=0;i<n;++i)
	{
		s[i+1]=s[i]+(i64)a[i]*b[i];//总分
		sb[i+1]=sb[i]+b[i];//总人数
	}
	for(int i=0;i<q;++i)
	{
		cin>>Q[i].o>>Q[i].u>>Q[i].v;
		--Q[i].u;--Q[i].v;
		if(Q[i].u>Q[i].v)swap(Q[i].u,Q[i].v);
		mn[i]=inf;mx[i]=0;bc[i]=-1;//累加器只能在这里初始化一次
		if(Q[i].o==1)
		{
			i64 tot=s[Q[i].v+1]-s[Q[i].u],num=sb[Q[i].v+1]-sb[Q[i].u];
			ans[i]=tot*100/num;//截断到两位小数后放大 100 倍，全程整数
		}
	}
	//离线：按评价值 x 从小到大扫，c[] 复用为「值为 x 的人数」前缀和
	for(int x=1;x<=100;++x)
	{
		c[0]=0;
		for(int i=0;i<n;++i)c[i+1]=c[i]+(a[i]==x?b[i]:0);
		for(int i=0;i<q;++i)
		{
			if(Q[i].o==1)continue;
			int cnt=c[Q[i].v+1]-c[Q[i].u];
			if(Q[i].o==2)
			{
				if(cnt>bc[i]){bc[i]=cnt;ans[i]=x;}//严格大于 ⇒ 并列时保留最小的 x
			}
			else if(cnt>0)
			{
				if(mn[i]==inf)mn[i]=x;//第一个出现的 x 就是最小值
				mx[i]=x;//最后一个出现的 x 就是最大值
			}
		}
	}
	for(int i=0;i<q;++i)
	{
		if(Q[i].o==1)
		{
			i64 v=ans[i];
			cout<<v/100<<'.'<<(v%100<10?"0":"")<<v%100<<'\n';
		}
		else if(Q[i].o==2)cout<<ans[i]<<'\n';
		else cout<<mx[i]-mn[i]<<'\n';
	}
	return 0;
}
