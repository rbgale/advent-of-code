#!/usr/bin/env bash
set -euo pipefail

YEAR=${1:?Usage: $0 YEAR DAY}
DAY=${2:?Usage: $0 YEAR DAY}
DAY_PADDED=$(printf '%02d' "$DAY")
ROOT=$(cd "$(dirname "$0")" && pwd)
YEAR_DIRECTORY="$ROOT/$YEAR"
DAY_DIRECTORY="$YEAR_DIRECTORY/day$DAY_PADDED"

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

"$ROOT/fetch-inputs.sh" "$YEAR" "$DAY" || true

echo "Done: created $DAY_DIRECTORY"
