#!/usr/bin/env bash
set -euo pipefail

YEAR=${1:?Usage: $0 YEAR DAY}
DAY=${2:?Usage: $0 YEAR DAY}
DAY_PADDED=$(printf '%02d' "$DAY")
ROOT=$(cd "$(dirname "$0")" && pwd)
YEAR_DIRECTORY="$ROOT/$YEAR"
DAY_DIRECTORY="$YEAR_DIRECTORY/day$DAY_PADDED"

AOC_SESSION=${AOC_SESSION:-}
[[ -z "$AOC_SESSION" && -f "$ROOT/.aoc_session" ]] && AOC_SESSION=$(cat "$ROOT/.aoc_session")
SESSION_HINT="Write your token to $ROOT/.aoc_session or set AOC_SESSION"

if [[ ! -d "$YEAR_DIRECTORY" ]]; then
    mkdir -p "$YEAR_DIRECTORY"
    echo "Year $YEAR not found. Created $YEAR/"
fi

if [[ -d "$DAY_DIRECTORY" ]]; then
    echo "Error: $DAY_DIRECTORY already exists." >&2
    exit 1
fi

mkdir -p "$DAY_DIRECTORY"

cat > "$DAY_DIRECTORY/part1.cpp" << 'EOF'
#include <print>
#include "aoc.h"

auto main() -> int {
    auto f{AOC_INPUT()};

    std::println("{}", 0);
    return 0;
}
EOF

cp "$DAY_DIRECTORY/part1.cpp" "$DAY_DIRECTORY/part2.cpp"

if [[ -n "$AOC_SESSION" ]]; then
    HTTP_CODE=$(curl -s -o "$DAY_DIRECTORY/input.txt" -w "%{http_code}" \
        "https://adventofcode.com/$YEAR/day/$DAY/input" \
        -H "Cookie: session=$AOC_SESSION" \
        -H "User-Agent: github.com/rbgale/advent-of-code new-day.sh")
    if [[ "$HTTP_CODE" != "200" ]]; then
        case "$HTTP_CODE" in
            400) echo "Error: bad session token (HTTP 400). $SESSION_HINT" >&2 ;;
            404) echo "Error: puzzle not found (HTTP 404). Is day $DAY of $YEAR unlocked yet?" >&2 ;;
            *)   echo "Error: unexpected HTTP $HTTP_CODE fetching input." >&2 ;;
        esac
        rm -f "$DAY_DIRECTORY/input.txt"
    fi
else
    echo "No session token found. $SESSION_HINT" >&2
fi

echo "Done: $DAY_DIRECTORY"
