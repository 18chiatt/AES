#include <stdio.h>
#include<stdlib.h>

#define word unsigned int
#define START_STATE { {0x19,0xa0,0x9a,0xe9},  {0x3d,0xf4,0xc6,0xf8},  {0xe3,0xe2,0x8d,0x48}, {0xbe,0x2b,0x2a,0x08}};
#define NK  8
#define NR (NK + 6)
#define NB 4

// CHANGE NK ABOVE TO CHANGE KEY LENTH

void populateState(unsigned char toReturn[4][4] , char * input);
void populateKey(char * keySting, unsigned char * key);
unsigned char ffAdd(unsigned char a, unsigned char b);
unsigned char xtime(unsigned char val);
void assertEquals(word a, word b);
unsigned char ffMultiply(unsigned char a, unsigned char b);
unsigned char sBox(const unsigned char a);
word subWord(word input);
void subBytes(unsigned char state[4][4]);
void assertStateEquals(unsigned char actual[4][4], unsigned char expected[4][4]);
void shiftRows(unsigned char state[4][4]);
void printState(unsigned char state[4][4]);
void mixColumns(unsigned char state[4][4]);
void doColumn(unsigned char state[4][4], int col);
word rotWord(word key);
word *keyExpansion(unsigned char key[4*NK], word output[4 * (NR + 1)]);
void assertExpansionEquals(word * actual, word * expected, int length);
void addRoundKey(unsigned char state[4][4], word roundKey[4]);
void tests();
void cipher(unsigned char state[4][4], word w[4 * (NR + 1 )]);
void printRawState();
unsigned char unSbox(const unsigned char a);
void InvCipher(unsigned char state[4][4], word w[4 * (NR + 1)]);
void invSubBytes(unsigned char state[4][4]);
void invShiftRows(unsigned char state[4][4]);
void testCaseOne();
void testCaseTwo();
void testCaseThree();

const unsigned char MIX_MATRIX[4][4] = {{2,3,1,1},{1,2,3,1},{1,1,2,3},{3,1,1,2}};
const unsigned char UN_MIX_MATRIX[4][4] = {{0x0e, 0x0b, 0x0d, 0x09}, {0x09,0x0e,0x0b,0x0d}, {0x0d,0x09,0x0e,0x0b}, {0x0b,0x0d,0x09,0x0e}};
const unsigned char Sbox[16][16] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 } ,
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 } ,
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 } ,
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 } ,
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 } ,
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf } ,
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 } ,
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 } ,
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 } ,
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb } ,
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 } ,
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 } ,
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a } ,
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e } ,
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf } ,
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
    };

const unsigned char unSboxMat[16][16] = {
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb } ,
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb } ,
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e } ,
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 } ,
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 } ,
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 } ,
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 } ,
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b } ,
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 } ,
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e } ,
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b } ,
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 } ,
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f } ,
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef } ,
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 } ,
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
    };


const word Rcon[52] = { 0x00000000, 
           0x01000000, 0x02000000, 0x04000000, 0x08000000, 
           0x10000000, 0x20000000, 0x40000000, 0x80000000, 
           0x1B000000, 0x36000000, 0x6C000000, 0xD8000000, 
           0xAB000000, 0x4D000000, 0x9A000000, 0x2F000000, 
           0x5E000000, 0xBC000000, 0x63000000, 0xC6000000, 
           0x97000000, 0x35000000, 0x6A000000, 0xD4000000, 
           0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000, 
           0xC5000000, 0x91000000, 0x39000000, 0x72000000, 
           0xE4000000, 0xD3000000, 0xBD000000, 0x61000000, 
           0xC2000000, 0x9F000000, 0x25000000, 0x4A000000, 
           0x94000000, 0x33000000, 0x66000000, 0xCC000000, 
           0x83000000, 0x1D000000, 0x3A000000, 0x74000000, 
           0xE8000000, 0xCB000000, 0x8D000000};

int main() {

    char * message = "00112233445566778899aabbccddeeff";
	unsigned char  state[4][4];

    char * keyStr = "000102030405060708090a0b0c0d0e0f";

    unsigned char key[32];

    populateKey(keyStr, key);
    

    // printState(key);
    word w[128];
    keyExpansion(key,w);

    populateState(state, message);


    cipher(state, w);
    // InvCipher(state, w);
    // CHANGE WHICH LINE IS COMMENTED TO SWAP FROM CIPHER TO INV CIPHER


    printf("Converted Text: \n");
    printRawState(state);
    testCaseOne();
    testCaseTwo();
    testCaseThree();

}

