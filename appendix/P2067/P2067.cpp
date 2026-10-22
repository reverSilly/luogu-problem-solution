#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n;
char g[mxn][mxn];
//石子（'x'）在网格中的位置：sx 为列号，sy 为行号
int sx[mxn*mxn],sy[mxn*mxn],scnt=0;
//直线的 一般式 A*x+B*y+C=0（网格坐标 x 为列，y 为行）
int A,B,C;

//坐标轴候选：rx 行与 cy 列必须覆盖全部 '1'，
//且轴上不能出现 '0'（轴上的格子要么是 '1'，要么压着石子 'x'）
bool axisok(int rx,int cy)
{
	int cntr=0,cntc=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			if(g[i][j]=='0'&&(i==rx||j==cy))
				return false;
			if(g[i][j]=='1'&&i!=rx&&j!=cy)
				return false;
			if(g[i][j]=='1')
			{
				if(i==rx)
					++cntr;
				if(j==cy)
					++cntc;
			}
		}
	return cntr>0&&cntc>0;
}

//该行 / 该列上 '1' 的个数
int rowone(int r)
{
	int cnt=0;
	for(int j=0;j<n;++j)
		if(g[r][j]=='1')
			++cnt;
	return cnt;
}

int colone(int c)
{
	int cnt=0;
	for(int i=0;i<n;++i)
		if(g[i][c]=='1')
			++cnt;
	return cnt;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>g[i];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(g[i][j]=='x')
			{
				sx[scnt]=j;
				sy[scnt]=i;
				++scnt;
			}

	//两个石子定出直线（网格坐标）
	A=sy[0]-sy[1];
	B=sx[1]-sx[0];
	C=sx[0]*sy[1]-sx[1]*sy[0];

	//选坐标轴：候选中 '1' 最多的那条十字即为真正的坐标轴
	int rx=-1,cy=-1,bestscore=-1;
	for(int r=0;r<n;++r)
		for(int c=0;c<n;++c)
		{
			if(!axisok(r,c))
				continue;
			int score=rowone(r)+colone(c);
			if(score>bestscore)
			{
				bestscore=score;
				rx=r;
				cy=c;
			}
		}

	//换算到以 (rx,cy) 为原点、Y 轴向上的坐标系
	if(!B)
	{
		//竖直线：网格列 j=-C/A，数学横坐标 X=j-cy
		printf("x=%.4f\n",(double)(-C)/A-cy);
		return 0;
	}
	if(!A)
	{
		//水平线：网格行 i=-C/B，数学纵坐标 Y=rx-i
		printf("y=%.4f\n",(double)rx+(double)C/B);
		return 0;
	}
	double k=(double)A/B;
	double b=(double)(A*cy+B*rx+C)/B;
	if(fabs(b)<1e-9)
		b=0;
	printf("y=%.4fx",k);
	if(b>0)
		printf("+%.4f",b);
	else if(b<0)
		printf("%.4f",b);
	printf("\n");
	return 0;
}
