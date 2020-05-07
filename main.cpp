#include <iostream>

// Compile with:
// c++ -std=c++11 -O3 -Wall -Wextra -Weffc++ -Wshadow -pedantic -I.. tests.cpp -o tests

#define RLE_IMPLEMENTATION
#include "rle.hpp"

#define HUFFMAN_IMPLEMENTATION
#include "huffman.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <chrono>

// Test sample data:

// A binary dump of "lenna.tga", a sample image. Size is 256x256 pixels, RGBA uncompressed.
#include "lenna-tga.hpp"

// 512 randomly shuffled byte values:
#include "random-512.hpp"

// A few test strings:
static const std::uint8_t str0[] = "Hello almighty world!";
static const std::uint8_t str1[] = "Covid 19 got you down?";
static const std::uint8_t str2[] = " Dr. Fontenot, Let's code some algorithms...";
static const std::uint8_t str3[] = "\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11";

// Run Length Encoding (RLE) tests:

static void Test_RLE_EncodeDecode(const std::uint8_t * sampleData, const int sampleSize){
    std::vector<std::uint8_t> compressedBuffer(sampleSize * 4, 0);
    std::vector<std::uint8_t> uncompressedBuffer(sampleSize, 0);

    // Compress:
    const int compressedSize = rle::easyEncode(sampleData, sampleSize, compressedBuffer.data(), compressedBuffer.size());

    std::cout << "RLE compressed size bytes   = " << compressedSize << "\n";
    std::cout << "RLE uncompressed size bytes = " << sampleSize << "\n";

    // Restore:
    const int uncompressedSize = rle::easyDecode(compressedBuffer.data(), compressedSize, uncompressedBuffer.data(), uncompressedBuffer.size());

    // Validate:
    bool successful = true;
    if (uncompressedSize != sampleSize){
        std::cerr << "RLE COMPRESSION ERROR! Size mismatch!\n";
        successful = false;
    }
    if (std::memcmp(uncompressedBuffer.data(), sampleData, sampleSize) != 0){
        std::cerr << "RLE COMPRESSION ERROR! Data corrupted!\n";
        successful = false;
    }

    if (successful){
        std::cout << "RLE compression successful!\n";
    }
    // No additional memory is allocated by the RLE encoder/decoder.
    // You have to provide big buffers.
}

static void Test_RLE(){
    std::cout << "> Testing random512...\n";
    Test_RLE_EncodeDecode(random512, sizeof(random512));

    std::cout << "> Testing strings...\n";
    Test_RLE_EncodeDecode(str0, sizeof(str0));
    Test_RLE_EncodeDecode(str1, sizeof(str1));
    Test_RLE_EncodeDecode(str2, sizeof(str2));
    Test_RLE_EncodeDecode(str3, sizeof(str3));

    std::cout << "> Testing lenna.tga...\n";
    Test_RLE_EncodeDecode(lennaTgaData, sizeof(lennaTgaData));
}

// Huffman encoding/decoding tests:

static void Test_Huffman_EncodeDecode(const std::uint8_t * sampleData, const int sampleSize){
    int compressedSizeBytes = 0;
    int compressedSizeBits  = 0;
    std::uint8_t * compressedData = nullptr;
    std::vector<std::uint8_t> uncompressedBuffer(sampleSize, 0);

    // Compress:
    huffman::easyEncode(sampleData, sampleSize, &compressedData, &compressedSizeBytes, &compressedSizeBits);

    std::cout << "Huffman compressed size bytes   = " << compressedSizeBytes << "\n";
    std::cout << "Huffman uncompressed size bytes = " << sampleSize << "\n";

    // Restore:
    const int uncompressedSize = huffman::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits, uncompressedBuffer.data(), uncompressedBuffer.size());

    // Validate:
    bool successful = true;
    if (uncompressedSize != sampleSize){
        std::cerr << "HUFFMAN COMPRESSION ERROR! Size mismatch!\n";
        successful = false;
    }
    if (std::memcmp(uncompressedBuffer.data(), sampleData, sampleSize) != 0){
        std::cerr << "HUFFMAN COMPRESSION ERROR! Data corrupted!\n";
        successful = false;
    }

    if (successful){
        std::cout << "Huffman compression successful!\n";
    }

    // easyEncode() uses HUFFMAN_MALLOC (std::malloc).
    HUFFMAN_MFREE(compressedData);
}

static void Test_Huffman(){
    std::cout << "> Testing random512...\n";
    Test_Huffman_EncodeDecode(random512, sizeof(random512));

    std::cout << "> Testing strings...\n";
    Test_Huffman_EncodeDecode(str0, sizeof(str0));
    Test_Huffman_EncodeDecode(str1, sizeof(str1));
    Test_Huffman_EncodeDecode(str2, sizeof(str2));
    Test_Huffman_EncodeDecode(str3, sizeof(str3));

    std::cout << "> Testing lenna.tga...\n";
    Test_Huffman_EncodeDecode(lennaTgaData, sizeof(lennaTgaData));
}

// Unit tests driver:

#define TEST(func)                                                                                         \
    do {                                                                                                   \
        std::cout << ">>> Testing " << #func << " encoding/decoding.\n";                                   \
        const auto startTime = std::chrono::system_clock::now();                                           \
        Test_##func();                                                                                     \
        const auto endTime = std::chrono::system_clock::now();                                             \
        std::chrono::duration<double> elapsedSeconds = endTime - startTime;                                \
        std::cout << ">>> " << #func << " tests completed in " << elapsedSeconds.count() << " seconds.\n"; \
        std::cout << std::endl;                                                                            \
    } while (0,0)


int main()
{
    std::cout << "\nRunning unit tests for the compression algorithms...\n\n";
    TEST(RLE);
    TEST(Huffman);
}