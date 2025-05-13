#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""
OPTIONS="b e n s t v E T"
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BASE=`tput setaf 7`
declare -a tests=(
    "VAR tests/test_1_cat.txt"
    "VAR tests/test_2_cat.txt"
    "VAR tests/test_3_cat.txt"
    "VAR tests/test_4_cat.txt"
    "VAR tests/test_5_cat.txt"
    "VAR tests/test_case_cat.txt"
)
declare -a extra=(
    "tests/test_1_cat.txt"
    "-b -e -n -s -t -v tests/test_1_cat.txt"
    "-t tests/test_3_cat.txt"
    "-ET tests/test_4_cat.txt"
    "-nb tests/test_1_cat.txt"
    "-s -n -e tests/test_4_cat.txt"
    "-n tests/test_1_cat.txt"
    "-ne tests/test_1_cat.txt"
    "-n tests/test_1_cat.txt tests/test_2_cat.txt"
    "-v tests/test_5_cat.txt"
    "no_file.txt"
)

testing() {
    t=`echo $@ | sed "s/VAR/$var/"`
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES=`diff test_s21_cat.log test_sys_cat.log -q`
    RESULT="SUCCESS"
    if [ -z "$DIFF_RES" ]
    then
      (( SUCCESS++ ))
        RESULT="SUCCESS"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"

    else
      (( FAIL++ ))
        RESULT="FAIL"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${RED}${RESULT}${BASE} cat $t"

    fi
    # echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in $OPTIONS
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in $OPTIONS
do
    for var2 in $OPTIONS
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"