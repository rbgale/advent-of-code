#!/usr/bin/env bash
set -euo pipefail

YEAR=${1:-}
DAY=${2:-}
ROOT=$(cd "$(dirname "$0")" && pwd)

shopt -s nullglob
files=()
if [[ -n "$YEAR" && -n "$DAY" ]]; then
    DAY_PADDED=$(printf '%02d' "$DAY")
    local_file="$ROOT/$YEAR/day$DAY_PADDED/input.txt"
    [[ -f "$local_file" ]] && files=("$local_file")
elif [[ -n "$YEAR" ]]; then
    files=("$ROOT/$YEAR"/day*/input.txt)
else
    files=("$ROOT"/[0-9][0-9][0-9][0-9]/day*/input.txt)
fi

count=${#files[@]}

if [[ $count -eq 0 ]]; then
    echo "No input files found."
    exit 0
fi

echo "This will delete $count input file(s)."
read -r -p "Continue? [y/N] " response

case "$response" in
    [yY][eE][sS]|[yY]) ;;
    *) echo "Aborted."; exit 0 ;;
esac

for f in "${files[@]}"; do
    rm "$f"
    echo "Deleted: ${f#"$ROOT"/}"
done
