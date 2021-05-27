#include <cstdlib>
#include <memory.h>
#include "sha256.h"

#define RIGHTROTATE(a, b) (((a) >> (b)) | ((a) << (32 - (b)))) // 定义循环右移函数

// 逻辑函数定义
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SIGMA0(x) (RIGHTROTATE(x, 2) ^ RIGHTROTATE(x, 13) ^ RIGHTROTATE(x, 22))
#define SIGMA1(x) (RIGHTROTATE(x, 6) ^ RIGHTROTATE(x, 11) ^ RIGHTROTATE(x, 25))
#define SIG0(x) (RIGHTROTATE(x, 7) ^ RIGHTROTATE(x, 18) ^ ((x) >> 3))
#define SIG1(x) (RIGHTROTATE(x, 17) ^ RIGHTROTATE(x, 19) ^ ((x) >> 10))

// 常量初始化(64个常数序列)
static const unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// 进行单次哈希运算
void sha256_process(sha256_context *ctx, const unsigned char buffer[])
{
    unsigned int a, b, c, d, e, f, g, h, t1, t2, w[64];
    int i, j;
    // 计算扩展消息块
    for (i = 0, j = 0; i < 16; ++i, j += 4)
    {
        w[i] = (buffer[j] << 24) | (buffer[j + 1] << 16) | (buffer[j + 2] << 8) | (buffer[j + 3]);
    }
    for (; i < 64; ++i)
    {
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    }
    // 用前一个中间哈希值对a, b, c, d, e, f, g, h进行初始化
    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];
    f = ctx->hash[5];
    g = ctx->hash[6];
    h = ctx->hash[7];
    // 应用SHA256压缩函数来更新a, b, c, d, e, f, g, h
    for (int i = 0; i < 64; ++i)
    {
        t1 = h + SIGMA1(e) + CH(e, f, g) + k[i] + w[i];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    // 计算当前的中间哈希值
    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
    ctx->hash[5] += f;
    ctx->hash[6] += g;
    ctx->hash[7] += h;
}

void sha256_starts(sha256_context *ctx)
{
    ctx->buffer_idx = 0;
    ctx->processed_msg_len = 0;
    // 定义8个初始哈希值
    ctx->hash[0] = 0x6a09e667;
    ctx->hash[1] = 0xbb67ae85;
    ctx->hash[2] = 0x3c6ef372;
    ctx->hash[3] = 0xa54ff53a;
    ctx->hash[4] = 0x510e527f;
    ctx->hash[5] = 0x9b05688c;
    ctx->hash[6] = 0x1f83d9ab;
    ctx->hash[7] = 0x5be0cd19;
}

void sha256_update(sha256_context *ctx, const unsigned char input[], size_t len)
{
    // 摘要计算主循环
    for (int i = 0; i < len; ++i)
    {
        // 获取当前的信息快
        ctx->buffer[ctx->buffer_idx] = input[i];
        ctx->buffer_idx++;
        if (ctx->buffer_idx == 64)
        {
            sha256_process(ctx, ctx->buffer);
            ctx->processed_msg_len += 512;
            ctx->buffer_idx = 0;
        }
    }
}

void sha256_result(sha256_context *ctx, unsigned char output[])
{
    int i = ctx->buffer_idx;
    // 消息预处理
    if (8 * ctx->buffer_idx < 448)
    {
        // 如果当前消息位数小于448位,直接将消息填充至448位
        ctx->buffer[i++] = 0x80; // 在消息末尾补上一位1
        // 将消息填充到448位
        while (8 * i < 448)
        {
            ctx->buffer[i++] = 0x00;
        }
    }
    else
    {
        // 如果当前消息位数大于等于448位,先将消息填充至512位,将这个消息块进行哈希映射,再重新填448字节
        ctx->buffer[i++] = 0x80; // 在消息末尾补上一位1
        // 将消息填充到512位
        while (8 * i < 512)
        {
            ctx->buffer[i++] = 0x00;
        }
        sha256_process(ctx, ctx->buffer); // 对刚填充的消息块进行哈希映射
        memset(ctx->buffer, 0, 56);       // 再填充448位消息
    }

    // 在消息末尾添加64位消息长度信息
    ctx->processed_msg_len += ctx->buffer_idx * 8;
    ctx->buffer[63] = ctx->processed_msg_len;
    ctx->buffer[62] = ctx->processed_msg_len >> 8;
    ctx->buffer[61] = ctx->processed_msg_len >> 16;
    ctx->buffer[60] = ctx->processed_msg_len >> 24;
    ctx->buffer[59] = ctx->processed_msg_len >> 32;
    ctx->buffer[58] = ctx->processed_msg_len >> 40;
    ctx->buffer[57] = ctx->processed_msg_len >> 48;
    ctx->buffer[56] = ctx->processed_msg_len >> 56;
    sha256_process(ctx, ctx->buffer); // 对最后一个消息块进行哈希处理

    // 小端转大端输出
    for (i = 0; i < 4; ++i)
    {
        output[i] = (ctx->hash[0] >> (24 - i * 8)) & 0xff;
        output[i + 4] = (ctx->hash[1] >> (24 - i * 8)) & 0xff;
        output[i + 8] = (ctx->hash[2] >> (24 - i * 8)) & 0xff;
        output[i + 12] = (ctx->hash[3] >> (24 - i * 8)) & 0xff;
        output[i + 16] = (ctx->hash[4] >> (24 - i * 8)) & 0xff;
        output[i + 20] = (ctx->hash[5] >> (24 - i * 8)) & 0xff;
        output[i + 24] = (ctx->hash[6] >> (24 - i * 8)) & 0xff;
        output[i + 28] = (ctx->hash[7] >> (24 - i * 8)) & 0xff;
    }
}