# luogu-problem-solution

洛谷主站题库（Luogu Main Problemset）Markdown 题面归档与 AI 驱动的**发现式（Discovery Narrative）OI 题解工程**。

本项目收录了洛谷主站 `P1001`–`P17391` 共 **16,391** 道题目的结构化题面，并按照统一的「人类思考叙事 + 完整引理证明 + 竞赛 C++ 编码规范」编写题解报告与配套对拍验证程序（当前已完成 `P1001`–`P1506` 区间内 **505** 篇题解）。

## 一、项目目录架构

为解决原本单目录下存放上万个文件导致的 GitHub 网页截断（单目录 1,000 文件上限）、IDE 索引卡顿以及题面/题解/中间代码混杂问题，仓库按职责划分为四大核心目录（`statement/`、`solution/`、`appendix/`、`reference/`）与维护工具目录（`scripts/`）：

```text
luogu-problem-solution/
├── README.md                              # 项目总览、目录架构规范、进度统计与维护指南
├── statement/                             # [题面库] 洛谷主站 Markdown 题面（共 16,391 题）
│   ├── P1000-P1999/                       # 按千位题号分片（单目录 ≤ 1,000 文件）
│   │   ├── P1001.md
│   │   ├── P1002.md
│   │   └── ... (P1001–P1999，共 999 题)
│   ├── P2000-P2999/                       # P2000–P2999（共 1,000 题）
│   ├── ...
│   └── P17000-P17999/                     # P17000–P17391（共 392 题）
├── solution/                              # [题解库] 五段式发现叙事题解报告（当前共 505 篇）
│   └── P1000-P1999/                       # 与 statement/ 保持一致的千位题号分片
│       ├── P1001_solution.md
│       ├── P1002_solution.md
│       └── ... (覆盖 P1001–P1506，除 P1504 外共 505 篇)
├── appendix/                              # [解题附件] 解题中间文件与验证套件（按题号独立建夹）
│   ├── P1139/
│   │   └── P1139_trace.py                 # 双栈状态空间推演与手工模拟脚本（原 temp_trace.py）
│   ├── P1153/
│   │   └── P1153.cpp                      # 极角/DFS 简单多边形计数标程（std program）
│   ├── P1154/
│   │   └── P1154_check.py                 # 暴力枚举 vs 差集倍数筛法随机数据对拍脚本（checker）
│   ├── P1163/
│   │   └── P1163.cpp                      # 二分月利率标程（std program）
│   ├── P1167/
│   │   └── P1167.cpp                      # 格里高利历日期差 + 贪心标程（std program）
│   └── P1168/
│       └── P1168.cpp                      # 对顶堆动态维护前缀中位数标程（std program）
├── reference/                             # [参考规范] AI 解题 Skill 与 C++ 编码风格指南
│   ├── SKILL.write-oi-solution.md         # 发现式 OI 题解生成规范（五段式输出与证明纪律）
│   └── coding-style-guide.md              # 竞赛 C++ 编码风格规范（基于 606 份源码提炼）
└── scripts/                               # [维护脚本] 自动化目录整理与迁移工具
    └── reorganize.py                      # 支持 git mv 的仓库重构与增量文件自动归档脚本
```

### 目录职责总览

| 顶层目录 | 分类职责 | 路径与命名规范 | 当前规模 |
| :--- | :--- | :--- | :--- |
| `statement/` | 原始题目题面（含背景、描述、输入输出格式、提示与元数据） | `statement/P{xxxx}-P{yyyy}/P{id}.md` | 16,391 份题面 |
| `solution/` | 完整题解报告（重述、思维链证明、模式总结、符号表、C++ 标程） | `solution/P{xxxx}-P{yyyy}/P{id}_solution.md` | 505 篇题解 |
| `appendix/` | 解题中间文件（标程、暴力、数据生成器、对拍器、追踪脚本、测试点） | `appendix/P{id}/P{id}*` | 6 道题配套代码 |
| `reference/` | 题解方法论（AI Skill 定义）与 C++ 竞赛代码规范文档 | `reference/*.md` | 2 份核心规范 |
| `scripts/` | 仓库目录迁移、新增文件自动归档与结构校验脚本 | `scripts/reorganize.py` | 1 份自动化脚本 |

## 二、四大核心分类详解

### 1. 题面库（`statement/`）

