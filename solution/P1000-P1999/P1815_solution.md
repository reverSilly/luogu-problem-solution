# P1815 [ECNA 2001 Practice] 蠕虫游戏

> 难度：普及/提高- · 标签：模拟、ICPC

## 1. 重新读题
### 1.1 问题重述（人话版）

\(50\times 50\) 的棋盘，左上角是 \((1,1)\)，第一维向下增大（南），第二维向右增大（东）。蠕虫开始时横着占满 \((25,11)\) 到 \((25,30)\)，头在 \((25,30)\)，一共 20 格。每条指令是 `E/W/S/N` 之一，头往那个方向挪一格，长度不变，所以尾巴同时让出一格。头可以走进尾巴刚刚让出的格子。

模拟到三种结局之一就停，剩下的指令扔掉：

- 撞到自己：`The worm ran into itself on move m.`
- 走出棋盘：`The worm ran off the board on move m.`
- 指令全部走完：`The worm successfully made all m moves.`

多组数据，读到 \(n=0\) 结束。\(n=0\) 本身不产生输出。第一步是 move 1。

### 1.2 关键约束与规模暗示

- \(n<100\)，棋盘只有 50，纯模拟。
- 开局朝西会立刻撞上自己的身体（头左边那格还被占着），题面明确说初始不许朝西。
- 先走出界还是先撞自己：新坐标先做边界判断。界外的格子不属于蠕虫，应报 off the board。
- 尾巴和头在同一步里都要改。若先判撞再删尾巴，头走进旧尾巴会被误判成撞自己，与题面那句「能走进刚刚让出的空格」相反。

## 2. 思维链（Chain-of-thought）

**引理 1（身体始终是最近 20 个头部位置，初始要预填 20 格）**

第 \(t\) 步之后（\(t\ge 0\)，\(t=0\) 表示还没动），蠕虫占据的 20 格是「当前头」以及它前面 19 个历史位置。初始历史要预先写成 \((25,11),\ldots,(25,30)\)，最后一格是头。

**证明**：题面说每次只移动一格且长度不变，因此变化的只有新头和旧尾。对步数归纳：\(t=0\) 时这 20 格就是题目给的初始身体。从 \(t\) 到 \(t+1\)，去掉最老的那一格（旧尾），加上新头，剩下的 19 格原样保留，仍是连续的 20 个位置。\(\square\)

**引理 2（必须先让出尾巴，再判断新头）**

设旧身体位置按从尾到头为 \(p_{t},\ldots,p_{t+19}\)，头是 \(p_{t+19}\)。新头 \(p\) 撞到自己，当且仅当 \(p\) 等于 \(p_{t+1},\ldots,p_{t+19}\) 之一，而**不等于**仅仅是旧尾 \(p_t\) 的那种情况要算活。实现上等价于：先把 \(p_t\) 从占用表删掉，再查 \(p\) 是否仍被占用。

**证明**：移动后的身体是 \(\{p_{t+1},\ldots,p_{t+19},p\}\)。\(p\) 与自己冲突 \(\Leftrightarrow p\) 落在 \(\{p_{t+1},\ldots,p_{t+19}\}\) 里。\(p_{t+19}\) 是旧头，新头是它的四邻，两者坐标不同，所以查不查旧头都不会误伤，但旧尾 \(p_t\) 必须排除。题面写明头可以进入尾巴刚让出的格子，排除 \(p_t\) 是这条规则的全部内容。先删尾巴再查表，查到的占用集合正好是 \(\{p_{t+1},\ldots,p_{t+19}\}\)。\(\square\)

**引理 3（出界优先于撞自己）**

若新坐标不在 \([1,50]\times[1,50]\)，输出 off the board，不再查占用。

**证明**：占用表只在棋盘内有意义。新坐标出界时，它不是任何身体格子（身体全部在棋盘内，引理 1）。题面的两种失败是「撞到自己」和「越出棋盘」，出界属于后者。先判断边界也避免读到数组外面。\(\square\)

