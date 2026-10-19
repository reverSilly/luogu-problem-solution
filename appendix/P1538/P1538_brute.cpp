#include<bits/stdc++.h>
using namespace std;

// 独立基准：沿用经典"5 行七段管"字符串表,行 1/3/5 为横段,行 2/4 为竖段(重复 k 次)。
// 索引 0=左上,1=上,2=右上,3=中,4=左下,5=下,6=右下。与标程逐行循环结构不同。
char lib[10][8]={
	"|-| |-|","  |   |"," -|-|- "," -|- -|","| |-  |",
	"|- - -|","|- -|-|"," -|   |","|-|-|-|","|-|- -|"
};

int main()
{
	int k;
	string s;
	cin>>k>>s;
	for(int row=1;row<=5;++row)
	{
		int times=(row&1)?1:k;          // 奇数行只输出一次,偶数行(竖段)输出 k 次
		for(int rep=0;rep<times;++rep)
		{
			for(char c:s)
			{
				int d=c-'0';
				if(row&1)               // 横段行 1/3/5
				{
					int idx=(row==1)?1:(row==3?3:5);
					cout<<' ';
					for(int j=0;j<k;++j)
						cout<<lib[d][idx];
					cout<<"  ";
				}
				else                    // 竖段行 2/4
				{
					int li=(row==2)?0:4;
					int ri=(row==2)?2:6;
					cout<<lib[d][li];
					for(int j=0;j<k;++j)
						cout<<' ';
					cout<<lib[d][ri]<<' ';
				}
			}
			cout<<'\n';
		}
	}
	return 0;
}
