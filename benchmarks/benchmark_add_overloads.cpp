#include "../src/polynomial.h"
#include <benchmark/benchmark.h>
#include <vector>

using namespace poly;

class _Polynomial : public Polynomial<int>
{
  public:
    _Polynomial(std::vector<int> coeff_) : m_coeff(coeff_) {}

    friend _Polynomial operator+(_Polynomial &&lhs, const _Polynomial &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend _Polynomial operator+(const _Polynomial &lhs, _Polynomial &&rhs)
    {
        rhs += lhs;
        return rhs;
    }

    const auto &data() const noexcept { return m_coeff; };

  private:
    std::vector<int> m_coeff;
};

const auto vec_large = std::vector<int>(50000, 10);
const auto vec_small = std::vector<int>(1000, -1);

static void BM_add_lvalues(benchmark::State &state)
{
    for (auto _ : state) {
        const auto large = Polynomial<int>{vec_large};
        const auto small = Polynomial<int>{vec_small};

        auto result = large + small;
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_add_lvalues);

static void BM_add_big_rvalue_small_lvalue(benchmark::State &state)
{
    for (auto _ : state) {
        auto large = _Polynomial{vec_large};
        const auto small = _Polynomial{vec_small};

        auto result = std::move(large) + small;
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_add_big_rvalue_small_lvalue);

static void BM_add_big_lvalue_small_rvalue(benchmark::State &state)
{
    for (auto _ : state) {
        const auto large = _Polynomial{vec_large};
        auto small = _Polynomial{vec_small};

        auto result = large + std::move(small);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_add_big_lvalue_small_rvalue);

BENCHMARK_MAIN();
