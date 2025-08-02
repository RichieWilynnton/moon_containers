add_executable(VectorPerfTest
    vectorPerfTest.cpp
)

depend_and_link(VectorPerfTest
    VectorLib
    benchmark::benchmark
)