存放从洛谷主站抓取的标准 Markdown 题面文件。每份题面包含难度（`Difficulty`）、标签（`Tags`）、题目描述（`Problem Description`）、输入输出格式、数据范围提示以及洛谷原题链接。

为确保在 GitHub Web 端浏览任意目录均不触发 1,000 文件截断限制，题面按题号千位分片存放（计算公式：`bucket = (id // 1000) * 1000`，目录名为 `P{bucket}-P{bucket+999}`）：

| 分片目录 | 题号范围 | 题面数量 | 已完成题解数 | 完成进度 |
| :--- | :--- | ---: | ---: | :--- |
| `P1000-P1999/` | `P1001` – `P1999` | 999 | 505 | 50.55%（已推进至 `P1506`，仅缺 `P1504`） |
| `P2000-P2999/` | `P2000` – `P2999` | 1,000 | 0 | 待推进 |
| `P3000-P3999/` | `P3000` – `P3999` | 1,000 | 0 | 待推进 |
| `P4000-P4999/` | `P4000` – `P4999` | 1,000 | 0 | 待推进 |
| `P5000-P5999/` | `P5000` – `P5999` | 1,000 | 0 | 待推进 |
| `P6000-P6999/` | `P6000` – `P6999` | 1,000 | 0 | 待推进 |
| `P7000-P7999/` | `P7000` – `P7999` | 1,000 | 0 | 待推进 |
| `P8000-P8999/` | `P8000` – `P8999` | 1,000 | 0 | 待推进 |
| `P9000-P9999/` | `P9000` – `P9999` | 1,000 | 0 | 待推进 |
| `P10000-P10999/` | `P10000` – `P10999` | 1,000 | 0 | 待推进 |
| `P11000-P11999/` | `P11000` – `P11999` | 1,000 | 0 | 待推进 |
| `P12000-P12999/` | `P12000` – `P12999` | 1,000 | 0 | 待推进 |
| `P13000-P13999/` | `P13000` – `P13999` | 1,000 | 0 | 待推进 |
| `P14000-P14999/` | `P14000` – `P14999` | 1,000 | 0 | 待推进 |
| `P15000-P15999/` | `P15000` – `P15999` | 1,000 | 0 | 待推进 |
| `P16000-P16999/` | `P16000` – `P16999` | 1,000 | 0 | 待推进 |
| `P17000-P17999/` | `P17000` – `P17391` | 392 | 0 | 待推进 |
| **合计** | `P1001` – `P17391` | **16,391** | **505** | **3.08%** |

### 2. 题解库（`solution/`）

存放遵循 `reference/SKILL.write-oi-solution.md` 规范生成的 Markdown 题解。题解摒弃教材式的“先抛结论再给公式”，采用**学生视角的发现式叙事（Discovery Narrative）**，固定包含五个核心部分：

1. **重新读题（Re-read the problem）**：剥离冗余背景，用直白语言重述问题模型、输入输出约束与边界陷阱。
2. **思维链与完整证明（Chain-of-thought）**：按真实思考顺序列出从暴力试探、性质剖析、子任务简化到经典算法归约的推导全过程，**每一条引理与贪心/DP 性质均附完整证明**（拒绝“显然成立”）。
3. **模式总结（Pattern summary）**：按实际思考顺序提炼可迁移的思维标签（如 `explore its property`、`consider easier version`、`reduce to easier problem`）。
4. **解法说明（The real solution）**：先给出完整符号表（每个数组/变量的含义、初值与助记说明），再按编号列出算法步骤、时空复杂度与验证状态。
5. **示例代码（Example code）**：严格符合 `reference/coding-style-guide.md` 的可提交 C++ 源码。

### 3. 解题附件与中间文件（`appendix/`）

用于集中存放解题、调试、造数据与对拍过程中产生的中间产物，按题目编号建立子目录 `appendix/P{id}/`，保持主目录与题解目录整洁。

#### 标准文件命名约定

