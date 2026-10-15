# P1065 [NOIP 2006 提高组] 作业调度方案 题解

---

## 一、发现叙事

### 1.1 题面还原

有 $m$ 台机器、$n$ 个工件。每个工件恰好有 $m$ 道工序，每道工序在**不同的指定机器**上完成，且加工时间给定。一个操作 "工件 $j$ 的第 $k$ 道工序" 记作 `j-k`。

题目额外给定一个**安排顺序** —— 它是 $m \times n$ 个操作的一个排列，规定了我们必须按这个顺序逐一插入每个操作。插入时受两条约束约束：

1. **工序依赖**：同一工件的后一道工序必须等前一道完成后才能开始。
2. **机器互斥**：同一时刻一台机器只能加工一个工件。

此外有两条**贪心约定**（这是本题的核心）：

- 保证约束 (1)(2) 的前提下，**尽量靠前插入**（即操作的开始时间尽可能早）。
- 如果有多个空档都能插入，选**最前面的一个**。

在这些约定下，安排顺序唯一确定了一个调度方案。

### 1.2 直觉与突破口

初看这是一个"调度问题"，容易让人联想到流水车间调度（Flow Shop）、Johnson 法则甚至 NP-困难。但题目有一个关键设定——**安排顺序已经固定**，我们只是机械地按顺序插入，并遵循"尽量靠前"的局部贪心规则。所以这不是一个最优化问题，而是一个**确定性模拟**问题。

突破口在于：对每个操作，我们只需要在其目标机器上寻找**最早的可用时间区间**，使得：

- 该区间的起点 $\ge$ 同一工件上一道工序的完成时间。
- 该区间的长度 $\ge$ 当前操作的加工时间。
- 该区间不与机器上已有的任何加工区间重叠。

用"夹缝中找空隙"的视角来看：一台机器上已有的加工区间把时间轴切成了若干段空闲区间（空档），我们要在这些空档中找到第一个满足条件的位置。

---

## 二、思路推导与正确性证明

### 2.1 算法流程的形式化

设：

- $mach_{j,k}$ —— 工件 $j$ 的第 $k$ 道工序使用的机器编号。
- $time_{j,k}$ —— 该工序的加工时间。
- $seq$ 长度为 $m \times n$，给出了操作的工件号顺序；对于 $seq$ 中第 $t$ 次出现的工件 $j$，它对应的是该工件的第 $step_j$ 道工序（$step_j$ 从 $0$ 开始计数）。
- $finish_j$ —— 工件 $j$ 最近完成的工序的结束时间，初始为 $0$。
- $M_i$ —— 机器 $i$ 上的已调度区间集合，每个区间为 $[l, r)$，表示机器在该半开时间区间内被占用。

按 $seq$ 遍历每个元素 $w = seq[t]$：

1. 令 $k = step_w$，$machine = mach_{w,k}$，$need = time_{w,k}$。
2. 在机器 $machine$ 的区间集合 $M_{machine}$ 中，寻找最早的开始时间 $start$，满足：
   - $start \ge finish_w$（工序依赖）。
   - 对于任意 $[l, r) \in M_{machine}$，区间 $[start, start + need)$ 与 $[l, r)$ 不相交（机器互斥）。
3. 将 $[start, start + need)$ 插入 $M_{machine}$。
4. 更新 $step_w \leftarrow step_w + 1$，$finish_w \leftarrow start + need$。
5. 用 $finish_w$ 更新全局答案 $ans$。

遍历结束后，$ans$ 即为完成全部任务的总时间。

### 2.2 正确性证明

**引理 1**：在任意时刻，按上述流程调度出的方案满足约束 (1) 和 (2)。

- 约束 (1)：$start \ge finish_w$ 保证了同一工件上一道工序已完成。
- 约束 (2)：新区间不与 $M_{machine}$ 中任何区间重叠，保证了机器不超载。

**引理 2**：在"尽量靠前"和"选最前面空档"的约定下，每个操作的插入位置是唯一确定的。

- 对于给定的 $M_{machine}$ 和 $finish_w$，所有满足条件的 $start$ 构成一个集合。其中最小的那个就是 "最靠前" 的位置。算法通过扫描空档取第一个可行解，自然得到这个最小值。归纳地，整个调度方案唯一。

**引理 3**（归纳不变量）：算法正确维护了每个工件已完成的工序数 $step_w$ 及其最后完成时间 $finish_w$。

- 基本情况：$step_w = 0$，$finish_w = 0$。
- 归纳步：调度完一个操作后，$step_w$ 加 $1$，$finish_w$ 更新为该操作的结束时间。这个结束时间是后续同工件操作的依赖基准。

**定理**：算法输出的 $ans$ 等于题目所求的总加工时间。

- 由引理 1–3，算法忠实地模拟了题目的调度规则，并按规则计算出最后一台机器完成最后一个操作的时刻。由于调度方案唯一，该结果必然正确。$\square$

### 2.3 边界与细节

- **空档查找**：将机器上的区间按左端点排序后，时间轴被分为若干连续段：
  ```
  [0, l_1), [r_1, l_2), [r_2, l_3), ..., [r_{k-1}, l_k), [r_k, +∞)
  ```
  对每个空档 $[cur, next\_l)$，若 $\max(cur, finish_w) + need \le next\_l$，则该空档可用。
