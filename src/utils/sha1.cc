#include <cstring>
#include <openssl/sha.h>

#include "utils/sha1.hh"

namespace utils
{
    std::string SHA1_Wrapper (const std::string& input)
    {
        const char *string = input.c_str();
        unsigned char digest[SHA_DIGEST_LENGTH];

        SHA1((unsigned char*) string, strlen(string), (unsigned char*) &digest);

        char res[SHA_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        {
            sprintf(&res[i*2], "%02x", (unsigned int) digest[i]);
        }
        return res;
    }
}