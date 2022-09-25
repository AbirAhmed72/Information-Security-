#include<bits/stdc++.h>
using namespace std;

#define SHR(x, n) (x >> n)
#define ROTR(x, n) ((x >> n) ^ (x << (64 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define sum512_zero(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define sum512_one(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define sigma512_zero(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7))
#define sigma512_one(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6))

uint64_t key[80], Hash[8];

string msg;
uint64_t blockNo;
uint64_t msgLength;
uint64_t position = 0;
unsigned char *totalChar = new unsigned char[blockNo * 128];

void initializeHash(){

    ifstream ifile;
    ifile.open("initialHASH.txt");

    if(!ifile){
        cout << "unable to open initialHASH.txt";
        exit(0);
    }

    for(int i = 0; i < 8; i++){
        ifile >> hex >> Hash[i];
    }
        
    ifile.close();
}

void initializeKey(){

    ifstream ifile;
    ifile.open("k_values.txt");

    if(!ifile){
        cout << "unable to open k_values.txt" << endl;
        exit(1);
    }

    for(int i = 0; i < 80; i++){
        ifile >> hex >> key[i];
    }

    ifile.close();
}

void printHash(){

    cout << "\nHASH VALUE: ";
    cout << hex << (uint64_t)Hash[0] << (uint64_t)Hash[1]
         << (uint64_t)Hash[2] << (uint64_t)Hash[3]
         << (uint64_t)Hash[4] << (uint64_t)Hash[5]
         << (uint64_t)Hash[6] << (uint64_t)Hash[7] << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
}

void padding(){
    
    for(uint64_t i = 0; i < msgLength; i++){
        totalChar[i] = msg[i];
    }        

    // cout << "\nline 70: " << totalChar << endl;

    totalChar[msgLength] = 1 << 7;
    
    // cout << "\nline 74: " << totalChar << endl;


    for(uint64_t i = msgLength + 1; i < blockNo * 128; i++){
        totalChar[i] = 0;
    
    }
    
    // cout << "\nline 82: " << totalChar << endl;


    // for(int i=0; i<blockNo * 128; i++){
    //     cout << hex << totalChar[i] << " ";
    // }
    
    uint64_t totalBins = msgLength * 8;
    // cout << "\nline 90: " << totalBins << " " << msgLength << endl;


    for(uint64_t i = (blockNo * 128 - 1); i > (blockNo * 128 - 17); i--){
        
        totalChar[i] = totalBins;
        // cout << "\nline 95: " << totalChar << " ";
        totalBins = totalBins >> 8;
        // cout << "\nline 97: " << totalBins << endl;
    }
    
    // cout << "\nline 101: " << totalChar << endl;
    
}

void roundCalculation(){
    
    for(uint64_t k = 1; k <= blockNo; k++){

        uint64_t W[80], t1, t2, a, b, c, d, e, f, g, h;

        // round calculation for individual blocks
        for(int j = 0; j < 16; j++){

            W[j] = totalChar[position];
            // cout << "\nline 115: " << W << endl;

            for (int i = 1; i < 8; i++){
                W[j] = W[j] << 8 | totalChar[position + i];
            }
            // cout << "\nline 120: " << W << endl;
            position += 8;
        }

        // find values of W[t]
        for(int j = 16; j < 80; j++){
            W[j] = (sigma512_one(W[j - 2]) + W[j - 7] + sigma512_zero(W[j - 15]) + W[j - 16]);
            // cout << "\nline 127: " << W[j] << endl;
        }
        // cout << "\nline 129: " << W << endl;
            
        a = Hash[0];
        e = Hash[4];
        b = Hash[1];
        f = Hash[5];
        c = Hash[2];
        g = Hash[6];
        d = Hash[3];
        h = Hash[7];

        // inside round calculation
        for(int i = 0; i < 80; i++){

            t1 = (h + CH(e, f, g) + sum512_one(e) + W[i] + key[i]);
            t2 = (sum512_zero(a) + MAJ(a, b, c));
            h = g;
            g = f;
            f = e;
            e = (d + t1);
            d = c;
            c = b;
            b = a;
            a = (t1 + t2);
        }

        Hash[0] = (a + Hash[0]);
        Hash[1] = (b + Hash[1]);
        Hash[2] = (c + Hash[2]);
        Hash[3] = (d + Hash[3]);
        Hash[4] = (e + Hash[4]);
        Hash[5] = (f + Hash[5]);
        Hash[6] = (g + Hash[6]);
        Hash[7] = (h + Hash[7]);
    }

    printHash();
}

int main(){

    // //string msg;
    string temp;
    ifstream ifile;
    ifile.open("data.txt");
    getline(ifile, msg);

    while(getline(ifile, temp)){
        msg += "\n" + temp;
    }        
        
    cout << msg << endl;

    // // uint64_t blockNo;
    // //uint64_t msgLength;
    // //uint64_t position = 0;

    msgLength = msg.length();

    if (msgLength == 0){
        exit(0);
    }      

    blockNo = ceil((msgLength + 17 + 0.0) / 128);
    printf("BLOCKS: %ld\n", blockNo);

    initializeKey();
    initializeHash();

    // padding
    
    padding();
    
    // round calculation
    
    roundCalculation();

    
    
}