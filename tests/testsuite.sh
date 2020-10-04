#!/bin/bash

# Testsuite script

# Colors
green='\033[1;32m'
blue='\033[0;34m'
purple='\033[0;35m'
cyan='\033[1;36m'
gray='\033[0;37m'
red='\033[1;31m'
reset='\033[0m'

# Verify that we are in the right folder (tests/)
dir="$(basename $PWD)"
if [ $dir != "tests" ]; then
    echo -e "$cyan""You need to be in the $red'Spider/tests/'$cyan" \
        "directory to launch the testsuite. $reset"
    exit 1
fi

# Logging
echo -e "$cyan""Current directory:" $PWD

# Counters initialization
total_tests=0
total_passed=0

is_argument ()
{
    arg=$1
    nb=$2
    if [ $nb -eq 0 ]; then
        return 0
    fi
    temp=$3
    for elm in $arg; do
        if [ "$elm" == "$temp" ]; then
            return 0
        fi
    done
    return 1
}

# init tests
if is_argument "$*" "$#" "init"; then
    . subscripts/init_test.sh
fi
# add tests
if is_argument "$*" "$#" "add"; then
    . subscripts/add_test.sh
fi
# status tests
if is_argument "$*" "$#" "status"; then
    . subscripts/status_test.sh
fi
# ls-files tests
if is_argument "$*" "$#" "ls-files"; then
    . subscripts/ls-files_test.sh
fi

# Logging
echo -e "$green\n[$tests_passed/$total_tests tests passed!]"

# Reset print color
echo -ne $reset

exit 0