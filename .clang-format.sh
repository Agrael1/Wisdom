#!/bin/bash

FORMAT_TARGETS=("src" "examples")

function format() {
  find "$@" \( -name '*.h' -or -name '*.hpp' -or -name '*.ixx' -or -name '*.m' -or -name '*.mm' -or -name '*.c' -or -name '*.cpp' \) ! -name 'xxhash.h' -print0 |
    while IFS= read -r -d '' f; do
      echo "format $f"
      clang-format -i "$f"
    done

  echo "~~~ $@ Done ~~~"
}

for dir in "$FORMAT_TARGETS"; do
  if [ -d "$dir" ]; then
    format "$dir"
  fi
done
