#include<bits/stdc++.h>
using namespace std;
// 完整枚举：对手把补集牌分配到 (轮,对手) 槽 + John 出牌排列，最小化 John 赢的轮数
int m,n,A[6],B[40];
int greedy()
{
	sort(A,A+n);sort(B,B+n*(m-1));
	int cnt=n*(m-1),j=0,beat=0;
	for(int i=0;i<n;++i){while(j<cnt&&B[j]<=A[i])++j;if(j<cnt){++beat;++j;}}
	return n-beat;
}
int main()
{
	mt19937 rn(31337);
	int mismatch=0,cases=0;
	// 小规模: (m,n) in {(2,1..4),(3,1..2),(4,1..2)}
	vector<pair<int,int>>cfg={{2,1},{2,2},{2,3},{2,4},{3,1},{3,2},{4,1},{4,2}};
	for(auto mn:cfg)
	{
		m=mn.first;n=mn.second;
		int slots=n*(m-1);
		for(int t=0;t<300;++t)
		{
			vector<int>pool(n*m);iota(pool.begin(),pool.end(),1);
			shuffle(pool.begin(),pool.end(),rn);
			for(int i=0;i<n;++i)A[i]=pool[i];
			for(int i=0;i<slots;++i)B[i]=pool[n+i];
			vector<int>ja(A,A+n);sort(ja.begin(),ja.end());
			vector<vector<int>>JP;do{JP.push_back(ja);}while(next_permutation(ja.begin(),ja.end()));
			vector<int>ob(B,B+slots);sort(ob.begin(),ob.end());
			int best=n;
			do
			{
				// ob[r*(m-1)+o] = 第 r 轮对手 o 的牌
				for(auto&jp:JP)
				{
					int w=0;
					for(int r=0;r<n;++r)
					{
						bool win=true;
						for(int o=0;o<m-1;++o)
							if(ob[r*(m-1)+o]>jp[r])win=false;
						if(win)++w;
					}
					best=min(best,w);
				}
			}while(next_permutation(ob.begin(),ob.end()));
			int g=greedy();
			++cases;
			if(g!=best){++mismatch;cout<<"MISMATCH m="<<m<<" n="<<n<<" greedy="<<g<<" full="<<best<<" A:";for(int i=0;i<n;++i)cout<<' '<<A[i];cout<<" B:";for(int i=0;i<slots;++i)cout<<' '<<B[i];cout<<'\n';if(mismatch>8)break;}
		}
		if(mismatch>8)break;
	}
	cout<<"done "<<cases<<" full-enumeration cases, mismatch="<<mismatch<<'\n';
	return 0;
}