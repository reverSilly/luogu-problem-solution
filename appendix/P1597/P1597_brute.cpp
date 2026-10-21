//
// P1597 朴素解：按 ';' 切分语句, 逐条解析
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string all,t;
	while(getline(cin,t))
		all+=t;
	int v[3]{};
	string cur;
	for(char c:all)
	{
		if(c!=';')
		{
			cur+=c;
			continue;
		}
		// cur 形如 "a:=3" 或 "b:=a"
		int lhs=cur[0]-'a';
		char rhs=cur[3];
		if(rhs>='0'&&rhs<='9')
			v[lhs]=rhs-'0';
		else
			v[lhs]=v[rhs-'a'];
		cur.clear();
	}
	cout<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';
	return 0;
}
