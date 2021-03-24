#include <benchmark/benchmark.h>
#include <vector>

class _Polynomial
{
  public:
    _Polynomial(std::vector<int> coeff_) : m_coeff(coeff_) {}

    _Polynomial &plus_equal_resize_loop(const _Polynomial &other)
    {
        if (m_coeff.size() < other.m_coeff.size())
            m_coeff.resize(other.m_coeff.size(), 0);
        for (std::size_t i = 0; i < other.m_coeff.size(); ++i)
            m_coeff[i] += other.m_coeff[i];
        return *this;
    }

    _Polynomial &plus_equal_copy_transform(const _Polynomial &other)
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

    _Polynomial &minus_equal_copy_transform(const _Polynomial &other)
    {
        auto result(other.m_coeff);
        std::transform(result.cbegin(), result.cend(), result.begin(),
                       std::negate<>{});
        this->plus_equal_copy_transform(_Polynomial{std::move(result)});
        return *this;
    }

    const auto &data() const noexcept { return m_coeff; }

  private:
    std::vector<int> m_coeff;
};

const auto v1 = std::vector<int>(50000, -1);
const auto v2 = std::vector<int>(1000000, 1);

static void BM_plus_equal_resize_loop(benchmark::State &state)
{
    auto p1 = _Polynomial{v1};
    const auto p2 = _Polynomial{v2};
    for (auto _ : state) {
        p1.plus_equal_resize_loop(p2);
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_plus_equal_resize_loop);

static void BM_plus_equal_copy_transform(benchmark::State &state)
{
    auto p1 = _Polynomial{v1};
    const auto p2 = _Polynomial{v2};
    for (auto _ : state) {
        p1.plus_equal_copy_transform(p2);
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_plus_equal_copy_transform);

static void BM_minus_equal_copy_transform(benchmark::State &state)
{
    auto p1 = _Polynomial{v1};
    const auto p2 = _Polynomial{v2};
    for (auto _ : state) {
        p1.minus_equal_copy_transform(p2);
        benchmark::DoNotOptimize(p1.data());
    }
}
BENCHMARK(BM_minus_equal_copy_transform);

BENCHMARK_MAIN();
