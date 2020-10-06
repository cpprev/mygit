#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <iostream>

int main()
{
    char string[] = "compute sha2ompute sha2ompute sha2ompute sha2ompute sha2";
    unsigned char digest[SHA_DIGEST_LENGTH];

    SHA1((unsigned char*) &string, strlen(string), (unsigned char*) &digest);

    char res[SHA_DIGEST_LENGTH * 2 + 1];

    int i;
    for (i = 0; i < 20; i++) {
        printf("%02x", digest[i]);
        sprintf(&res[i*2], "%02x", (unsigned int)digest[i]);
    }
    printf("\n");
    std::cout << res << "\n";

    return 0;
}