#!/usr/bin/env python3
import shutil
import subprocess
from pathlib import Path
import sys


def main() -> int:
    clang_format = shutil.which("clang-format") or shutil.which("clang-format.exe")
    if clang_format is None:
        print("clang-format not found on PATH.")
        return 1

    files = [p for p in Path("./src/").rglob("*") if p.is_file() and p.suffix in {".cpp", ".h"}]

    if not files:
        print("No .cpp or .h files found.")
        return 0

    changed_count = 0
    for path in files:
        before = path.read_bytes()
        subprocess.run([clang_format, "-i", str(path)], check=True)
        after = path.read_bytes()
        if before != after:
            changed_count += 1

    print(f"Formatted {len(files)} files; modified {changed_count}.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
