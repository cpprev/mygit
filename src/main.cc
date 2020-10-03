#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <iostream>

int main()
{
    char string[] = "compute sha2ompute sha2ompute sha2ompute sha2ompute sha2";
    unsigned char digest[SHA_DIGEST_LENGTH];

    SHA1((unsigned char*) &string, strlen(string), (unsigned char*) &digest);

    std::string temp;
    int i;
    for (i = 0; i < 20; i++) {
        temp += (char) digest[i];
        printf("%02x", digest[i]);
    }
    printf("\n");
    std::cout << temp << "\n";

    return 0;
}