- **插入后排序**：为避免破坏有序性，可以在 $M_{machine}$ 末尾插入新区间后整体排序（数据规模极小，$m \cdot n < 400$，排序代价可忽略）。
- **半开区间**：使用 $[l, r)$ 半开区间简化计算 —— 加工时间为 $need$ 的操作占据 $start$ 到 $start + need - 1$ 的时刻，下一操作可从 $start + need$ 开始。

---

## 三、模式总结

| 关键点 | 类型 |
|--------|------|
| 问题本质 | **确定性模拟**，不是最优化 |
| 核心操作 | 在有序区间集合中查找并插入新区间 |
| 数据结构需求 | 每个机器维护一组排序的 $[l, r)$ 区间，支持查找最早可用空档 |
| 时间复杂度 | $O((mn)^2 \log(mn))$，$n,m \le 20$ 时可视为常数 |
| 空间复杂度 | $O(m \cdot (mn)) = O(m^2 n)$ |

本题是典型的"按规则模拟 + 区间维护"题。类似思路可推广到：

- 会议室预订（给定已有会议，找最早可用时间段）
- 内存分配（给定已占用的内存块，找首个足够大的空闲块）
- 任意"插入到最早可用空隙"的调度问题

---

## 四、正式解法与实现要点

### 4.1 数据结构

```
mach[j][k]  : int —— 工件 j 第 k 道工序的机器号 (0-based)
cost[j][k]  : int —— 工件 j 第 k 道工序的加工时间
step[j]     : int —— 工件 j 下一道要安排的工序下标
finish[j]   : int —— 工件 j 最近完成工序的结束时刻
mac[mach]   : vector<pair<int,int>> —— 机器 mach 上已占用的 [l, r) 区间，按 l 排序
```

### 4.2 算法步骤

1. 读入 $m, n$，读入安排顺序 $seq$（长度 $mn$，立即转为 $0$-based 工件号）。
2. 读入 $n \times m$ 的机器号矩阵 $mach$（转 $0$-based）。
3. 读入 $n \times m$ 的加工时间矩阵 $cost$。
4. 初始化 $step[:] = 0$，$finish[:] = 0$，$ans = 0$。
5. 遍历 $seq$ 中的每个工件号 $w$：
   - $k = step[w]$
   - $mac = mach[w][k]$，$t = cost[w][k]$
   - 在 $mac$ 机器的区间列表中查找最早空档：
     ```
     cur = 0
     start = -1
     for (l, r) in intervals[mac], sorted:
         if max(cur, finish[w]) + t <= l:
             start = max(cur, finish[w]); break
         cur = max(cur, r)
     if start == -1:
         start = max(cur, finish[w])
     ```
   - 将 $[start, start + t)$ 插入 $intervals[mac]$ 并排序。
   - $step[w] \leftarrow step[w] + 1$
   - $finish[w] \leftarrow start + t$
   - $ans = \max(ans, finish[w])$
6. 输出 $ans$。

### 4.3 复杂度

- $\#operations = mn \le 400$。
- 每次查找空档遍历 $O(mn)$ 个区间，加上排序 $O((mn)\log(mn))$。
- 总复杂度 $O((mn)^2 \log(mn))$，在 $n,m < 20$ 时完全可接受。

---

## 五、C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int m, n;
    cin >> m >> n;
    int tot = m * n;
    vector<int> seq(tot);
    for(int i = 0; i < tot; ++i)
    {
        cin >> seq[i];
        --seq[i];
    }

    vector<vector<int>> mach(n, vector<int>(m));
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < m; ++j)
        {
            cin >> mach[i][j];
            --mach[i][j];
        }
    }

    vector<vector<int>> cost(n, vector<int>(m));
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            cin >> cost[i][j];

    vector<int> step(n, 0);
    vector<int> finish(n, 0);
    vector<vector<pair<int,int>>> mac(m);

    int ans = 0;
    for(int i = 0; i < tot; ++i)
    {
        int w = seq[i];
        int k = step[w];
        int mc = mach[w][k];
        int t = cost[w][k];
        int pre = finish[w];

        int cur = 0;
        int st = -1;
        for(auto [l, r] : mac[mc])
        {
            if(max(cur, pre) + t <= l)
            {
                st = max(cur, pre);
                break;
            }
            if(r > cur)
                cur = r;
        }
        if(st == -1)
            st = max(cur, pre);

        mac[mc].push_back({st, st + t});
        sort(mac[mc].begin(), mac[mc].end());

        ++step[w];
        finish[w] = st + t;
        if(finish[w] > ans)
            ans = finish[w];
    }

    cout << ans << '\n';
    return 0;
}
```

### 5.1 代码要点说明

- **零基转换**：读入后立即将机器号和工件号减一，内部全程 $0$-based。
- **空档扫描**：`cur` 记录当前扫描到的时间位置（上一个占用区间的右端点）。对每个区间 $[l, r)$，检查它前面的空档 $[cur, l)$ 是否满足条件。若不满足，将 `cur` 推进到 $r$。
- **最后空档**：如果所有区间之间的空档都不够，就把操作放在所有区间之后（即从 `max(cur, pre)` 开始）。
- **插入后排序**：`mac[mc]` 用 `push_back` 后 `sort` 保持有序。由于总操作数不到 $400$，排序开销极小。
- **半开区间**：区间 $[st, st + t)$ 表示占用 $st$ 到 $st + t - 1$ 的时刻，下一操作可从 $st + t$ 开始，计算简洁。
- **Allman 花括号风格**：函数体、`if`、`for`、`while` 的花括号均换行另起，符合指定规范。
