# mygit Project [started 3/10/2020]

---

:pushpin: The Project was made in C++ for the implementation and Bash for the testsuite.

:pushpin: This project's aim is to learn how git works internally.

---

| Libraries Required        | 
| -----------------         |
| <zlib.h>                  | 
| <openssl/sha.h>           |

| Commands implemented      | Usage                 |
|---------------------------|-----------------------|
| init                      | mygit init            |
| add                       | mygit add PATH [-f]   |
| commit                    | mygit commit -m MSG   |
| write-tree                | mygit write-tree      |
| status                    | mygit status          |
| ls-files                  | mygit ls-files        |
| cat-file                  | mygit cat-file HASH [-t] [-p] [-r] |
| hash-object               | mygit hash-object FILE [--write] [--type blob/tree/commit]|