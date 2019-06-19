find ./src -name \*.h\* -print -o -name \*.c\* -print | xargs clang-format -style=file -i
find ./sdk -name \*.h\* -print -o -name \*.c\* -print | xargs clang-format -style=file -i
