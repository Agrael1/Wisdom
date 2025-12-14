#!/usr/bin/env python3
"""
Version management script for Wisdom project
Usage: python version.py [major|minor|patch|set <version>|get]
"""

import re
import sys
from pathlib import Path

VERSION_FILE = Path(__file__).parent / "VERSION"


def read_version():
    """Read current version from VERSION file"""
    return VERSION_FILE.read_text().strip()


def write_version(version):
    """Write version to VERSION file"""
    VERSION_FILE.write_text(f"{version}\n")


def parse_version(version_str):
    """Parse version string into components"""
    match = re.match(r"^(\d+)\.(\d+)\.(\d+)$", version_str)
    if not match:
        raise ValueError(f"Invalid version format: {version_str}")
    return tuple(map(int, match.groups()))


def bump_version(bump_type):
    """Bump version according to type (major, minor, patch)"""
    current = read_version()
    major, minor, patch = parse_version(current)

    if bump_type == "major":
        major += 1
        minor = 0
        patch = 0
    elif bump_type == "minor":
        minor += 1
        patch = 0
    elif bump_type == "patch":
        patch += 1
    else:
        raise ValueError(f"Invalid bump type: {bump_type}")

    new_version = f"{major}.{minor}.{patch}"
    write_version(new_version)
    return new_version


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} [major|minor|patch|set <version>|get]")
        sys.exit(1)

    command = sys.argv[1].lower()

    try:
        if command == "get":
            print(read_version())
        elif command == "set":
            if len(sys.argv) < 3:
                print("Error: 'set' requires a version argument")
                sys.exit(1)
            version = sys.argv[2]
            parse_version(version)  # Validate format
            write_version(version)
            print(f"Version set to: {version}")
        elif command in ["major", "minor", "patch"]:
            old_version = read_version()
            new_version = bump_version(command)
            print(f"Version bumped: {old_version} -> {new_version}")
        else:
            print(f"Error: Unknown command '{command}'")
            sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
