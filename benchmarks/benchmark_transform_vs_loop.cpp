#include "../src/polynomial.h"
#include <benchmark/benchmark.h>
#include <vector>

using namespace poly;

class _Polynomial : public Polynomial<int>
{
  public:
    _Polynomial(std::vector<int> coeff_) : m_coeff(coeff_) {}

    _Polynomial &operator+=(const _Polynomial &other)
    {
        const auto &[small, large] = std::minmax(
            m_coeff, other.m_coeff,
            [](const auto &a, const auto &b) { return a.size() < b.size(); });

        auto result = large;
        std::transform(small.cbegin(), small.cend(), result.cbegin(),
                       result.begin(), std::plus<>{});
        m_coeff = std::move(result);
        return *this;
    }

    _Polynomial &operator-=(const _Polynomial &other)
    {
        auto result(other.m_coeff);
        std::transform(result.cbegin(), result.cend(), result.begin(),
                       std::negate<>{});
        *this += _Polynomial{std::move(result)};
        return *this;
    }

    const auto &data() const noexcept { return m_coeff; };

  private:
    std::vector<int> m_coeff;
};

const auto v1 = std::vector<int>(500, -1);
const auto v2 = std::vector<int>(100000, 10);

static void BM_add_loop(benchmark::State &state)
{
    auto p1 = Polynomial<int>{v1};
    const auto p2 = Polynomial<int>{v2};
    for (auto _ : state) {
        p1 += p2;
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_add_loop);

static void BM_add_transform(benchmark::State &state)
{
    auto p1 = _Polynomial{v1};
    const auto p2 = _Polynomial{v2};
    for (auto _ : state) {
        p1 += p2;
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_add_transform);

static void BM_minus_loop(benchmark::State &state)
{
    auto p1 = Polynomial<int>{v1};
    const auto p2 = Polynomial<int>{v2};
    for (auto _ : state) {
        p1 -= p2;
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_minus_loop);

static void BM_minus_transform(benchmark::State &state)
{
    auto p1 = _Polynomial{v1};
    const auto p2 = _Polynomial{v2};
    for (auto _ : state) {
        p1 -= p2;
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_minus_transform);

BENCHMARK_MAIN();
