#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include "bitset"

FILE *input_fp;
FILE *output_fp;
FILE *result_fp;
int dataword_size;
char *generator;
std::string padding;
std::string binaryData;
int sizeOfGenerator;
int error = 0;
int numofCodeword = 0;
int main(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, "usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
        return 0;
    }

    input_fp = fopen(argv[1], "rb");
    if (input_fp == NULL)
    {
        printf("input file open error.\n");
        return 1;
    }
    output_fp = fopen(argv[2], "wb");
    if (output_fp == NULL)
    {
        printf("output file open error.\n");
        return 1;
    }
    result_fp = fopen(argv[3], "wb");
    if (result_fp == NULL)
    {
        printf("result file open error.\n");
        return 1;
    }

    dataword_size = atoi(argv[5]);
    if (dataword_size != 4 && dataword_size != 8)
    {
        printf("dataword size must be 4 or 8.\n");
        return 1;
    }

    generator = argv[4];
    sizeOfGenerator = strlen(generator);
    char c;
    int i = 0;

    while (EOF != fscanf(input_fp, "%c", &c))
    {
        int ch = c;
        if (i < 1)
        {

            padding = std::bitset<8>(ch).to_string();
        }
        else
        {
            // if (c != ' ')
            binaryData += std::bitset<8>(ch).to_string();
        }
        i++;
    }
    std::bitset<8> binary(padding);
    // std::cout << padding << std::endl;
    int decimalValue = binary.to_ulong();
    binaryData = binaryData.substr(decimalValue);
    // std::cout << binaryData << std::endl;

    fclose(input_fp);

    std::string codewords;

    if (generator[0] == '0')
    {
        generator = strtok(generator, "0");
    }
    // printf("%s",generator);
    int tempGeneratorSize = strlen(generator);

    for (int i = 0; i < binaryData.length(); i += dataword_size + sizeOfGenerator - 1)
    {
        std::string dividend;

        for (int j = i; j < i + dataword_size + sizeOfGenerator - 1; j++)
        {
            dividend += binaryData[j];
        }
        std::string remainder = dividend;
        int tempSize = remainder.length() - sizeOfGenerator;

        int j = 0;
        while (j <= tempSize)
        {
            for (int k = 0; k < tempGeneratorSize; k++)
            {
                if (remainder[j + k] != generator[k])
                {
                    remainder[j + k] = '1';
                }
                else
                {
                    remainder[j + k] = '0';
                }
            }
            while (j < remainder.length())
            {
                if (remainder[j] == '0')
                {
                    j++;
                }
                else
                    break;
            }
        }

        remainder = remainder.substr(dataword_size);
        std::bitset<8> binary(remainder);
        // std::cout << remainder << std::endl;

        int decimalValue = binary.to_ulong();
        if (decimalValue != 0)
        {
            error++;
        }
        numofCodeword++;
        std::string codeword = dividend.substr(0, dataword_size);

        codewords += codeword;
    }

    for (int i = 0; i < codewords.length(); i += 8)
    {
        std::string temp;
        int j = i;
        while (j < i + 8)
        {
            temp += codewords[j];
            j++;
        }
        std::bitset<8> binary(temp);
        int decimalValue = binary.to_ulong();
        char ascii = static_cast<char>(decimalValue);
        fprintf(output_fp, "%c", ascii);
    }

    fclose(output_fp);
    fprintf(result_fp, "%d %d", numofCodeword, error);
    fclose(result_fp);

    return 0;
}