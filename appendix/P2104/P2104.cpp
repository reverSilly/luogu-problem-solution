// 题目：P2104 二进制
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10000015};

// 游程编码: seg[i] = (个数<<1)|位值
// seg[0] 是最高位段, seg[top-1] 是最低位段, 相邻两段位值必然不同
int seg[mxn];
int top;
string bs,op;

// 在最低位侧压入一段, 与栈顶同位则合并
void push_seg(int cnt,int bit)
{
	if(top>0 && (seg[top-1]&1)==bit)
	{
		seg[top-1]+=cnt<<1;
		return;
	}
	seg[top++]=cnt<<1|bit;
}

// 最高位侧两段同位时合并
void merge_up()
{
	if(top>=2 && (seg[top-1]&1)==(seg[top-2]&1))
	{
		int bit=seg[top-1]&1;
		seg[top-2]=(((seg[top-2]>>1)+(seg[top-1]>>1))<<1)|bit;
		--top;
	}
}

// 加 1: 末尾连续的 1 变 0, 其上方那一位 0 变 1
void add_one()
{
	if(top==0) return;
	int last=seg[top-1],bit=last&1,k=last>>1;
	if(bit==0)
	{
		// 最低位是 0, 只有这一位变 1
		if(k>1)
		{
			seg[top-1]=(k-1)<<1;
			push_seg(1,1);
		}
		else
		{
			// 最低位段只剩一个 0, 整个段变成 1, 与上方 1 段合并
			if(top>=2)
			{
				seg[top-2]=(((seg[top-2]>>1)+1)<<1)|1;
				--top;
			}
			else seg[0]=2|1;
		}
		return;
	}
	// 最低位是连续的 k 个 1
	if(top==1)
	{
		// 整串都是 1, 最高位进位(题目保证不会出现)
		seg[0]=2|1;
		push_seg(k,0);
		return;
	}
	int m=seg[top-2]>>1;	// 上方 0 段的长度
	--top;					// 去掉末尾的 1 段
	if(m>1)
	{
		seg[top-1]=(m-1)<<1;	// 上方 0 段只剩 m-1 个 0
		push_seg(1,1);		// 那一位 0 变 1
	}
	else
	{
		// 上方 0 段只有一位, 变成 1 后与再上方的 1 段合并
		if(top>=2)
		{
			seg[top-2]=(((seg[top-2]>>1)+1)<<1)|1;
			--top;
		}
		else seg[0]=2|1;
	}
	push_seg(k,0);			// 末尾 k 个 1 变成 k 个 0
}

// 减 1: 末尾连续的 0 变 1, 其上方那一位 1 变 0
void sub_one()
{
	if(top==0) return;
	int last=seg[top-1],bit=last&1,k=last>>1;
	if(bit==1)
	{
		// 最低位是 1, 只有这一位变 0
		if(k>1)
		{
			seg[top-1]=((k-1)<<1)|1;
			push_seg(1,0);
		}
		else
		{
			// 最低位段只剩一个 1, 整个段变成 0, 与上方 0 段合并
			if(top>=2)
			{
				seg[top-2]=((seg[top-2]>>1)+1)<<1;
				--top;
			}
			else seg[0]=2;
		}
		return;
	}
	// 最低位是连续的 k 个 0
	if(top==1)
	{
		// 整个数是 0, 无法借位(题目保证不会出现)
		return;
	}
	int m=seg[top-2]>>1;	// 上方 1 段的长度
	--top;					// 去掉末尾的 0 段
	if(m>1)
	{
		seg[top-1]=((m-1)<<1)|1;	// 上方 1 段只剩 m-1 个 1
		push_seg(1,0);		// 那一位 1 变 0
	}
	else
	{
		// 上方 1 段只有一位, 变成 0 后与再上方的 0 段合并
		if(top>=2)
		{
			seg[top-2]=((seg[top-2]>>1)+1)<<1;
			--top;
		}
		else seg[0]=2;
	}
	push_seg(k,1);			// 末尾 k 个 0 变成 k 个 1
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	cin>>bs>>op;

	// 按数位串建栈, 前导 0 原样保留
	for(char c:bs)
	{
		int b=c-'0';
		if(top>0 && (seg[top-1]&1)==b) seg[top-1]+=2;
		else seg[top++]=2|b;
	}

	for(char c:op)
	{
		if(c=='+') add_one();
		else if(c=='-') sub_one();
		else if(c=='*') push_seg(1,0);
		else if(c=='/')
		{
			if(top==0) continue;
			int last=seg[top-1],cnt=last>>1;
			if(cnt==1) --top;
			else seg[top-1]=((cnt-1)<<1)|(last&1);
			// 删空则视作 0
			if(top==0) seg[top++]=2;
		}
	}

	string out;
	out.reserve(n+m+5);
	for(int i=0;i<top;++i)
	{
		int cnt=seg[i]>>1,bit=seg[i]&1;
		out.append(cnt,char('0'+bit));
	}
	cout<<out<<'\n';
	return 0;
}
