#!/bin/sh

FORMAT_TARGETS=("wisdom" "examples")

function format() {
    for f in $(find $@ -name '*.h' -or -name '*.inl' -or -name '*.ixx' -or -name '*.m' -or -name '*.mm' -or -name '*.c' -or -name '*.cpp'); do 
        echo "format ${f}";
        clang-format -i ${f};
    done

    echo "~~~ $@ Done ~~~";
}

for dir in "$FORMAT_TARGETS"; do
    if [ -d "${dir}" ]; then
        format ${dir};
    fi
done
