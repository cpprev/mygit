#!/bin/bash

function init_test ()
{
    test_name=$1

    ./../build/mygit init &> /dev/null
    ret=$?

    if [ $ret -eq 1 ]; then
        echo -e "$red[KO]""$cyan""[$test_name]"
    fi

    FILE1=".mygit"
    FILE2=".mygit/objects"
    FILE3=".mygit/refs"
    if ! [ -d "$FILE1" ] || ! [ -d "$FILE2" ] || ! [ -d "$FILE3" ]; then
        echo -e "$red[KO]""$cyan""[$test_name]"
    else
        echo -e "$green[OK]""$cyan""[$test_name]"
    fi

    rm -rf .mygit/

}

init_test "basic init"
