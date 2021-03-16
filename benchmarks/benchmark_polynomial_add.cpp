#include <algorithm>
#include <benchmark/benchmark.h>
#include <vector>

const std::vector<int> v1(100, 10);
const std::vector<int> v2(800, -1);

static void BM_add_transform(benchmark::State &state)
{
    for (auto _ : state) {
        const auto &[small, large] =
            std::minmax(v1, v2, [](const auto &a, const auto &b) {
                return a.size() < b.size();
            });
        auto result = large;
        auto it = result.begin();
        std::advance(it, large.size() - small.size());
        std::transform(small.cbegin(), small.cend(), it, it, std::plus<>{});
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_add_transform);

static void BM_add_loop(benchmark::State &state)
{
    for (auto _ : state) {
        const auto &[small, large] =
            std::minmax(v1, v2, [](const auto &a, const auto &b) {
                return a.size() < b.size();
            });
        std::vector<int> result(large.size());
        const auto small_sz = small.size();
        const auto large_sz = large.size();
        const auto diff = large_sz - small_sz;
        for (std::size_t i = 0; i < diff; ++i)
            result[i] = large[i];
        for (std::size_t i = 0; i < small_sz; ++i)
            result[i + diff] = small[i] + large[i + diff];
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_add_loop);

BENCHMARK_MAIN();
