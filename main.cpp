#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <algorithm>
#include <numeric>
#include <format>
#include <string>

#if defined(__AVX__)
#include <immintrin.h>
#endif

constexpr size_t misalign_bytes = 16;
constexpr size_t very_misalign_bytes = 11;

void scalar_add(const float* a, const float* b, float* result, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        result[i] = a[i] + b[i];
    }
}

#if defined(__AVX__)
void SIMD_add_aligned(const float* a, const float* b, float* result, size_t N) {
    for (size_t i = 0; i < N; i += 8) {
        __m256 va = _mm256_load_ps(a + i);
        __m256 vb = _mm256_load_ps(b + i);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_store_ps(result + i, vc);
    }
}

void SIMD_add_unaligned(const float* a, const float* b, float* result, size_t N) {
    for (size_t i = 0; i < N; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(result + i, vc);
    }
}
#endif

int generateRandomNumber(int min = 1, int max = 100) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

template<typename F, typename... Args>
std::chrono::milliseconds time(F func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
}

void test(const float* data, const int N, float* a, float* b, float* result, int alignment)
{
    std::copy(data, data + N, a);
    std::copy(data + N, data + 2 * N, b);
    
    time(scalar_add, a, b, result, N);
    auto t = time(scalar_add, a, b, result, N);
    std::cout << std::format("Scalar addition ({:>2}b aligned): Time: {:>6}, Sum: {}\n", alignment, t, std::accumulate(result, result + N, 0.0f));
#if defined(__AVX__)
    time(alignment == 32 ? SIMD_add_aligned : SIMD_add_unaligned, a, b, result, N);
    t = time(alignment == 32 ? SIMD_add_aligned : SIMD_add_unaligned, a, b, result, N);
    std::cout << std::format("SIMD   addition ({:>2}b aligned): Time: {:>6}, Sum: {}\n", alignment, t, std::accumulate(result, result + N, 0.0f));
#endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <vector_size>\n";
        return 1;
    }

    size_t N = std::stoi(argv[1]);
    if (N % 8 != 0) {
        std::cerr << "Vector size must be a multiple of 8.\n";
        return 1;
    }

    std::cout << "Running with vector size: " << N << std::endl;

    // Allocate raw memory with padding for alignment + misalignment
    size_t total_bytes = N * sizeof(float) + 64;
    float* data = new float[2 * N];

    std::generate(data, data + 2 * N, [](){ return generateRandomNumber() - 50; });

    float* raw_a =      new float[total_bytes / sizeof(float)];
    float* raw_b =      new float[total_bytes / sizeof(float)];
    float* raw_result = new float[total_bytes / sizeof(float)];

    float* base_a = reinterpret_cast<float*>((reinterpret_cast<uintptr_t>(raw_a) + 31) & ~uintptr_t(31));
    float* base_b = reinterpret_cast<float*>((reinterpret_cast<uintptr_t>(raw_b) + 31) & ~uintptr_t(31));
    float* base_result = reinterpret_cast<float*>((reinterpret_cast<uintptr_t>(raw_result) + 31) & ~uintptr_t(31));

    float* misaligned_a = reinterpret_cast<float*>(reinterpret_cast<char*>(base_a) + misalign_bytes);
    float* misaligned_b = reinterpret_cast<float*>(reinterpret_cast<char*>(base_b) + misalign_bytes);
    float* misaligned_result = reinterpret_cast<float*>(reinterpret_cast<char*>(base_result) + misalign_bytes);

    float* very_misalign_a = reinterpret_cast<float*>(reinterpret_cast<char*>(base_a) + very_misalign_bytes);
    float* very_misalign_b = reinterpret_cast<float*>(reinterpret_cast<char*>(base_b) + very_misalign_bytes);
    float* very_misalign_result = reinterpret_cast<float*>(reinterpret_cast<char*>(base_result) + very_misalign_bytes);

    test(data, N, base_a, base_b, base_result, 32);
    test(data, N, misaligned_a, misaligned_b, misaligned_result, misalign_bytes);
    test(data, N, very_misalign_a, very_misalign_b, very_misalign_result, very_misalign_bytes);

    delete [] raw_a;
    delete [] raw_b;
    delete [] raw_result;
    delete [] data;
    return 0;
}
