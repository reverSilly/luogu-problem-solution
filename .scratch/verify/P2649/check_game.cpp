#include<bits/stdc++.h>
using namespace std;
// m=2 完整博弈枚举：max-min 与 min-max 都应等于 n-max_beaten
int n,A[8],B[8];
int greedy()
{
	sort(A,A+n);sort(B,B+n);
	int j=0,beat=0;
	for(int i=0;i<n;++i){while(j<n&&B[j]<=A[i])++j;if(j<n){++beat;++j;}}
	return n-beat;
}
int wins(const vector<int>&jp,const vector<int>&op){int w=0;for(int i=0;i<n;++i)if(jp[i]>op[i])++w;return w;}
int main()
{
	mt19937 rn(777);
	int mismatch=0;
	for(int t=0;t<400;++t)
	{
		n=2+rn()%4; // 2..5
		vector<int>pool(2*n);iota(pool.begin(),pool.end(),1);
		shuffle(pool.begin(),pool.end(),rn);
		for(int i=0;i<n;++i)A[i]=pool[i];
		for(int i=0;i<n;++i)B[i]=pool[n+i];
		vector<int>ja(A,A+n),ob(B,B+n);
		sort(ja.begin(),ja.end());sort(ob.begin(),ob.end());
		vector<vector<int>>JP,OP;
		do{JP.push_back(ja);}while(next_permutation(ja.begin(),ja.end()));
		do{OP.push_back(ob);}while(next_permutation(ob.begin(),ob.end()));
		// max-min: John 先手, 对手最优响应
		int maxmin=0;
		for(auto&jp:JP){int mn=n;for(auto&op:OP)mn=min(mn,wins(jp,op));maxmin=max(maxmin,mn);}
		// min-max: 对手先手, John 最优响应
		int minmax=n;
		for(auto&op:OP){int mx=0;for(auto&jp:JP)mx=max(mx,wins(jp,op));minmax=min(minmax,mx);}
		int g=greedy();
		if(g!=maxmin||g!=minmax){++mismatch;cout<<"MISMATCH n="<<n<<" greedy="<<g<<" maxmin="<<maxmin<<" minmax="<<minmax<<" A:";for(int i=0;i<n;++i)cout<<' '<<A[i];cout<<" B:";for(int i=0;i<n;++i)cout<<' '<<B[i];cout<<'\n';if(mismatch>5)break;}
	}
	cout<<"done 400 game cases, mismatch="<<mismatch<<'\n';
	return 0;
}