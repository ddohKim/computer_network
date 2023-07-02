#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include "bitset"

FILE *input_fp;
FILE *output_fp;
int dataword_size;
char *generator;
// char generator[100];
// char inputData[1000000000];
std::string inputData;
std::string binaryData;
int sizeOfGenerator;
int main(int argc, char **argv)
{
    if (argc != 5)
    {
        fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
        return 0;
    }

    input_fp = fopen(argv[1], "rb");
    if (input_fp == NULL)
    {
        printf("input file open error.\n");
        return 0;
    }
    output_fp = fopen(argv[2], "wb");
    if (output_fp == NULL)
    {
        printf("input file open error.\n");
        return 0;
    }

    dataword_size = atoi(argv[4]);
    if (dataword_size != 4 && dataword_size != 8)
    {
        printf("dataword size must be 4 or 8.\n");
        return 0;
    }
    // printf("%s",argv[2]);
    // printf("%s",argv[2]);
    generator = argv[3];
    // char genereator[100]="1101";
    // strcpy(generator,"0101");
    // const char *generator="0101";

    // printf("%c",generator[0]);
    sizeOfGenerator = strlen(generator);
    // printf("%d",sizeOfGenerator);
    char c;
    int i = 0;
    while (EOF != fscanf(input_fp, "%c", &c))
    {
        // inputData[i] = c;
        inputData += c;
        i++;
    }
    fclose(input_fp);

    for (int i = 0; i < inputData.length(); i++)
    {
        int ascii = inputData[i];
        std::string temp = std::bitset<8>(ascii).to_string();
        binaryData += temp;
    }
    // std::cout << binaryData << std::endl;

    std::string add_zero;
    int tempSize = sizeOfGenerator;
    while (tempSize > 1)
    {
        add_zero += "0";
        tempSize--;
    }
    std::string codewords;
    // printf("\n%s",generator);
    if (generator[0] == '0')
    {
        generator = strtok(generator, "0");
    }
    // printf("%s",generator);
    int tempGeneratorSize = strlen(generator);
    for (int i = 0; i < binaryData.length(); i += dataword_size)
    {

        std::string dividend;
        // printf("%s",generator);

        for (int j = i; j < i + dataword_size; j++)
        {
            dividend += binaryData[j];
        }
        dividend += add_zero;
        std::string remainder = dividend;
        tempSize = remainder.length() - sizeOfGenerator;

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
                {
                    break;
                }
            }
        }
        remainder = remainder.substr(dataword_size);
        std::string codeword = dividend.substr(0, dataword_size) + remainder;

        codewords += codeword;
    }

    int temp = 8 - codewords.length() % 8;
    if (8 - codewords.length() % 8)
    {
        int j = 0;
        while (j < temp)
        {
            codewords.insert(0, 1, '0');
            j++;
        }
    }

    std::bitset<8> binary(temp);
    int decimalValue = binary.to_ulong();
    char ascii = static_cast<char>(decimalValue);
    fprintf(output_fp, "%c", ascii);

    for (int i = 0; i < codewords.length(); i += 8)
    {
        std::string temp = codewords.substr(i, 8).c_str();
        std::bitset<8> binary(temp);
        int decimalValue = binary.to_ulong();
        char ascii = static_cast<char>(decimalValue);

        fprintf(output_fp, "%c", ascii);
    }

    fclose(output_fp);

    return 0;
}