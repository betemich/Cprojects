#!/bin/bash
FAIL=0
SUCCESS=0
COUNTER=1

my_program=(
    "./s21_cat"
)

orig_program=(
    "cat"
)

tests=(
    "-benst my_tests/test1.txt"
    "-bens my_tests/test1.txt"
    "-bnst my_tests/test1.txt my_tests/test2.txt"
    "-bn my_tests/test2.txt -s -t"
    "-v my_tests/test1.txt"
    "-E my_tests/test3.txt -T"
    "--number-nonblank my_tests/test2.txt --squeeze-blank"
    "-sE my_tests/test1.txt my_tests/test2.txt my_tests/test3.txt -v"
    "-sE my_tests/test1.txt my_tests/test2.txt -v -nofile.txt my_tests/test3.txt"
    "-et my_tests/test1.txt nofile1.txt nofile2.txt my_tests/test2.txt"
    "-bs my_tests/test1.txt test3.txt"
    "my_tests/test1.txt"
    "no_file.txt -Tev"
    "-se my_tests/test4.txt my_tests/test5.txt my_tests/test6.txt"
    "-sE my_tests/test4.txt my_tests/test5.txt my_tests/test6.txt"
    "-s my_tests/test4.txt my_tests/test5.txt my_tests/test6.txt"
    "-e my_tests/test4.txt my_tests/test5.txt my_tests/test6.txt"

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
echo "SUCCESS $SUCCESS"
echo "FAIL $FAIL"