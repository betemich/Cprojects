#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=1

my_program=(
    "./s21_grep"
)

orig_program=(
    "grep"
)

tests=(
    "-e a my_tests/test1.txt"
    "a my_tests/test1.txt"
    "b -ivc my_tests/test1.txt"
    "b -ivc my_tests/test1.txt my_tests/test2.txt"
    "d my_tests/test1.txt -lc"
    "-eh c my_tests/test1.txt"
    "-f my_tests/test1.txt my_tests/test2.txt -no"
    ") my_tests/test2.txt"
    "#if -e a -e b my_tests/test1.txt -f my_tests/test3.txt"
    "-ivnh a my_tests/no_file.txt -s -e b my_tests/test3.txt"
    "-o -s -v -i my_tests/test2.txt my_tests/test3.txt"
    "-f my_tests/no_file.txt -v my_tests/test2.txt"
    "\"\" -osv my_tests/test2.txt"
)

run_test() {
    "${my_program[@]}" $@ > "${my_program[@]}".log
    "${orig_program[@]}" $@ > "${orig_program[@]}".log
    DIFF="$(diff -s "${my_program[@]}".log "${orig_program[@]}".log)"
    if [ "$DIFF" == "Files "${my_program[@]}".log and "${orig_program[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $@"
    else 
        let "FAIL++"
        echo "$COUNTER - Fail $@"
    fi
    let "COUNTER++"
    rm -f "${my_program[@]}".log "${orig_program[@]}".log
}

echo "#####START TEST#####"

for i in "${tests[@]}"
do
    run_test "$i"
done

printf "\n"
echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
##############################