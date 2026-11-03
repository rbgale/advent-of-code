#!/usr/bin/env python3
"""Update the stars table in README.md for modified years."""
import json
import os
import subprocess
import sys
import urllib.error
import urllib.request
from pathlib import Path

USER_ID = "4747526"
FULL_STARS = "⭐⭐"
HALF_STARS = "⭐☆"
NO_STARS = "☆☆"

README = Path("README.md")
START = "<!-- aoc-stars:start -->"
END = "<!-- aoc-stars:end -->"
COLS = 25


def is_year(name):
    return len(name) == 4 and name.isdecimal()


def changed_years(before):
    """Years whose folders changed since commit `before`, or None if unknown.

    An empty or all-zeros `before` makes git diff fail, which also returns None.
    """
    try:
        out = subprocess.run(args=["git", "diff", "--name-only", before],
                             capture_output=True, text=True, check=True).stdout
    except (subprocess.CalledProcessError, FileNotFoundError):
        return None

    parent_directories = {line.split("/", maxsplit=1)[0] for line in out.splitlines()}

    return {int(directory) for directory in parent_directories if is_year(directory)}


def preexisting_rows_map(table):
    rows = {}

    for line in table.splitlines():
        year = line.partition("|")[2].partition("|")[0].strip()
        if is_year(year):
            rows[int(year)] = line

    return rows


def fetch_stars(year, session):
    req = urllib.request.Request(
        url=f"https://adventofcode.com/{year}/leaderboard/private/view/{USER_ID}.json",
        headers={
            "Cookie": f"session={session}",
            "User-Agent": f"github.com/{os.environ.get('GITHUB_REPOSITORY')} README stars updater",
        }
    )

    try:
        with urllib.request.urlopen(req, timeout=15) as response:
            leaderboard = json.load(response)
    except (urllib.error.HTTPError, json.JSONDecodeError) as e:
        sys.exit(f"Can't get {year} stars; the session cookie may be expired or USER_ID is wrong: {e}")

    # 'completion_day_level' format:
    # {
    #   "1": { "1": {"get_star_ts": 1764547200}, "2": {"get_star_ts": 1764590400}},
    #   "2": {"1": {"get_star_ts": 1764633600}}
    # }
    daily_progress = leaderboard["members"][USER_ID]["completion_day_level"]

    return {int(day): len(daily_progress[day]) for day in daily_progress}


def render_row(year, stars):
    cells = []
    for day in range(1, COLS + 1):
        if year >= 2025 and day > 12:
            cells.append("")
            continue

        day_stars = stars.get(day, 0)
        cells.append(FULL_STARS if day_stars == 2 else HALF_STARS if day_stars == 1 else NO_STARS)

    return f"| {year} | " + " | ".join(cells) + f" |"


def render_table(rows):
    header = "| Year | " + " | ".join(str(day) for day in range(1, COLS + 1)) + " |"
    separator = "|" + "|".join([":--"] + [":-:"] * COLS) + "|"
    body = [rows[y] for y in sorted(rows, reverse=True)]

    return "\n".join([header, separator] + body)


def main():
    session = os.environ.get("AOC_SESSION") or Path(".aoc_session").read_text().strip()
    years = {int(p.name) for p in Path(".").iterdir() if p.is_dir() and is_year(p.name)}
    text = README.read_text()

    if START not in text or END not in text:
        sys.exit(f"The README.md needs a '{START}' line and '{END}' line.")

    head, rest = text.split(START, maxsplit=1)
    old_table, tail = rest.split(END, maxsplit=1)
    rows_map = preexisting_rows_map(old_table)

    changed = changed_years(os.environ.get("BEFORE_SHA", ""))
    to_fetch = years if changed is None else changed & years

    for year in to_fetch:
        rows_map[year] = render_row(year, fetch_stars(year, session))

    README.write_text(f"{head}{START}\n{render_table(rows_map)}\n{END}{tail}")


if __name__ == "__main__":
    main()
