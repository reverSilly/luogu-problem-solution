#!/usr/bin/env python3
"""生成题库难度 / 标签统计报告。

用法（在仓库根目录执行）：

    python3 scripts/gen_statistics.py            # 生成报告与逐题 CSV
    python3 scripts/gen_statistics.py --check    # 只校验题面元数据，不写文件

输入：statement/P{xxxx}-P{yyyy}/P{id}.md 中的 `## Difficulty` / `## Tags` 两个章节，
      以及 solution/、appendix/ 的实际覆盖情况。
输出：reference/problem-statistics.md（人读报告）
      reference/problem-data.csv（机读逐题明细）
"""

from __future__ import annotations

import argparse
import csv
import datetime
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STATEMENT = ROOT / "statement"
SOLUTION = ROOT / "solution"
APPENDIX = ROOT / "appendix"
OUT_MD = ROOT / "reference" / "problem-statistics.md"
OUT_CSV = ROOT / "reference" / "problem-data.csv"

# 难度按洛谷官方从易到难排序。存档里出现过的写法全部保留原样，不做归并改写。
DIFF_ORDER = [
    "入门",
    "普及-",
    "普及/提高-",
    "提高/省选-",
    "省选/NOI-",
    "NOI/NOI+",
    "NOI+",
    "Level 8",
    "暂未评级",
]
DIFF_UNKNOWN = "未标注"

YEAR_RE = re.compile(r"^(19|20)\d{2}$")

# 评测相关标记：不描述算法，只描述判题方式 / 题目形态
JUDGE_TAGS = {
    "O2优化",
    "Special Judge",
    "交互题",
    "交互库",
    "提交答案",
    "通信题",
    "模板题",
    "语言月赛",
}

# 竞赛 / 赛事 / 地区来源（关键词命中即归类）
SOURCE_KEYWORDS = [
    "洛谷", "NOIP", "NOI", "CSP", "ICPC", "CCPC", "IOI", "APIO", "WC", "CTS",
    "THUPC", "THUSC", "THUWC", "CTSC", "CTT", "WF", "Final", "NAC", "SEERC",
    "CERC", "NWRRC", "Moscow", "USACO", "Code Jam", "蓝桥", "省选", "集训队",
    "梦熊", "校赛", "省赛", "邀请赛", "GESP", "Ynoi", "Code+", "夏令营", "科创",
    "初中", "Information", "Olympiad", "Cup", "Round", "Contest", "CSPro",
    "NOISG", "POI", "KOI", "COTS", "ROIR", "COCI", "KTSC", "JOI", "UOI", "RMI",
    "COI", "CCC", "CCO", "eJOI", "CEOI", "Baltic", "NordicOI", "EGOI", "Balkan",
    "INOI", "PAIO", "SWERC", "NERC", "传智", "国创", "天梯", "华为", "字节",
    "腾讯", "阿里巴巴", "小米", "网易", "美团", "百度",
]
SOURCE_EXACT = {
    "北京", "上海", "天津", "重庆", "广东", "浙江", "江苏", "山东", "福建", "湖北",
    "湖南", "河南", "四川", "陕西", "安徽", "台湾", "南京", "西安", "杭州",
    "昆明", "成都", "横浜", "首尔", "各省省选", "省赛/邀请赛", "高校校赛",
    "基础算法",  # 占位，避免误判；见下方 tag_category 的算法白名单
    "其它技巧",
}
# 区域/国家提示词：出现在括号注释里说明是赛事出处
REGION_HINTS = (
    "波兰", "克罗地亚", "俄罗斯", "韩国", "日本", "新加坡", "乌克兰", "罗马尼亚",
    "瑞典", "加拿大", "波罗的海", "欧洲", "中欧", "北欧", "巴尔干", "伊朗",
    "马来西亚", "保加利亚", "东欧", "越南", "印度", "美国", "英国", "德国",
    "法国", "澳大利亚", "南非", "巴西", "墨西哥", "匈牙利", "捷克", "芬兰",
    "挪威", "女生", "集训",
)
# 明确属于算法/数据结构的标签，优先级高于「来源」规则
ALGO_WHITELIST = {"基础算法", "其它技巧", "模拟", "枚举", "排序", "构造"}


