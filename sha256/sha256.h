#ifndef __MYSHA256__
#define __MYSHA256__

#include <cstddef>

// 信息摘要是长度为32个字节的数组，一个字节位8位
#define SHA256_MESSAGE_SIZE 32

// 定义SHA256哈希操作的内容信息结构体
typedef struct
{
    unsigned char buffer[64];             // 512位消息区块
    unsigned int buffer_idx;              // 消息区块的索引
    unsigned long long processed_msg_len; // 已处理的原始消息的长度
    unsigned int hash[8];                 // 该消息目前的哈希值
} sha256_context;

void sha256_starts(sha256_context *ctx);                                          // 初始化内容信息结构体
void sha256_update(sha256_context *ctx, const unsigned char input[], size_t len); // 迭代计算消息的哈希值(信息摘要)
void sha256_result(sha256_context *ctx, unsigned char output[]);                  // 对信息进行补码处理，并将结果存储在output内返回

#endif