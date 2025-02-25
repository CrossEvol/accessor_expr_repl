# Intro
this repo is for repl of the nested accessor such as `a.b[0].c.d[1]`

design a very simple commandline repl program

below is its grammar:
```text
$x =  1 
$array = @[1,2,]
@map = @{k1 = 1, k2 = 2,}
$y = nil
array[0] = map
array[0].k1
```

for simplicity, it does not take auto resizing for array and auto inserting for hash-map which are commonly seen in dynamic language into consideration

# Link
- [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) 
- [cmake-examples](https://github.com/ttroy50/cmake-examples)
- [Catch2](https://github.com/catchorg/Catch2) 
- [googletest](https://github.com/google/googletest)
- [fmt](https://github.com/fmtlib/fmt)