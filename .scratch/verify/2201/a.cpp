#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr i64 ninf{-1145141919810};

int a[mxn],b[mxn];   // a:光标左侧的栈(下标0..lt-1)  b:光标右侧的栈(栈顶是光标后第一个数)
i64 f[mxn];          // f[i]=光标左侧前i个数的最大前缀和
i64 sum;             // 光标左侧所有数之和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int lt=0,rt=0;
	f[0]=ninf;
	for(int i=0;i<n;++i)
	{
		char op;
		cin>>op;
		if(op=='I')
		{
			int x;
			cin>>x;
			a[lt++]=x;
			sum+=x;
			f[lt]=max(f[lt-1],sum);
		}
		else if(op=='D')
		{
			sum-=a[--lt];
		}
		else if(op=='L')
		{
			if(lt)
			{
				b[rt++]=a[--lt];
				sum-=b[rt-1];
			}
		}
		else if(op=='R')
		{
			if(rt)
			{
				a[lt++]=b[--rt];
				sum+=a[lt-1];
				f[lt]=max(f[lt-1],sum);
			}
		}
		else
		{
			int k;
			cin>>k;
			cout<<f[k]<<'\n';
		}
	}
	return 0;
}
