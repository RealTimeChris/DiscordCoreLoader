
#include <stdint.h>

int main() {
    // Check AVX1, AVX2, and AVX512 support using CPUID instruction
    uint32_t cpuid_features[4];
    uint32_t avx_bit = 0x10000000;        // Bit 28 of ECX for EAX=0x1
    uint32_t avx2_bit = 0x00000020;       // Bit 5 of EBX for EAX=0x7
    uint32_t avx512f_bit = 0x00010000;    // Bit 16 of EBX for EAX=0x7

    // Execute CPUID instruction with EAX=0x1
    __asm__ __volatile__ (
        cpuid :
        =a (cpuid_features[0]),
        =b (cpuid_features[1]),
        =c (cpuid_features[2]),
        =d (cpuid_features[3]) :
        a (0x00000001),
        c (0x00000000)
    );

    // Check if AVX1 is supported
    int avx_supported = (cpuid_features[2] & avx_bit) != 0;

    // Check if AVX2 is supported
    int avx2_supported = 0;
    if (avx_supported) {
        // Execute CPUID instruction with EAX=0x7 and ECX=0
        __asm__ __volatile__ (
            cpuid :
            =a (cpuid_features[0]),
            =b (cpuid_features[1]),
            =c (cpuid_features[2]),
            =d (cpuid_features[3]) :
            a (0x00000007),
            c (0x00000000)
        );

        avx2_supported = (cpuid_features[1] & avx2_bit) != 0;
    }

    // Check if AVX512 is supported
    int avx512_supported = 0;
    if (avx_supported) {
        // Execute CPUID instruction with EAX=0x7 and ECX=0
        __asm__ __volatile__ (
            cpuid :
            =a (cpuid_features[0]),
            =b (cpuid_features[1]),
            =c (cpuid_features[2]),
            =d (cpuid_features[3]) :
            a (0x00000007),
            c (0x00000000)
        );

        avx512_supported = (cpuid_features[1] & avx512f_bit) != 0;
    }

    // Determine the AVX type
    int avx_type = -1;
    if (avx512_supported) {
        avx_type = 0;   // AVX512
    } else if (avx2_supported) {
        avx_type = 1;   // AVX2
    } else if (avx_supported) {
        avx_type = 2;   // AVX
    }

    // Return the result
    return avx_type;
}
