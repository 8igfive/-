#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "sha256.h"

int main()
{
    FILE *input;
    int input_len = 0;
    FILE *output;

    unsigned char *msg;
    unsigned char digset[SHA256_MESSAGE_SIZE];
    sha256_context ctx;

    // 读取输入消息
    input = fopen("./input.txt", "r");
    fseek(input, 0, SEEK_END);
    input_len = ftell(input);
    msg = (unsigned char *)malloc((input_len + 1) * sizeof(char));
    rewind(input);
    input_len = fread(msg, 1, input_len, input);
    msg[input_len] = '\0';

    // 运行SHA256算法
    sha256_starts(&ctx);
    sha256_update(&ctx, msg, input_len);
    sha256_result(&ctx, digset);

    // 输出原始消息和SHA256算法加密后的消息摘要
    printf("message : %s\n", msg);
    printf("digset  : 0x");
    output = fopen("./output.txt", "w");
    for (int i = 0; i < SHA256_MESSAGE_SIZE; i++)
    {
        printf("%x", digset[i]);
        fprintf(output, "%x", digset[i]);
    }
    printf("\n");
    return 0;
}