void testCaseOne(){
    printf("\nBegin test case 1\n");

    if(NK != 4 ){
        printf("NK not set correcrtly! for TC 1, aborting");
        return;
    }
    char * inMessage = "00112233445566778899aabbccddeeff";
	unsigned char  state[4][4];

    char * keyStr = "000102030405060708090a0b0c0d0e0f";

    unsigned char key[32];

    populateKey(keyStr, key);    

    // printState(key);
    word w[128];
    keyExpansion(key,w);
    populateState(state, inMessage);
    cipher(state, w);
    printf("Encrypted:\n");
    printRawState(state);

    printf("\nNow decrypt!\n");
    InvCipher(state, w);
    printRawState(state);
    printf("\n");
}

void testCaseTwo(){
    printf("\nBegin test case 2\n");

    if(NK != 6){
        printf("NK not set correcrtly! for TC 2, aborting");
        return;
    }
    char * inMessage = "00112233445566778899aabbccddeeff";
	unsigned char  state[4][4];

    char * keyStr = "000102030405060708090a0b0c0d0e0f1011121314151617";

    unsigned char key[32];

    populateKey(keyStr, key);    

    // printState(key);
    word w[128];
    keyExpansion(key,w);
    populateState(state, inMessage);
    cipher(state, w);
    printf("Encrypted:\n");
    printRawState(state);

    printf("\nNow decrypt!\n");
    InvCipher(state, w);
    printRawState(state);
    printf("\n");
}

void testCaseThree(){
    printf("\nBegin test case 3\n");

    if(NK != 8 ){
        printf("NK not set correcrtly! for TC 3, aborting");
        return;
    }
    char * inMessage = "00112233445566778899aabbccddeeff";
	unsigned char  state[4][4];

    char * keyStr = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f";

    unsigned char key[32];

    populateKey(keyStr, key);    

    // printState(key);
    word w[128];
    keyExpansion(key,w);
    populateState(state, inMessage);
    cipher(state, w);
    printf("Encrypted:\n");
    printRawState(state);

    printf("\nNow decrypt!\n");
    InvCipher(state, w);
    printRawState(state);
    printf("\n");
}

void assertEquals(word a, word b){
    if(a != b){
        printf("TEST FAILED!!! %x   !==   %x\n", a, b);
        return;
    }
    printf("%x == %x\n", a,b);
}

void assertStateEquals(unsigned char actual[4][4], unsigned char expected[4][4]){
    for(int i =0; i< 4; i++){
        for(int j =0; j< NB; j++){
            if(actual[i][j] != expected[i][j]){
                printf("TEST FAILED!!!!%x != %x", actual[i][j], expected[i][j]);
            } else {
                printf("%x == %x,  ", actual[i][j], expected[i][j]);
            }
        }
        printf("\n");
    }
}

void assertExpansionEquals(word * actual, word * expected, int length){
    for(int i =0; i< length; i++){
        if(i % 8 == 0){
            printf("\n");
        }
        if(actual[i] != expected[i]){
            printf("FAIL, not equal!, %x != %x !!!   \n", actual[i], expected[i]);
            break;
        } 
        else {
            printf("%x == %x", actual[i],expected[i]);
        }

    }
    printf("\nPASS\n");
    return;
}

void printState(unsigned char state[4][4] ){
    for(int i = 0; i< 4; i++){
        for(int j =0; j< 4; j++){
            printf("%x, ", state[i][j]);
        }
        printf("\n");
    }
}

void printRawState(unsigned char state[4][4]){
    for(int i =0; i< 4; i++){
        for(int j =0; j< 4; j++){
            printf("%02x", state[j][i]);
        }
        fflush(stdout);
    }
}

unsigned char xtime(unsigned char val) {
    unsigned char shouldMod = (val & (1 << 7) ) == ((unsigned char) 128);
    unsigned char shifted = val << 1;
    if(shouldMod){
        shifted = shifted ^ 0x1b;
    }
    return shifted;
}

unsigned char ffMultiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char curr = a;
    while(b){
        if(b & 0x1){
            result ^= curr;
        }
        curr = xtime(curr);
        b = b >> 1;
    }
    return result;
}


unsigned char ffAdd(unsigned char a, unsigned char b){
    return a ^ b;
}

void populateState(unsigned char toReturn[4][4] , char * input){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < NB; j++) {
            char strBuf[3] = {input[0],input[1],0x00};
            unsigned int buffer;
			sscanf(strBuf, "%x", &buffer );
            toReturn[j][i] = (unsigned char) buffer;
            input = input + 2;
		}
	}
}

void populateKey(char * keyString, unsigned char * key){
    for(int i =0; i< NK * 4; i++){
        char chars[3] = {keyString[i*2], keyString[i * 2 + 1], 0x00};
        int intBuffer = 0;
        sscanf(chars, "%x", &intBuffer);
        key[i] = (unsigned char)intBuffer;
    }
}


