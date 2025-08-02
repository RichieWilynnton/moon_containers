#include <benchmark/benchmark.h>

#include <VectorLib/vector.hpp>

static void CustomArguments(benchmark::internal::Benchmark* b)
{
    b->Arg(10)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
}

static void BM_MoonVectorPushBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        Moon::Vector<int> vec;
        for (int i = 0; i < state.range(0); ++i)
        {
            vec.PushBack(i);
        }
        state.counters["Capacity"] = vec.Capacity();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_StdVectorPushBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vec;
        for (int i = 0; i < state.range(0); ++i)
        {
            vec.push_back(i);
        }
        state.counters["Capacity"] = vec.capacity();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_MoonVectorIteration(benchmark::State& state)
{
    Moon::Vector<int> vec;
    for (int i = 0; i < state.range(0); ++i)
    {
        vec.PushBack(i);
    }

    for (auto _ : state)
    {
        int sum = 0;
        for (auto it = vec.Begin(); it != vec.End(); ++it)
        {
            sum += *it;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_StdVectorIteration(benchmark::State& state)
{
    std::vector<int> vec;
    for (int i = 0; i < state.range(0); ++i)
    {
        vec.push_back(i);
    }

    for (auto _ : state)
    {
        int sum = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            sum += *it;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_MoonVectorRandomAccess(benchmark::State& state)
{
    Moon::Vector<int> vec;
    for (int i = 0; i < state.range(0); ++i)
    {
        vec.PushBack(i);
    }

    for (auto _ : state)
    {
        int sum = 0;
        for (size_t i = 0; i < vec.Size(); ++i)
        {
            sum += vec[i];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_StdVectorRandomAccess(benchmark::State& state)
{
    std::vector<int> vec;
    for (int i = 0; i < state.range(0); ++i)
    {
        vec.push_back(i);
    }

    for (auto _ : state)
    {
        int sum = 0;
        for (size_t i = 0; i < vec.size(); ++i)
        {
            sum += vec[i];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_MoonVectorEmplaceBack(benchmark::State& state)
{
    struct TestStruct
    {
        int a, b, c;
        TestStruct(int x, int y, int z) : a(x), b(y), c(z) {}
    };

    for (auto _ : state)
    {
        Moon::Vector<TestStruct> vec;
        for (int i = 0; i < state.range(0); ++i)
        {
            vec.EmplaceBack(i, i + 1, i + 2);
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void BM_StdVectorEmplaceBack(benchmark::State& state)
{
    struct TestStruct
    {
        int a, b, c;
        TestStruct(int x, int y, int z) : a(x), b(y), c(z) {}
    };

    for (auto _ : state)
    {
        std::vector<TestStruct> vec;
        for (int i = 0; i < state.range(0); ++i)
        {
            vec.emplace_back(i, i + 1, i + 2);
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK(BM_MoonVectorPushBack)->Apply(CustomArguments);
BENCHMARK(BM_StdVectorPushBack)->Apply(CustomArguments);
BENCHMARK(BM_MoonVectorIteration)->Apply(CustomArguments);
BENCHMARK(BM_MoonVectorRandomAccess)->Apply(CustomArguments);
BENCHMARK(BM_StdVectorRandomAccess)->Apply(CustomArguments);
BENCHMARK(BM_MoonVectorEmplaceBack)->Apply(CustomArguments);
BENCHMARK(BM_StdVectorEmplaceBack)->Apply(CustomArguments);

BENCHMARK_MAIN();
