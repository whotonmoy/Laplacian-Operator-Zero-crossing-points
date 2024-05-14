#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

vector<unsigned char> readRawImage(const char* filename, int width, int height);
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height);
void applyLaplacianOperator(const vector<unsigned char>& inputImage, vector<unsigned char>& zeroCrossingMap, int width, int height);

int main() 
{
    // Define image parameters
    // Change these parameters according to the image being tested
    const int width = 280;
    const int height = 280;

    // Load the input image
    vector<unsigned char> inputImage = readRawImage("building.raw", width, height);

    // Initialize the zero-crossing points map
    vector<unsigned char> zeroCrossingMap(width * height, 0);

    // Apply the Laplacian operator to compute the zero-crossing points
    applyLaplacianOperator(inputImage, zeroCrossingMap, width, height);

    // Save the zero-crossing points map
    saveImage("ZeroCrossingMap_output.raw", zeroCrossingMap, width, height);

    return 0;
}

// Function to read a raw image file
vector<unsigned char> readRawImage(const char* filename, int width, int height) {
    vector<unsigned char> image(width * height);
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(image.data()), width * height);
        file.close();
    }
    return image;
}

// Function to save the grayscale image
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(image.data()), width * height);
        file.close();
    }
}

// Apply the Laplacian operator to compute the zero-crossing points
void applyLaplacianOperator(const vector<unsigned char>& inputImage, vector<unsigned char>& zeroCrossingMap, int width, int height) {
    // Laplacian 3x3 kernel
    int laplacianKernel[9] = {
        0, 1, 0,
        1, -4, 1,
        0, 1, 0
    };

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sum = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int pixel = inputImage[(y + j) * width + (x + i)];
                    int kernelValue = laplacianKernel[(j + 1) * 3 + (i + 1)];
                    sum += pixel * kernelValue;
                }
            }

            // Detect zero-crossing points
            if (sum > 0) {
                zeroCrossingMap[y * width + x] = 255;
            }
            else {
                zeroCrossingMap[y * width + x] = 0;
            }
        }
    }
}