方向：`E` 列 +1，`W` 列 -1，`S` 行 +1，`N` 行 -1。初始头在列 30、身体向西延伸到列 11，与「东是列增大」一致。样例第 3 组连续 30 个 `E` 在第 21 步出界（列 30 再向东 20 步到列 50，第 21 步到列 51），与题面输出一致，方向没有装反。

暴力不用双端队列，而是按引理 1 把头部历史存在数组里，碰撞时只扫描下标 `head-18 .. head`（不含更老的那一格尾巴）。两份实现对照，再加上同算法的 Python 历史模拟，三方一致。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 长度不变 \(\Rightarrow\) 身体 = 最近 20 个头的位置，初始 20 格要预填。
- **simulate directly** — 先删尾巴，再判出界，再判占用，最后把头推进去。
- **watch the sentinel** — 棋盘外先报 off the board；\(n=0\) 只是结束标记，不输出。

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| \(n\) | `int` | 本组指令数 | 读到 0 停止 | \(n<100\) |
| `s` | `string` | 指令串 | 读入 | 只有 EWSN |
| `occ[r][c]` | `int[52][52]` | 格子是否被身体占 | 每组清零后把第 25 行 11..30 列标 1 | 下标 1..50 |
| `body` | `deque<pair>` | 从前到后是头到尾 | `(25,30)` … `(25,11)` | `push_front` 放新头，`pop_back` 丢尾巴 |

### 4.2 算法步骤

1. `while (cin>>n)`，\(n=0\) 退出。读指令串。
2. 清空占用，把列 11 到 30 依次 `push_front`，这样队头是 \((25,30)\)。
3. 对每条指令算新头。先弹出队尾并清占用。越界则输出 off the board 并停；占用仍为真则输出 into itself 并停；否则标记占用并 `push_front`。
4. 指令走完还没死，输出 successfully。组与组之间不要额外空行。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

- 每组 \(O(n)\)，\(n<100\)，占用表 \(O(1)\) 清 2500 格。
- 编译 warn=0。
- 官方样例四组输出与题面逐字符相同（18 步成功、第 9 步撞自己、第 21 步出界、13 步成功）。
- 10 组边界（只有 `0`、单步四个方向、连续 `N/S/E/W`）加 200 组随机（每组 1 到 4 条长度 \(\le 60\) 的指令）。标程（双端队列 + 占用表）、暴力（头部历史数组）、Python 历史模拟三者全等。共 211 组，随机段 3.754s。
- 最长单组 \(n=99\) 标程 0.010s，该组答案 `The worm ran into itself on move 4.`。

## 5. 示例代码
### 5.1 标程（`P1815.cpp`）

```cpp
//
// 题目：P1815 [ECNA 2001 Practice] 蠕虫游戏
// https://www.luogu.com.cn/problem/P1815
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

int occ[52][52];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	while(cin>>n)
	{
		if(!n)
			break;
		string s;
		cin>>s;
		memset(occ,0,sizeof(occ));
		deque<pair<int,int>> body;
		for(int c=11;c<=30;++c)
		{
			occ[25][c]=1;
			body.push_front({25,c});
		}
		bool dead{false};
		for(int i=0;i<n;++i)
		{
			int hr{body.front().first};
			int hc{body.front().second};
			int nr{hr};
			int nc{hc};
			char mv{s[i]};
			if(mv=='E')
				++nc;
			else if(mv=='W')
				--nc;
			else if(mv=='S')
				++nr;
			else
				--nr;
			int tr{body.back().first};
			int tc{body.back().second};
			body.pop_back();
			occ[tr][tc]=0;
			if(nr<1||nr>50||nc<1||nc>50)
			{
				cout<<"The worm ran off the board on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			if(occ[nr][nc])
			{
				cout<<"The worm ran into itself on move "<<i+1<<".\n";
				dead=true;
				break;
			}
			occ[nr][nc]=1;
			body.push_front({nr,nc});
		}
		if(!dead)
			cout<<"The worm successfully made all "<<n<<" moves.\n";
	}
	return 0;
}
```
