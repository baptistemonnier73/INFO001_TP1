typedef unsigned char byte;

#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void hash_SHA1(const char *input, byte *hash);
int computeN(const char *alphabet, int size);
void i2c(const char *alphabet, int size, long input, char *output);
uint64_t h2i(const unsigned char *y, uint64_t t, uint64_t N);


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

        int N = computeN(alphabet, size);

        printf("N = %d\n\n", N);

        if (strcmp(command, "config") == 0)
        {
        }
        if (strcmp(command, "hash") == 0)
        {

            byte hash[SHA_DIGEST_LENGTH];
            char hexaHash[2 * SHA_DIGEST_LENGTH];

            hash_SHA1(args[0], hash);

            for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
            {
                sprintf(hexaHash + i * 2, "%02x", hash[i]);
            }

            printf("%s (%s)\n", hexaHash, args[0]);
        }
        if (strcmp(command, "i2c") == 0)
        {
            int input = atoi(args[0]);
            char output[size + 1];

            i2c(alphabet, size, input, output);

            printf("i2c(%d) = \"%s\"\n", input, output);
        }
        if(strcmp(command, "h2i") == 0) {
            byte hash[SHA_DIGEST_LENGTH];
            unsigned char hexaHash[2 * SHA_DIGEST_LENGTH];

            hash_SHA1(args[0], hash);

            for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
            {
                sprintf(hexaHash + i * 2, "%02x", hash[i]);
            }

            printf("hash(\"%s\") = %s\n", args[0], hexaHash);
            printf("h2i(hash(\"%s\"), %llu) = %llu\n", args[0], atoi(args[1]), h2i(hash, atoi(args[1]), N));

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

void i2c(const char *alphabet, int size, long input, char *output)
{
    // Indexes de chaque case du tableau de retour par rapport au tableau de l'alphabet (par exemple pour ABVM, ce sera {0, 1, 21, 12})
    int i[size];

    // Initialisation de la valeur à retourner (par exemple AAAA)
    for (int j = 0; j < size; j++)
    {
        i[j] = 0;
        output[j] = alphabet[0];
    }

    // Ne pas oublier le caractère nul à la fin
    output[size] = '\0';

    // Conteur pour le tableau i défini plus haut ainsi que pour le tableau de retour (on se place dans un premier temps sur la dernière valeur de ces tableaux)
    int j = size - 1;

    // On itère sur la valeur passée en paramètres (par exemple de 0 à 1233)
    for (int k = 0; k < input; k++)
    {
        // Si on est pas encore au bout de l'alphabet
        if (i[j] < strlen(alphabet) - 1)
        {
            i[j]++;
            output[j] = alphabet[i[j]];
        }
        // Sinon on décrémente j...
        else
        {
            // ...jusqu'à ce qu'on trouve une valeur n'étant pas la dernière de l'alphabet ou que j devienne nul
            while(i[j] >= strlen(alphabet) - 1 && j >= 0) {
                j--;
            }
            // on incrémente j
            if (i[j] < strlen(alphabet) - 1)
            {
                i[j]++;
                output[j] = alphabet[i[j]];
            }
            // puis on réinitialise les valeurs suivantes à la position 0 de l'alphabet
            while(j < size - 1) {
                j++;
                i[j] = 0;
                output[j] = alphabet[0];
            }
        }
    }
}

uint64_t h2i(const unsigned char *y, uint64_t t, uint64_t N) {
    uint64_t subY = *(uint64_t *)y;

    return (subY + t) % N;
}

