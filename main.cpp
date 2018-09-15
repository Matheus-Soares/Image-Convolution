#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

unsigned char* readImage(const char* filename, unsigned int &W, unsigned int &H, char* type);
vector<vector<int>> readKernel(const char* filename);
unsigned char* filter(const vector<vector<int>> &kernel, unsigned char* data, unsigned int W, unsigned int H);
bool saveImage(const char* filename, unsigned char *data, unsigned int W, unsigned int H);

int main(int argc, char* argv[]){

    if (argc != 4){
        cout << "Input arguments error. Input should be <kernel.dat executableFile.pgm It>\n" << endl;
        return 1;
    }

    unsigned int H, W;  // Size of the image
    H = W = 0;
    char type[2];       // Type of the image

    unsigned char *data = readImage(argv[2], W, H, type);           // Read the image
    int it = atoi(argv[3]);                                         // Number of iterattions

    if(data == nullptr){
        printf("Error reading the image!\n");
        return -1;
    }

    unsigned char *filtered = data;         // Image filtered

    vector<vector<int>> kernel;             // Kernel
    kernel = readKernel(argv[1]);

    for (int i = 0; i < it; i++){
        filtered = filter(kernel, filtered, W, H);      // Filtering the image
    }

    saveImage("NewImage.pgm", filtered, W, H);      // Save the final image (Remember to modify the extension

    free(filtered);
    free(data);

    return 0;
}


unsigned char* readImage(const char* filename, unsigned int &W, unsigned int &H, char* type){

    FILE* fp = fopen(filename, "r");

    if(!fp) return nullptr;

    unsigned int maxVal=0;
    unsigned char* data;

    fscanf(fp, "%c", &type[0]);
    fscanf(fp, "%c", &type[1]);

    fscanf(fp, "%d %d %d", &W, &H, &maxVal);

    data = (unsigned char*) calloc((W*H), sizeof(unsigned int));

    for(int i = 0; i < (W*H); i++)
        fscanf(fp, "%d ", &data[i]);

    fclose(fp);
    return data;

}

vector<vector<int>> readKernel(const char* filename){

    vector<vector<int>> kernel;
    FILE* fp = fopen(filename, "r");

    int m, n;

    fscanf(fp, "%d %d", &m, &n);

    kernel.resize(m);

    for(int i = 0 ; i < m ; ++i)
        kernel[i].resize(n);

    for(int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp, "%d ", &kernel[i][j]);
        }
    }

    fclose(fp);
    return kernel;

}

unsigned char* filter(const vector<vector<int>> &kernel, unsigned char* data, unsigned int W, unsigned int H){

    unsigned char* filtered;
    filtered = (unsigned char*) calloc((W*H), sizeof(unsigned int));
    int sum = 0;

    for(int i = 0; i < kernel.size(); i++){
        for(int j = 0; j < kernel[i].size(); j++){
            sum += kernel[i][j];         // Compute the sum of the numbers of the kernel
        }
    }
    unsigned int index;
    for (int i= 0; i < H; i++){
        for (int j = 0; j < W; j++){

            int pk = 0;
            for (int k = 0; k < kernel.size(); k++){
                for (int l = 0; l < kernel[k].size(); l++){

                    int x = (j - ((((kernel.size() - 1) / 2)) - l));
                    int y = (i - ((((kernel.size() - 1) / 2)) - k));

                    if (x < 0)
                        x = (j + l);

                    if (y < 0)
                        y = (i + k);

                    if (x > (W - 1)){
                        x = (j - (l - 2));
                    }

                    if (y > (H - 1)){
                        y = (i - (k - 2));
                    }

                    index = (y * W) + x;

                    pk += data[index] * kernel[k][l];
                }
            }

            if (sum != 0) pk = pk / sum;
            if (pk > 255) pk = 255;

            filtered[i * W + j] = static_cast<unsigned char>(abs(pk));
        }
    }

    return filtered;
}

bool saveImage(const char* filename, unsigned char *data, unsigned int W, unsigned int H){

    FILE* fp = fopen(filename, "w");

    if(!fp) return false;

    fprintf(fp, "P2\n%d %d\n255\n", W, H);

    for(int i = 0; i< (W*H); i++)
        fprintf(fp, "%d ", data[i]);

    fclose(fp);

    return data;
}