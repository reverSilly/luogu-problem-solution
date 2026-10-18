#include<bits/stdc++.h>
using namespace std;

int n[7];

int main()
{
	int tc=0;
	while(cin>>n[1]>>n[2]>>n[3]>>n[4]>>n[5]>>n[6])
	{
		int sum=0;
		for(int i=1;i<=6;++i)
			sum+=i*n[i];
		if(sum==0)
			break;
		++tc;
		cout<<"Collection #"<<tc<<":\n";
		if(sum&1)
		{
			cout<<"Can't be divided.\n\n";
			continue;
		}
		int target=sum/2;
		vector<int> dp(target+1,0);
		dp[0]=1;
		for(int i=1;i<=6;++i)
			for(int c=0;c<n[i];++c)
				for(int w=target;w>=i;--w)
					if(dp[w-i])
						dp[w]=1;
		if(dp[target])
			cout<<"Can be divided.\n\n";
		else
			cout<<"Can't be divided.\n\n";
	}
	return 0;
}
