
#include <gmp.h>
#include "receiver.h"
#include "rsa.h"
#include <ctime>
int main()
{
    //get socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //connect sender
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //ipv4
    serv_addr.sin_addr.s_addr = inet_addr("192.168.31.128");  //ip address
    serv_addr.sin_port = htons(8000);  //port
    int result=connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(result==-1){
        printf("errno for connection is %d\n",errno);
    }else{
        printf("connection built!\n");
    }
    //receive public key and key length
    unsigned char buffer[100000];
    unsigned char *b_f=buffer;
    int32_t pk_len=0;
    recvPKeyAndLen(b_f,&pk_len,sock);
	//printf("ReceivedPublicKey:%s\n",buffer);
    struct public_key pub[1];
    gmp_sscanf((const char*)buffer,"%ZX,%ZX",pub->modulus,pub->exponent);
    gmp_printf("Public_key:\n\nmodulus:%ZX\n\nexponent:%ZX\n\n",pub->modulus,pub->exponent);

    // //print public key information for comparison
    // for (int i=0; i<ntohl(pk_len); i++)
    // {
    //     printf("0x%02x, ", *(buffer+i));
    // }
    // printf("\npklen from server:%d\n",ntohl(pk_len));
    //generate public key
    // unsigned char *PKey=buffer;
    // RSA *EncryptRsa = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, ntohl(pk_len));
    // if(EncryptRsa==NULL){
    //     printf("EncryptRsa error!\n");
    // }
    // RSA_print_fp(stdout,EncryptRsa,0);
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt,time(NULL));
    mpz_t seedNum,outSeedNum;
    mpz_init(seedNum);
	mpz_init(outSeedNum);
    mpz_urandomb(seedNum,grt,128);
    gmp_printf("OriginSeed:%ZX\n\n",seedNum);
    RSA_encrypt(outSeedNum,seedNum,pub);
    gmp_printf("EncryptedSeed:%ZX\n\n",outSeedNum);

    // //encrypt process
    unsigned char seed[1280],outseed[1280];
	memset(seed,0,1280);memset(outseed,0,1280);
    // unsigned char ranstr[SEED_LEN];
    gmp_sprintf((char*)seed,"%ZX\0",seedNum);
    gmp_sprintf((char*)outseed,"%ZX\0",outSeedNum);
	//gmp_printf("seed:\n%s\noutseed:%s\n",seed,outseed);
	//int seedLen = strlen((const char*)seed);
	//int outSeedLen = strlen((const char*)outseed);
	//printf("seedLen:%d\noutseedLen:%d\n",seedLen,outSeedLen);
    // memset(ranstr,0,128);
    // // genSeed(ranstr);
    // gmp_sprintf((char*)ranstr,"%ZX\0",seedNum);
    // strcpy((char*)seed,(const char*)ranstr);

    // if(RSA_public_encrypt(SEED_LEN, (const unsigned char*)seed, outseed, EncryptRsa, RSA_NO_PADDING)==-1)
    // {
    //     printf("encrypt failed!\n");
    //     char szErrMsg[1024] = {0};
    //     printf("error for encrypt is %s\n",ERR_error_string(ERR_get_error(),szErrMsg));
    // }
    // else{
    //     printf("The seed is %s\n\n\n\n\n",seed);
    //     //printf("The seed after encryption is %s\n\n\n\n\n",outseed);
    // }
    //send encrypted seed
    sendSeed(outseed,1280,sock);
    unsigned char data_after_encrypt[16];
    unsigned char data_after_decrypt[16];
    unsigned char aesSeed[32];
    strncpy((char*)aesSeed,(const char*)seed,32);
    AES_KEY AESDecryptKey;
    AES_set_decrypt_key(aesSeed, 256, &AESDecryptKey);
    while(1){
        //receive data
        printf("Wainting For File...\n");
        memset(data_after_encrypt,0,sizeof(data_after_encrypt));
        recvFile(data_after_encrypt,data_after_decrypt,&AESDecryptKey,sock);
    }
    //RSA_free(EncryptRsa);
    close(sock);
    return 0;
}
