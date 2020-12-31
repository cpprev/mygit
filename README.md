# mygit Project [started 3/10/2020] [WIP]

---

:pushpin: The Project was implemented in C++ and uses Bash for the testsuite scripts.

:pushpin: This project's aim is to learn how git works internally.

---
### How to build & Usage

    mkdir build && cd build/ && cmake .. && make -j && ./mygit *

---

| Libraries Required        | Download |
| -----------------         | -------- |
| <zlib.h>                  | sudo apt install zlib1g-dev |
| <openssl/sha.h>           | sudo apt install libssl-dev |

| Commands implemented      | Usage                 |
|---------------------------|-----------------------|
| init                      | mygit init            |
| add                       | mygit add PATH [-f]   |
| commit                    | mygit commit -m MSG   |
| log                       | mygit log             |
| branch                    | mygit branch [NAME] [-d NAME] |
| checkout                  | mygit checkout [NAME/HASH] |
| write-tree                | mygit write-tree      |
| status                    | mygit status          |
| diff                      | mygit diff            |
| ls-files                  | mygit ls-files        |
| cat-file                  | mygit cat-file HASH [-t] [-p] [-r] |
| hash-object               | mygit hash-object FILE [--write] [--type type] |
| config                    | mygit config [--add user.id "hello w"] [--get user.id ] [--unset user.id] [--local/--global]|
| merge                     | mygit merge BRANCH    |
