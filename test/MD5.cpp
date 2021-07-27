#include <iostream>
#include "math.h"
#include <cstring>
#include <vector>

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

using namespace std;

int s[64] = {     7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22
                , 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20
                , 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23
                , 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
unsigned int T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


class MD5 {
public:
    MD5(string s);
    void Padding();
    void Hash_md5(int N);
    string GetHexStr(unsigned int num_str);
    void Output();
    ~MD5();
private:
    unsigned int X[16];
    unsigned int A, B, C, D;
    vector<unsigned char> input;
    long long len;
    unsigned char n[8];
};
MD5::MD5(string s) {

    A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D =  0x10325476;

    for(int j =0; j<s.size(); j++)
        input.push_back(s[j]);

    len = input.size()*8;

    memset(X, 0, sizeof(X));

    Padding();
};
MD5::~MD5(){
}
void MD5::Padding(){

    unsigned char p0 = 0x80, p1 = 0x0;
    input.push_back(p0);

    while (input.size() %64 != 56)
        input.push_back(p1);

    unsigned char LENGTH[8];
    memcpy(LENGTH, &len, sizeof(LENGTH));

    for(int j=0; j<8; j++)
        input.push_back(LENGTH[j]);


    int N = input.size()/64;

    this->Hash_md5(N);
};

void MD5::Hash_md5(int N) {
    vector<char> temp;
    for (int i = 0; i < N; i++){

        temp.clear();
        temp.insert(temp.end(), input.begin()+64*i, input.begin()+64*(i+1));

        memcpy(X, &temp.at(0), sizeof(X));


        unsigned int AA = A, BB = B, CC = C, DD = D;

        for (int l = 0; l < 64; l++) {
            unsigned int f;
            int g;
            if(l<16) {
                f = F((BB),(CC),(DD));
                g = l;
            }
            if(l>=16 && l<32)   {
                f = G((BB),(CC),(DD));
                g = (5*l+1)%16;
            }
            if(l>=32 && l<48)   {
                f = H((BB),(CC),(DD));
                g = (3*l+5)%16;
            }
            if(l>=48 && l<64)   {
                f = I(BB,CC,DD);
                g = (7*l)%16;
            }

            f = f + AA + X[g] + (unsigned int)T[l];
            AA = DD;
            DD = CC;
            CC = BB;
            BB = BB + ROTATE_LEFT(f, s[l]);

        }
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
};

string MD5::GetHexStr(unsigned int num_str) {
    std::string hexstr = "";
    char szch[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    unsigned char *tmptr = (unsigned char *)&num_str;
    int len = sizeof(num_str);
    // Little-endian byte order, reverse printing
    for (int i = 0; i < len; i++){
        unsigned char ch = tmptr[i] & 0xF0;
        ch = ch >> 4;
        hexstr.append(1, szch[ch]);
        ch = tmptr[i] & 0x0F;
        hexstr.append(1, szch[ch]);
    }
    return hexstr;
}

void MD5::Output(){
    unsigned int buf[4] = {A,B,C,D};
    string result;

    for(int i =0; i<4; i++)
        result.append(GetHexStr(buf[i]));
    cout<<result<<endl;

};