word subWord(word input){
    word output = 0;
    word mask = 0xff;
    for(int i =0; i< 4; i++){
        word currByte = (input & mask) >> (i * 8);
        word replacement = sBox(currByte);
        word inPosition = replacement << (8 * i);
        output |= inPosition;
        mask = mask << 8;
    }
    return output;
}

void subBytes(unsigned char state[4][4]){
    for(int i =0; i< 4; i++){
        for(int j =0; j< NB; j++){
            state[i][j] = sBox(state[i][j]);
        }
    }
}

void invSubBytes(unsigned char state[4][4]){
    for(int i =0; i< 4; i++){
        for(int j =0; j< NB; j++){
            state[i][j] = unSbox(state[i][j]);
        }
    }
}

unsigned char sBox(const unsigned char a){
    return Sbox[a >> 4][a & 0xf];
}

unsigned char unSbox(const unsigned char a){
    return unSboxMat[a >> 4][a & 0xf];
}

void shiftRows(unsigned char state[4][4]){
    unsigned char temp[4][4] = START_STATE;
    for(int i =0; i< 4; i++){
        temp[0][i] = state[0][i];
        temp[1][i] = state[1][(i + 1) % 4];
        temp[2][i] = state[2][(i + 2) % 4];
        temp[3][i] = state[3][(i + 3) % 4];
    }
    for(int i =0; i< 4; i++){
        for(int j =0; j< 4; j++){
            state[i][j] = temp[i][j];
        }
    }
}

void invShiftRows(unsigned char state[4][4]){
    unsigned char temp[4][4] = {
        {state[0][0],state[0][1],state[0][2],state[0][3]},
        {state[1][3],state[1][0],state[1][1],state[1][2]},
        {state[2][2],state[2][3],state[2][0],state[2][1]},
        {state[3][1],state[3][2],state[3][3],state[3][0]}
    };
    
    for(int i =0; i< 4; i++){
        for(int j =0; j< 4; j++){
            state[i][j] = temp[i][j];
        }
    }
}

void mixColumns(unsigned char state[4][4]) {
    for(int i =0; i< 4; i++){
            unsigned char temps[4] = {0,0,0,0};
        for(int j =0; j< 4; j++){
            unsigned char result = 0;
            for(int k =0; k< 4; k++){
                result = result ^ ffMultiply(state[k][i], MIX_MATRIX[j][k]);
            }
            temps[j] = result;
        }
        for(int j =0; j< 4; j++){
            state[j][i] = temps[j];
        }
    }
}

void unMixColumns(unsigned char state[4][4]) {
    for(int i =0; i< 4; i++){
            unsigned char temps[4] = {0,0,0,0};
        for(int j =0; j< 4; j++){
            unsigned char result = 0;
            for(int k =0; k< 4; k++){
                result = result ^ ffMultiply(state[k][i], UN_MIX_MATRIX[j][k]);
            }
            temps[j] = result;
        }
        for(int j =0; j< 4; j++){
            state[j][i] = temps[j];
        }
    }
}

word rotWord(word key){
    unsigned char * iterator = (unsigned char *)&key;
    unsigned char a0 = iterator[0];
    unsigned char a1 = iterator[1];
    unsigned char a2 = iterator[2];
    unsigned char a3 = iterator[3];
    unsigned char toReturn[4] = {a3,a0,a1,a2};
    // "memory safety"    
    return *(word *)toReturn;
}

word *keyExpansion(unsigned char key[4*NK], word output[4 * (NR + 1)]) { // output is 4 word per round + 1
    word temp = 0;

    for(int i = 0; i< NK; i++){
        unsigned char curr[4] = {key[4 * i + 3], key[4 * i + 2], key[4 * i + 1], key[4 * i] };
        output[i] = *(word*) curr;
    }
    
    for(int i = NK; i< (4 * (NR + 1)); i++){
        temp = output[i-1];
        if (i % NK == 0){
            temp = subWord(rotWord(temp)) ^ Rcon[i/NK];
        } else if(NK > 6 && (i % NK) == 4){
            temp = subWord(temp);
        }
        output[i] = output[i-NK] ^ temp;
    }
    return output;
}

void addRoundKey(unsigned char state[4][4], word roundKey[4]){
    for(int i =0; i< 4; i++){
        word toAssign = roundKey[i];
        unsigned char * iterator = (unsigned char *)&toAssign;
        unsigned char bytes[4] = {iterator[3],iterator[2],iterator[1],iterator[0]};
        for(int j =0; j< 4; j++){
            state[j][i] = state[j][i] ^ bytes[j];
        }
    }
}

