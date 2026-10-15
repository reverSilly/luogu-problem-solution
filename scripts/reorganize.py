#!/usr/bin/env python3
"""
Repository file architecture reorganization & maintenance tool for luogu-problem-solution.

Categorizes files into:
  - statement/P{xxxx}-P{yyyy}/P{id}.md
  - solution/P{xxxx}-P{yyyy}/P{id}_solution.md
  - appendix/P{id}/... (std programs, generators, checkers, traces, testcases)
  - reference/...      (SKILL definitions, coding style guides)
  - scripts/...        (repository maintenance scripts)

Usage:
  python3 scripts/reorganize.py [--root .] [--dry-run] [--git] [--flat]
"""

import argparse
import os
import re
import shutil
import subprocess
from pathlib import Path
from typing import Optional, Tuple

STMT_RE = re.compile(r"^([PB])(\d+)\.md$")
SOL_RE = re.compile(r"^([PB])(\d+)_solution\.md$")
APPENDIX_RE = re.compile(
    r"^([PB])(\d+)(?:_(?:check|checker|gen|generator|brute|bf|std|trace|sim|test).*)?\.(?:cpp|cc|cxx|c|h|hpp|py|sh|in|out|ans|txt)$"
)

REFERENCE_FILES = {
    "SKILL.write-oi-solution.md",
    "coding-style-guide.md",
}

SPECIAL_APPENDIX_MAP = {
    "temp_trace.py": ("P1139", "P1139_trace.py"),
}


def get_bucket_dir(prefix: str, num: int) -> str:
    base = (num // 1000) * 1000
    if base < 10000:
        return f"{prefix}{base:04d}-{prefix}{base + 999:04d}"
    return f"{prefix}{base}-{prefix}{base + 999}"


def classify_file(filename: str, flat: bool = False) -> Optional[Path]:
    """Return target relative path for a root-level file, or None if it stays at root."""
    if filename in {"README.md", ".gitignore", ".gitattributes", "LICENSE"}:
        return None
    if filename == "reorganize.py":
        return Path("scripts") / "reorganize.py"
    if filename in REFERENCE_FILES or filename.startswith("SKILL."):
        return Path("reference") / filename
    if filename in SPECIAL_APPENDIX_MAP:
        pid, new_name = SPECIAL_APPENDIX_MAP[filename]
        return Path("appendix") / pid / new_name

    m_stmt = STMT_RE.match(filename)
    if m_stmt:
        prefix, num_str = m_stmt.group(1), m_stmt.group(2)
        if flat:
            return Path("statement") / filename
        bucket = get_bucket_dir(prefix, int(num_str))
        return Path("statement") / bucket / filename

    m_sol = SOL_RE.match(filename)
    if m_sol:
        prefix, num_str = m_sol.group(1), m_sol.group(2)
        if flat:
            return Path("solution") / filename
        bucket = get_bucket_dir(prefix, int(num_str))
        return Path("solution") / bucket / filename

    m_app = APPENDIX_RE.match(filename)
    if m_app:
        pid = f"{m_app.group(1)}{m_app.group(2)}"
        return Path("appendix") / pid / filename

    return None


def move_file(src: Path, dst: Path, use_git: bool, dry_run: bool) -> None:
    if src.resolve() == dst.resolve():
        return
    if dry_run:
        print(f"[DRY-RUN] {src} -> {dst}")
        return
    dst.parent.mkdir(parents=True, exist_ok=True)
    if use_git:
        res = subprocess.run(["git", "mv", str(src), str(dst)], capture_output=True, text=True)
        if res.returncode == 0:
            return
    shutil.move(str(src), str(dst))


def reorganize(root: Path, dry_run: bool = False, use_git: bool = False, flat: bool = False) -> Tuple[int, dict]:
    counts = {"statement": 0, "solution": 0, "appendix": 0, "reference": 0, "scripts": 0, "skipped": 0}
    moved = 0

    for entry in sorted(root.iterdir(), key=lambda p: p.name):
        if entry.is_dir():
            continue
        target_rel = classify_file(entry.name, flat=flat)
        if target_rel is None:
            counts["skipped"] += 1
            continue
        top_cat = target_rel.parts[0]
        counts[top_cat] = counts.get(top_cat, 0) + 1
        move_file(entry, root / target_rel, use_git=use_git, dry_run=dry_run)
        moved += 1

    return moved, counts


def main() -> None:
    parser = argparse.ArgumentParser(description="Reorganize luogu-problem-solution file structure.")
    parser.add_argument("--root", type=Path, default=Path("."), help="Repository root directory")
    parser.add_argument("--dry-run", action="store_true", help="Preview moves without modifying disk")
    parser.add_argument("--git", action="store_true", help="Use 'git mv' when moving tracked files")
    parser.add_argument("--flat", action="store_true", help="Do not shard statement/ and solution/ into 1000-ID buckets")
    args = parser.parse_args()

    moved, counts = reorganize(args.root.resolve(), dry_run=args.dry_run, use_git=args.git, flat=args.flat)
    mode = "DRY-RUN" if args.dry_run else "COMPLETED"
    print(f"[{mode}] Total files processed: {moved}")
    for k, v in counts.items():
        print(f"  {k:10s}: {v}")


if __name__ == "__main__":
    main()
