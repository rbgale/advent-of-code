#!/usr/bin/env bash
set -euo pipefail

YEAR=${1:-}
DAY=${2:-}
ROOT=$(cd "$(dirname "$0")" && pwd)

AOC_SESSION=${AOC_SESSION:-}
[[ -z "$AOC_SESSION" && -f "$ROOT/.aoc_session" ]] && AOC_SESSION=$(cat "$ROOT/.aoc_session")

if [[ -z "$AOC_SESSION" ]]; then
    echo "Error: no session token. Write your token to $ROOT/.aoc_session or set AOC_SESSION." >&2
    exit 1
fi

FAILED=0

fetch_input() {
    local year=$1 day=$2
    local day_padded
    day_padded=$(printf '%02d' "$day")
    local day_dir="$ROOT/$year/day$day_padded"

    if [[ ! -d "$day_dir" ]]; then
        echo "Error: $day_dir does not exist." >&2
        FAILED=1
        return
    fi

    local http_code
    http_code=$(curl -s -o "$day_dir/input.txt" -w "%{http_code}" \
        "https://adventofcode.com/$year/day/$day/input" \
        -H "Cookie: session=$AOC_SESSION" \
        -H "User-Agent: github.com/rbgale/advent-of-code fetch-inputs.sh")

    if [[ "$http_code" != "200" ]]; then
        rm -f "$day_dir/input.txt"
        case "$http_code" in
            400) echo "Error: bad session token (HTTP 400) for $year day $day. Write your token to $ROOT/.aoc_session or set AOC_SESSION." >&2 ;;
            404) echo "Error: puzzle not found (HTTP 404) for $year day $day. Is it unlocked yet?" >&2 ;;
            *)   echo "Error: unexpected HTTP $http_code for $year day $day." >&2 ;;
        esac
        FAILED=1
        return
    fi

    echo "Fetched: $year/day$day_padded/input.txt"
}

shopt -s nullglob

if [[ -n "$YEAR" && -n "$DAY" ]]; then
    fetch_input "$YEAR" "$DAY"
elif [[ -n "$YEAR" ]]; then
    for day_dir in "$ROOT/$YEAR"/day*/; do
        day_num=$(basename "$day_dir")
        day_num=${day_num#day}
        day_num=$((10#$day_num))
        fetch_input "$YEAR" "$day_num"
    done
else
    for year_dir in "$ROOT"/[0-9][0-9][0-9][0-9]/; do
        year=$(basename "$year_dir")
        for day_dir in "$year_dir"day*/; do
            day_num=$(basename "$day_dir")
            day_num=${day_num#day}
            day_num=$((10#$day_num))
            fetch_input "$year" "$day_num"
        done
    done
fi

exit $FAILED