| 中间文件类型 | 建议命名格式 | 说明 |
| :--- | :--- | :--- |
| 标准程序（Std Program） | `P{id}.cpp` 或 `std.cpp` | 最终通过验证、与题解 Part 5 一致的 C++ 标程 |
| 暴力基准（Brute-force） | `P{id}_brute.cpp` 或 `brute.cpp` | 小数据范围绝对正确的朴素实现，用于对拍基准 |
| 数据生成器（Generator） | `P{id}_gen.py` 或 `P{id}_gen.cpp` | 随机测试数据与极端边界数据生成脚本 |
| 对拍/校验器（Checker） | `P{id}_check.py` 或 `P{id}_checker.cpp` | 自动循环对拍脚本或 Special Judge 校验程序 |
| 模拟/追踪脚本（Trace） | `P{id}_trace.py` | 辅助理解题目机制、打印小规模状态转移或样例手推的脚本 |
| 测试样例（Testcases） | `testcases/*.in` / `*.out` | 本地提取的样例或自造 Hack 测试数据 |

#### 当前已归档的中间文件清单

| 题目编号 | 归档路径 | 文件类型 | 用途说明 |
| :--- | :--- | :--- | :--- |
| `P1139` | `appendix/P1139/P1139_trace.py` | Trace Script | 《单向双轨道》双栈排序状态模拟与目标序列逆序验证（原 `temp_trace.py`） |
| `P1153` | `appendix/P1153/P1153.cpp` | Std Program | 《点和线》基于叉积规范相交判定的 DFS 简单多边形计数标程 |
| `P1154` | `appendix/P1154/P1154_check.py` | Checker | 《奶牛分厩》暴力模运算与差集倍数扫描算法的 5,000 组随机数据对拍脚本 |
| `P1163` | `appendix/P1163/P1163.cpp` | Std Program | 《银行贷款》月利率固定次数二分判定标程 |
| `P1167` | `appendix/P1167/P1167.cpp` | Std Program | 《刷题》格里高利历闰年分钟差换算 + 排序贪心标程 |
| `P1168` | `appendix/P1168/P1168.cpp` | Std Program | 《中位数》大根堆 + 小根堆（对顶堆）在线维护奇数前缀中位数标程 |

### 4. 参考规范（`reference/`）

存放驱动整个项目的两份核心标准文档：

- **`reference/SKILL.write-oi-solution.md`**：定义 AI 撰写算法题解的工作流、五段式输出模板、引理证明要求及仓库目录读写约定。
- **`reference/coding-style-guide.md`**：基于对 606 个既有 `.cpp` 源码全量分析提炼出的描述性编码规范，核心特征包括：
  - 仅包含 `#include<bits/stdc++.h>` 与全局 `using namespace std;`，使用 `using i64=long long;`。
  - 全局静态数组优先，尺寸常量采用 `constexpr int mxn{...+5};` 松弛约定，哨兵值优先采用 `114514` 家族（如 `1145141919`）或 `1e9`。
  - 内部统一使用 **0-based** 下标（读入即 `--u;--v;`，输出恢复 `i+1`）。
  - 线段树等递归结构采用标志性的自递归 Lambda **`sth` 模式**（半开区间 `[l, r)`，根下标 `0`）。
  - I/O 加速固定顺序：`cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);`，换行一律使用 `'\n'`。
  - 缩进采用 Tab，运算符紧凑不留空格，保留注释化的 `// cerr<<...` 作为可随时启用的调试层。

## 三、日常使用与维护工作流

### 1. 编写新题解的标准流程

当使用 AI 助手或手动攻克新题目（例如 `P1504` 或 `P1507`）时：

1. **读取题面**：从 `statement/P1000-P1999/P1507.md` 读取完整题目描述与数据范围。
2. **编写对拍与标程**：在 `appendix/P1507/` 下创建 `P1507.cpp`（标程）与 `P1507_check.py`（暴力对拍脚本），完成小规模穷举或随机数据验证。
3. **生成题解**：参照 `reference/SKILL.write-oi-solution.md` 与 `reference/coding-style-guide.md`，将完整题解保存至 `solution/P1000-P1999/P1507_solution.md`。

### 2. 使用自动化归档脚本（`scripts/reorganize.py`）

如果在解题过程中习惯先在仓库根目录临时创建 `Pxxxx.cpp`、`Pxxxx_check.py` 或 `Pxxxx_solution.md`，或者需要将本地旧版扁平仓库迁移至新架构，可直接运行 `scripts/reorganize.py`：

```bash
# 预览将要执行的文件移动操作（不实际修改文件）
python3 scripts/reorganize.py --dry-run

# 在普通目录或解压后的仓库中执行架构整理
python3 scripts/reorganize.py

# 在 Git 仓库中执行架构整理（自动检测并使用 git mv 保留历史记录）
python3 scripts/reorganize.py --git
```
