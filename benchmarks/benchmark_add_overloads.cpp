#include "../src/polynomial.h"
#include <benchmark/benchmark.h>
#include <vector>

using namespace poly;
auto get_p1() { return Polynomial<int>{std::vector<int>(50000, 10)}; }
auto get_p2() { return Polynomial<int>{std::vector<int>(1000, -1)}; }

static void BM_add_lvalue(benchmark::State &state)
{
    for (auto _ : state) {
        const auto p1 = get_p1();
        const auto p2 = get_p2();
        auto result = p1 + p2;
        benchmark::DoNotOptimize(result.coefficients());
    }
}
BENCHMARK(BM_add_lvalue);

static void BM_add_rvalue_first(benchmark::State &state)
{
    for (auto _ : state) {
        auto p1 = get_p1();
        const auto p2 = get_p2();
        auto result = std::move(p1) + p2;
        benchmark::DoNotOptimize(result.coefficients());
    }
}
BENCHMARK(BM_add_rvalue_first);

static void BM_add_rvalue_second(benchmark::State &state)
{
    for (auto _ : state) {
        const auto p1 = get_p1();
        auto p2 = get_p2();
        auto result = p1 + std::move(p2);
        benchmark::DoNotOptimize(result.coefficients());
    }
}
BENCHMARK(BM_add_rvalue_second);

BENCHMARK_MAIN();
