#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 正解：倍长前缀和 + 单调队列滑动窗口最小值
int solve(int n,vector<int>&a)
{
	vector<i64>P(2*n+1,0);
	for(int i=1;i<=2*n;++i)P[i]=P[i-1]+a[(i-1)%n];
	vector<int>dq(2*n+1);int head=0,tail=0,ans=0;
	for(int i=1;i<2*n;++i)
	{
		while(head<tail&&P[dq[tail-1]]>=P[i])--tail;
		dq[tail++]=i;
		while(head<tail&&dq[head]<i-n+1)++head;
		if(i>=n&&P[dq[head]]>=P[i-n])++ans;
	}
	return ans;
}

// 暴力：枚举每个旋转起点，逐条通报检查心情是否 <0
int brute(int n,vector<int>&a)
{
	int ans=0;
	for(int k=0;k<n;++k)
	{
		i64 mood=0;bool ok=true;
		for(int j=0;j<n;++j)
		{
			mood+=a[(k+j)%n];
			if(mood<0){ok=false;break;}
		}
		if(ok)++ans;
	}
	return ans;
}

int main()
{
	mt19937 rn(999);
	int T=5000,mis=0;
	for(int tc=0;tc<T;++tc)
	{
		int n=1+rn()%9;
		vector<int>a(n);
		for(int i=0;i<n;++i)a[i]=(int)(rn()%9)-4;	// [-4,4]
		int s=solve(n,a),b=brute(n,a);
		if(s!=b)
		{
			++mis;
			if(mis<=5)
			{
				cerr<<"MISMATCH n="<<n<<" s="<<s<<" b="<<b<<" a:";
				for(int x:a)cerr<<x<<" ";
				cerr<<"\n";
			}
		}
	}
	cout<<"tests="<<T<<" mismatches="<<mis<<"\n";
	return 0;
}