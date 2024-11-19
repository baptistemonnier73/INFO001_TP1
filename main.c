typedef unsigned char byte;

#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void hash_SHA1(const char *input, byte *hash);
int computeN(const char *alphabet, int size);

int main(int argc, char **argv)
{
    FILE *file;

    if (argc >= 2)
    {
        file = fopen(argv[1], "r");
        char fileLine[200];
        char hashFunction[200];
        char alphabet[200];
        int size;
        char command[200];
        char args[10][200];
        for (int nbOfLine = 0; fgets(fileLine, 200, file); nbOfLine++)
        {
            fileLine[strlen(fileLine) - 1] = fileLine[strlen(fileLine) - 1] == '\n' ? '\0' : fileLine[strlen(fileLine) - 1];

            switch (nbOfLine)
            {
                case 0:
                    strcpy(hashFunction, fileLine);
                    break;

                case 1:
                    strcpy(alphabet, fileLine);
                    break;

                case 2:
                    size = atoi(fileLine);
                    break;

                case 3:
                    strcpy(command, fileLine);
                    break;

                default:
                    strcpy(args[nbOfLine - 4], fileLine);
                    break;
            }
        }

        printf(">>> running test file '%s'\n", argv[1]);
        printf("hash function: '%s'\n", hashFunction);
        printf("alphabet = '%s'\n", alphabet);
        printf("size = %d\n", size);


        if(strcmp(command, "config") == 0) {
            printf("N = %d\n", computeN(alphabet, size));
        }
        if(strcmp(command, "hash") == 0) {
            printf("N = %d\n\n", computeN(alphabet, size));

            byte hash[SHA_DIGEST_LENGTH];
            char hexaHash[2 * SHA_DIGEST_LENGTH];

            hash_SHA1(args[0], hash);

            for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
                sprintf(hexaHash + i * 2, "%02x", hash[i]);
            }

            printf("%s (%s)\n", hexaHash, args[0]);
        } 
    }
    else
    {
    }

    return 0;
}

int computeN(const char *alphabet, int size)
{
    return pow(strlen(alphabet), size);
}

void hash_SHA1(const char *input, byte *hash)
{
    SHA1((unsigned char *)input, strlen(input), hash);
}