def parse_statement(path: Path) -> dict:
    text = path.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()
    head = lines[0] if lines else ""
    m = re.match(r"^#\s*(P\d+)\s*(.*)$", head)
    if m:
        pid, title = m.group(1), m.group(2).strip()
    else:
        pid, title = path.stem, ""
    broken = "Problem not found or unavailable" in text[:400]

    def section(name: str) -> str | None:
        mm = re.search(rf"^##\s*{name}\s*$\s*\n+(.*?)(?=\n##\s|\Z)", text, re.M | re.S)
        if not mm:
            return None
        for line in mm.group(1).splitlines():
            s = line.strip()
            if s:
                return s
        return ""

    diff = section("Difficulty")
    tags_raw = section("Tags")
    tags: list[str] = []
    if tags_raw:
        for t in re.split(r"[,，、]", tags_raw):
            t = t.strip().lstrip("\ufeff").strip("\u200b").strip()
            if t and t not in {"None", "无", "暂无", "-"}:
                tags.append(t)
    return {
        "id": pid,
        "num": int(pid[1:]),
        "title": title if not broken else "（题面缺失）",
        "difficulty": (diff or DIFF_UNKNOWN).strip() if diff else DIFF_UNKNOWN,
        "tags": tags,
        "broken": broken,
        "path": path,
    }


def load_solution_ids() -> set[str]:
    if not SOLUTION.exists():
        return set()
    out = set()
    for p in SOLUTION.glob("*/*.md"):
        m = re.match(r"^(P\d+)_solution\.md$", p.name)
        if m:
            out.add(m.group(1))
    return out


def load_appendix_ids() -> set[str]:
    if not APPENDIX.exists():
        return set()
    return {p.name for p in APPENDIX.iterdir() if p.is_dir() and re.fullmatch(r"P\d+", p.name)}


def tag_category(tag: str) -> str:
    if YEAR_RE.match(tag):
        return "年份"
    if tag in JUDGE_TAGS:
        return "评测/题型标记"
    if tag in ALGO_WHITELIST:
        return "算法/数据结构/技巧"
    for kw in SOURCE_KEYWORDS:
        if kw in tag:
            return "竞赛/来源"
    if tag in SOURCE_EXACT:
        return "竞赛/来源"
    if "（" in tag and any(h in tag for h in REGION_HINTS):
        return "竞赛/来源"
    if tag.endswith(("杯", "赛")) or "比赛" in tag:
        return "竞赛/来源"
    return "算法/数据结构/技巧"


def bar(n: int, peak: int, width: int = 34) -> str:
    if peak <= 0 or n <= 0:
        return ""
    return "█" * max(1, round(n / peak * width))