void tests() {
    assertEquals( ffAdd(0x57,0x83), 0xd4);
    assertEquals(xtime(0x57), 0xae);
    assertEquals(xtime(0xae), 0x47);
    assertEquals(xtime(0x47), 0x8e);
    assertEquals(xtime(0x8e), 0x07);

    assertEquals(ffMultiply(0x57,0x13),0xfe);
    assertEquals(ffMultiply(0x27,0x70),0xc9);

    assertEquals(sBox(0x12),0xc9);

    assertEquals(subWord(0x00102030), 0x63cab704);
    assertEquals(subWord(0x40506070) ,0x0953d051);
    assertEquals(subWord(0x8090a0b0), 0xcd60e0e7);
    assertEquals(subWord(0xc0d0e0f0) , 0xba70e18c);

    unsigned char testState[4][4] =  START_STATE;

    unsigned char expectedForSub[4][4] = { {0xd4,0xe0,0xb8,0x1e},
                         {0x27,0xbf,0xb4,0x41},
                         {0x11,0x98,0x5d,0x52},
                         {0xae,0xf1,0xe5,0x30}};
    subBytes(testState);
    assertStateEquals(testState, expectedForSub);

    unsigned char expectedForShift[4][4] =  { {0xd4, 0xe0, 0xb8, 0x1e},
                         {0xbf, 0xb4, 0x41, 0x27},
                         {0x5d, 0x52, 0x11, 0x98},
                         {0x30, 0xae, 0xf1, 0xe5}};

    

    shiftRows(testState);
    assertStateEquals(testState,expectedForShift);

    unsigned char expectedForMix[4][4] =    { {0x04, 0xe0, 0x48, 0x28},
                         {0x66, 0xcb, 0xf8, 0x06},
                         {0x81, 0x19, 0xd3, 0x26},
                         {0xe5, 0x9a, 0x7a, 0x4c}};
    

    mixColumns(testState);
    assertStateEquals(testState,expectedForMix);
    assertEquals(rotWord(0x09cf4f3c), 0xcf4f3c09);
    assertEquals(rotWord(0x2a6c7605) , 0x6c76052a);

    unsigned char key[16] =       { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    word expanded[44] = { 0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c,
                            0xa0fafe17, 0x88542cb1, 0x23a33939, 0x2a6c7605,
                            0xf2c295f2, 0x7a96b943, 0x5935807a, 0x7359f67f,
                            0x3d80477d, 0x4716fe3e, 0x1e237e44, 0x6d7a883b,
                            0xef44a541, 0xa8525b7f, 0xb671253b, 0xdb0bad00,
                            0xd4d1c6f8, 0x7c839d87, 0xcaf2b8bc, 0x11f915bc,
                            0x6d88a37a, 0x110b3efd, 0xdbf98641, 0xca0093fd,
                            0x4e54f70e, 0x5f5fc9f3, 0x84a64fb2, 0x4ea6dc4f,
                            0xead27321, 0xb58dbad2, 0x312bf560, 0x7f8d292f,
                            0xac7766f3, 0x19fadc21, 0x28d12941, 0x575c006e,
                            0xd014f9a8, 0xc9ee2589, 0xe13f0cc8, 0xb6630ca6 };

    word w[44];

    keyExpansion(key,w);

    assertExpansionEquals(w,expanded,44);

    addRoundKey(testState,w + 4);

    unsigned char round[4][4] =   { {0xa4, 0x68, 0x6b, 0x02},
                          {0x9c, 0x9f, 0x5b, 0x6a},
                          {0x7f, 0x35, 0xea, 0x50},
                          {0xf2, 0x2b, 0x43, 0x49}};

    assertStateEquals(testState,round);

    unsigned char in[16]  = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                    0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };

    unsigned char result[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
                       0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };

    unsigned char inFixed[4][4];
    unsigned char resultFixed[4][4];
    for(int i =0; i< 4; i++){
        for(int j =0; j< 4; j++){
            inFixed[i][j] = in[i + 4 * j];
            resultFixed[i][j] = result[i + 4 * j];
        }
    }
    printf("Printing state\n");
    printState(inFixed);
    printf("Done Printing state\n");

    cipher(inFixed,w);

    assertStateEquals(inFixed,resultFixed);
}

void cipher(unsigned char state[4][4], word w[4 * (NR + 1 )]) {
    addRoundKey(state, w);

    for(int i = 1; i< NR; i++){

        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, w + (i * 4));
    }

    subBytes(state);
    shiftRows(state);

    addRoundKey(state, w + (4 * NR));
}

void InvCipher(unsigned char state[4][4], word w[4 * (NR + 1)]) {
    addRoundKey(state, w + NR * NB);;

    for(int i = NR - 1; i > 0; i--){
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, w + i * NB);
        unMixColumns(state);
    }

    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, w);
}