def pct(n: int, total: int) -> str:
    return f"{n / total * 100:.2f}%" if total else "0.00%"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true", help="只校验元数据完整性")
    args = ap.parse_args()

    files = sorted(STATEMENT.glob("P*-P*/P*.md"))
    if not files:
        print("未找到 statement/ 下的题面文件", file=sys.stderr)
        return 1
    records = [parse_statement(p) for p in files]
    sol_ids = load_solution_ids()
    app_ids = load_appendix_ids()

    total = len(records)
    valid = [r for r in records if not r["broken"]]
    diffs = Counter(r["difficulty"] for r in valid)
    all_tags: Counter[str] = Counter()
    for r in valid:
        all_tags.update(set(r["tags"]))
    n_tag_marks = sum(len(r["tags"]) for r in valid)
    covered = [r for r in valid if r["id"] in sol_ids]
    orphan = sorted(sol_ids - {r["id"] for r in valid})

    if args.check:
        print(f"题面文件 {total}，其中失效 {total - len(valid)}")
        print("难度取值（存档原值）：")
        for k in sorted(diffs, key=lambda x: DIFF_ORDER.index(x) if x in DIFF_ORDER else 99):
            print(f"  {k:<14}{diffs[k]}")
        odd = [r for r in valid if not r["tags"]]
        print(f"无标签题目 {len(odd)}；无 Difficulty 章节 {diffs.get(DIFF_UNKNOWN, 0)}")
        if orphan:
            print(f"题解对应题面失效：{orphan}")
        return 0

    OUT_MD.parent.mkdir(parents=True, exist_ok=True)
    with OUT_CSV.open("w", encoding="utf-8-sig", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["题号", "题目名称", "难度", "标签数", "标签", "有题解", "有附件", "题面失效"])
        for r in sorted(records, key=lambda x: x["num"]):
            w.writerow([
                r["id"], r["title"], r["difficulty"], len(r["tags"]),
                " | ".join(r["tags"]),
                "是" if r["id"] in sol_ids else "否",
                "是" if r["id"] in app_ids else "否",
                "是" if r["broken"] else "否",
            ])

    L: list[str] = []
    today = datetime.date.today().isoformat()
    order = [d for d in DIFF_ORDER if d in diffs] + sorted(d for d in diffs if d not in DIFF_ORDER)
    peak_diff = max(diffs.values()) if diffs else 1

    L.append("# 洛谷题库难度 / 标签统计报告")
    L.append("")
    L.append(
        f"> 数据来源：本仓库 `statement/` 下全部题面的 `## Difficulty`、`## Tags` 元数据，"
        f"交叉核对 `solution/`、`appendix/` 的实际覆盖情况。"
        f"统计口径与已知偏差见[第六节](#六数据质量与口径说明)。"
        f"本文件由 `scripts/gen_statistics.py` 于 {today} 自动生成，**请勿手工编辑**。"
    )
    L.append("")

    # ---------------- 一、总览 ----------------
    L.append("## 一、总览")
    L.append("")
    L.append("| 指标 | 数值 |")
    L.append("| :--- | ---: |")
    L.append(f"| 题面文件总数 | {total:,} |")
    L.append(f"| 元数据可用题面 | {len(valid):,} |")
    L.append(f"| 题面失效（抓取失败） | {total - len(valid):,} |")
    L.append(f"| 难度取值种数 | {len(diffs)} |")
    L.append(f"| 标签种数 | {len(all_tags):,} |")
    L.append(f"| 标签标注总数（含重复） | {n_tag_marks:,} |")
    L.append(f"| 平均每题标签数 | {n_tag_marks / len(valid):.2f} |")
    L.append(f"| 已写题解 `solution/` | {len(covered):,}（{pct(len(covered), len(valid))}） |")
    L.append(f"| 有附件 `appendix/` | {len(app_ids & {r['id'] for r in valid}):,} |")
    L.append("")

    # ---------------- 二、难度 ----------------
    L.append("## 二、难度分布")
    L.append("")
    L.append("按洛谷官方难度序从易到难排列，取值为存档原样（未做归并）。")
    L.append("")
    L.append("| 难度 | 题目数 | 占比 | 累计占比 | 分布 |")
    L.append("| :--- | ---: | ---: | ---: | :--- |")
    cum = 0
    for d in order:
        n = diffs[d]
        cum += n
        L.append(
            f"| {d} | {n:,} | {pct(n, len(valid))} | {pct(cum, len(valid))} | {bar(n, peak_diff)} |"
        )
    L.append(f"| **合计** | **{len(valid):,}** | 100.00% | | |")
    L.append("")
    if order:
        top = max(order, key=lambda d: diffs[d])
        L.append(f"- 题目最多的档位：**{top}**，共 {diffs[top]:,} 题（{pct(diffs[top], len(valid))}）。")
        easy = sum(diffs[d] for d in order[:3])
        L.append(f"- 入门 / 普及- / 普及/提高- 三档合计 {easy:,} 题，占 {pct(easy, len(valid))}。")
        rated = len(valid) - diffs.get("暂未评级", 0) - diffs.get(DIFF_UNKNOWN, 0)
        L.append(f"- 有明确难度评级的题目 {rated:,} 题，另有 {diffs.get('暂未评级', 0):,} 题「暂未评级」。")
        L.append("")

    # ---------------- 三、标签 ----------------
    L.append("## 三、标签分布")
    L.append("")
    L.append(f"全库共出现 **{len(all_tags):,}** 种不同标签。下表为出现次数最多的 50 种，"
             "全量清单见文末[附录](#附录全量标签表)。")
    L.append("")
    L.append("| # | 标签 | 类别 | 题目数 | 占题库 | 分布 |")
    L.append("| ---: | :--- | :--- | ---: | ---: | :--- |")
    peak_tag = all_tags.most_common(1)[0][1]
    for i, (t, c) in enumerate(all_tags.most_common(50), 1):
        L.append(
            f"| {i} | {t} | {tag_category(t)} | {c:,} | {pct(c, len(valid))} | {bar(c, peak_tag)} |"
        )
    L.append("")

    L.append("### 3.1 按类别汇总")
    L.append("")
    cat_marks: Counter[str] = Counter()
    cat_unique: defaultdict[str, set[str]] = defaultdict(set)
    cat_problems: Counter[str] = Counter()
    for t, c in all_tags.items():
        cat = tag_category(t)
        cat_marks[cat] += c
        cat_unique[cat].add(t)
    for r in valid:
        for cat in {tag_category(t) for t in r["tags"]}:
            cat_problems[cat] += 1
    cat_desc = {
        "算法/数据结构/技巧": "考察的知识点与解题方法",
        "竞赛/来源": "赛事、省份、出题方等出处信息",
        "年份": "题目所属年份",
        "评测/题型标记": "判题方式与题目形态，非算法属性",
    }
    L.append("| 类别 | 标签种数 | 出现次数 | 涉及题目数 | 说明 |")
    L.append("| :--- | ---: | ---: | ---: | :--- |")
    for cat in ["算法/数据结构/技巧", "竞赛/来源", "年份", "评测/题型标记"]:
        if cat in cat_marks:
            L.append(
                f"| {cat} | {len(cat_unique[cat]):,} | {cat_marks[cat]:,} | "
                f"{cat_problems[cat]:,} | {cat_desc[cat]} |"
            )
    L.append("")
    L.append("> 类别由脚本按规则启发式判定（年份按四位数、来源按关键词与括号中的国家/地区、"
             "评测标记按固定名单），存在少量误分可能。")
    L.append("")

    L.append("### 3.2 各难度层的高频算法标签")
    L.append("")
    L.append("只统计「算法/数据结构/技巧」类标签，出处与评测标记已排除，用于看难度爬升时考点如何演进。")
    L.append("")
    L.append("| 难度 | 题目数 | 高频标签（题数） |")
    L.append("| :--- | ---: | :--- |")
    for d in order:
        sub = [r for r in valid if r["difficulty"] == d]
        if not sub:
            continue
        c = Counter()
        for r in sub:
            c.update({t for t in r["tags"] if tag_category(t) == "算法/数据结构/技巧"})
        top = "、".join(f"{t}（{n}）" for t, n in c.most_common(6))
        L.append(f"| {d} | {len(sub):,} | {top} |")
    L.append("")

    L.append("### 3.3 标签组合（共现 Top 30）")
    L.append("")
    L.append("统计同一道题上同时出现的「算法/数据结构/技巧」标签组合，可用于设计成套练习。")
    L.append("")
    pair: Counter[tuple[str, str]] = Counter()
    for r in valid:
        ts = sorted({t for t in r["tags"] if tag_category(t) == "算法/数据结构/技巧"})
        for i in range(len(ts)):
            for j in range(i + 1, len(ts)):
                pair[(ts[i], ts[j])] += 1
    L.append("| # | 标签组合 | 同时出现的题目数 |")
    L.append("| ---: | :--- | ---: |")
    for i, ((a, b), c) in enumerate(pair.most_common(30), 1):
        L.append(f"| {i} | {a} + {b} | {c:,} |")
    L.append("")

    # ---------------- 四、覆盖 ----------------
    L.append("## 四、题解覆盖情况")
    L.append("")
    L.append(f"`solution/` 已覆盖 **{len(covered):,} / {len(valid):,}** 题，"
             f"整体完成度 **{pct(len(covered), len(valid))}**。")
    if orphan:
        L.append("")
        L.append(f"> 另有 {len(orphan)} 份题解对应的题面本身已失效（{', '.join(orphan)}），未计入分母。")
    L.append("")
    L.append("| 难度 | 题目数 | 已写题解 | 完成度 | 剩余 |")
    L.append("| :--- | ---: | ---: | ---: | ---: |")
    for d in order:
        sub = [r for r in valid if r["difficulty"] == d]
        if not sub:
            continue
        done = sum(1 for r in sub if r["id"] in sol_ids)
        L.append(f"| {d} | {len(sub):,} | {done:,} | {pct(done, len(sub))} | {len(sub) - done:,} |")
    L.append("")

    # ---------------- 五、分段 ----------------
    L.append("## 五、题号分段分布")
    L.append("")
    buckets: defaultdict[int, list[int]] = defaultdict(lambda: [0, 0])
    for r in valid:
        if r["broken"]:
            continue
        key = (r["num"] // 1000) * 1000
        buckets[key][0] += 1
        if r["id"] in sol_ids:
            buckets[key][1] += 1
    L.append("| 分段 | 题面数 | 已写题解 | 完成度 |")
    L.append("| :--- | ---: | ---: | ---: |")
    for key in sorted(buckets):
        n, done = buckets[key]
        L.append(f"| `P{key}–P{key + 999}` | {n:,} | {done:,} | {pct(done, n)} |")
    L.append("")

    # ---------------- 六、口径 ----------------
    L.append("## 六、数据质量与口径说明")
    L.append("")
    L.append("统计完全基于本仓库 `statement/` 的题面头部元数据，**未访问洛谷站外数据**，"
             "以下是使用时必须知道的几件事：")
    L.append("")
    L.append("1. **难度取值照抄存档，不做归并**。存档里共存 9 种写法：官方 8 档名称的"
             "去加号版本（`提高/省选-` 对应 `提高+/省选−`、`省选/NOI-` 对应 `省选/NOI−`）、"
             "`暂未评级`，以及 `NOI+`、`NOI/NOI+`、`Level 8` 三种同时指向最高难度区间的写法——"
             "它们很可能来自不同批次的抓取脚本。报告按官方难度序排列，但保留原值以便回溯。")
    L.append(f"2. **高难度段占比明显偏高，存在系统性偏移风险**。存档中 `NOI+` 占 "
             f"{pct(diffs.get('NOI+', 0), len(valid))}，而 `入门` 仅占 "
             f"{pct(diffs.get('入门', 0), len(valid))}，与洛谷主站以入门/普及题为主体的真实分布不符；"
             "`NOI+`（" + f"{diffs.get('NOI+', 0):,}" + "）又远多于 `NOI/NOI+`（"
             + f"{diffs.get('NOI/NOI+', 0):,}" + "），说明抓取脚本使用的难度名称表与洛谷官方档位并未严格对齐。"
             "**引用具体难度前请以洛谷原题页面为准**，本表只适合做相对比较与覆盖度统计。")
    L.append(f"3. **失效题面**：{total - len(valid)} 份题面正文为 `Problem not found or unavailable`，"
             "已从全部统计中剔除，明细见 CSV 的「题面失效」列。")
    no_tag = sum(1 for r in valid if not r["tags"])
    missing = []
    if diffs.get(DIFF_UNKNOWN, 0):
        missing.append(f"{diffs[DIFF_UNKNOWN]} 份题面缺少 `## Difficulty` 章节（记为 `{DIFF_UNKNOWN}`）")
    if no_tag:
        missing.append(f"{no_tag} 份题面缺少 `## Tags`（记为 0 个标签）")
    if missing:
        L.append("4. **缺失元数据**：" + "，".join(missing) + "。")
    else:
        L.append("4. **元数据完整**：全部有效题面都带有 `## Difficulty` 与 `## Tags` 章节。")
    L.append("5. **难度与标签会变**：洛谷会不定期调整题目难度、增删标签，本报告只反映抓取时的快照。")
    L.append("6. **标签类别是启发式划分**，仅供聚合参考，逐题精确标签以 CSV 为准。")
    L.append("")

    # ---------------- 七、用法 ----------------
    L.append("## 七、如何更新与查询本报告")
    L.append("")
    L.append("```bash")
    L.append("# 1) 先校验元数据是否齐全（不写文件）")
    L.append("python3 scripts/gen_statistics.py --check")
    L.append("")
    L.append("# 2) 重新生成报告与逐题 CSV")
    L.append("python3 scripts/gen_statistics.py")
    L.append("```")
    L.append("")
    L.append("脚本只读取 `statement/`、`solution/`、`appendix/`，不会修改任何题面或题解。")
    L.append("逐题明细的常用查法（`reference/problem-data.csv` 为 UTF-8 BOM，Excel 可直接打开）：")
    L.append("")
    L.append("```bash")
    L.append("# 查某道题的难度与标签")
    L.append("grep '^P1004,' reference/problem-data.csv")
    L.append("")
    L.append("# 列出所有「线段树」但还没写题解的题")
    L.append("awk -F',' '$5 ~ /线段树/ && $6 == \"否\" {print $1, $2}' reference/problem-data.csv")
    L.append("")
    L.append("# 按难度筛选已写题解的数量")
    L.append("cut -d',' -f3,6 reference/problem-data.csv | sort | uniq -c")
    L.append("```")
    L.append("")

    # ---------------- 附录 ----------------
    L.append("## 附录：全量标签表")
    L.append("")
    L.append(f"<details><summary>展开查看全部 {len(all_tags):,} 种标签及出现次数</summary>")
    L.append("")
    L.append("| # | 标签 | 类别 | 题目数 |")
    L.append("| ---: | :--- | :--- | ---: |")
    for i, (t, c) in enumerate(all_tags.most_common(), 1):
        L.append(f"| {i} | {t} | {tag_category(t)} | {c:,} |")
    L.append("")
    L.append("</details>")
    L.append("")

    OUT_MD.write_text("\n".join(L) + "\n", encoding="utf-8")
    print(f"已写入 {OUT_MD.relative_to(ROOT)}（{total} 题，{len(all_tags)} 种标签）")
    print(f"已写入 {OUT_CSV.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
