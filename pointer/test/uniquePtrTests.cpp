#include <boost/test/tools/old/interface.hpp>
#define BOOST_TEST_MODULE UniquePtrTests
#include <PointerLib/uniquePtr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

namespace Moon::Test
{

struct Dummy
{
    int value;
    static int destructorCount;

    Dummy() : value(0) {}
    Dummy(int v) : value(v) {}
    ~Dummy()
    {
        ++destructorCount;
    }
};

int Dummy::destructorCount = 0;

struct UniquePtrFixture
{
    UniquePtrFixture()
    {
        Dummy::destructorCount = 0;
    }
    ~UniquePtrFixture() {}
};

BOOST_FIXTURE_TEST_SUITE(UniquePtrTestSuite, UniquePtrFixture)
BOOST_AUTO_TEST_CASE(UniquePtrTest)
{
    Moon::UniquePtr<Dummy> ptr(new Dummy(42));

    BOOST_CHECK(ptr.Get() != nullptr);

    BOOST_CHECK_EQUAL((*ptr).value, 42);
    BOOST_CHECK_EQUAL(ptr->value, 42);

    Moon::UniquePtr<Dummy> moved_ptr(std::move(ptr));

    BOOST_CHECK(ptr.Get() == nullptr);

    BOOST_CHECK_EQUAL((*moved_ptr).value, 42);
}

BOOST_AUTO_TEST_CASE(UniquePtrResetAndReleaseTest)
{
    Moon::UniquePtr<Dummy> ptr(new Dummy(100));
    BOOST_CHECK(ptr.Get() != nullptr);
    ptr.Reset();
    BOOST_CHECK(ptr.Get() == nullptr);

    ptr.Reset(new Dummy(200));
    BOOST_CHECK(ptr.Get() != nullptr);
    BOOST_CHECK_EQUAL(ptr->value, 200);

    Dummy* raw = ptr.Release();
    BOOST_CHECK(ptr.Get() == nullptr);
    BOOST_CHECK(raw != nullptr);
    delete raw;
}

BOOST_AUTO_TEST_CASE(UniquePtrAssignmentTest)
{
    Moon::UniquePtr<Dummy> ptr1(new Dummy(10));
    Moon::UniquePtr<Dummy> ptr2;
    ptr2 = std::move(ptr1);

    BOOST_CHECK(ptr1.Get() == nullptr);
    BOOST_CHECK(ptr2.Get() != nullptr);
    BOOST_CHECK_EQUAL(ptr2->value, 10);
}

BOOST_AUTO_TEST_CASE(UniquePtrConstructor)
{
    Dummy* dummy = new Dummy(10);
    Moon::UniquePtr<Dummy> ptr1(dummy);

    BOOST_CHECK(ptr1.Get() != nullptr);
    BOOST_CHECK_EQUAL(ptr1->value, 10);
}
BOOST_AUTO_TEST_CASE(UniquePtrDestructor)
{
    {
        Moon::UniquePtr<Dummy> ptr1(new Dummy(10));
        Moon::UniquePtr<Dummy> ptr2(new Dummy(10));
    }
    BOOST_CHECK(Dummy::destructorCount == 2);
}
BOOST_AUTO_TEST_CASE(UniquePtrArrayTest)
{
    Moon::UniquePtr<Dummy[]> arr(new Dummy[3]{Dummy(1), Dummy(2), Dummy(3)});
    BOOST_CHECK(arr.Get() != nullptr);
    BOOST_CHECK_EQUAL(arr[0].value, 1);
    BOOST_CHECK_EQUAL(arr[1].value, 2);
    BOOST_CHECK_EQUAL(arr[2].value, 3);

    arr.Reset();
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 3);
    BOOST_CHECK(arr.Get() == nullptr);
}

BOOST_AUTO_TEST_CASE(UniquePtrBoolCast)
{
    Moon::UniquePtr<Dummy> dummy1;
    Moon::UniquePtr<Dummy[]> dummy2;
    BOOST_CHECK(dummy1 == false);
    BOOST_CHECK(dummy2 == false);
    dummy1.Reset(new Dummy(5));
    dummy2.Reset(new Dummy[3]{Dummy(1), Dummy(2), Dummy(3)});
    BOOST_CHECK(dummy1 == true);
    BOOST_CHECK(dummy2 == true);
}
BOOST_AUTO_TEST_CASE(UniquePtrMakeUnique)
{
    auto ptr = Moon::UniquePtr<Dummy>::MakeUnique(30);
    BOOST_CHECK(ptr.Get() != nullptr);
    BOOST_CHECK_EQUAL(ptr->value, 30);

    auto arr_ptr = Moon::UniquePtr<Dummy[]>::MakeUnique(3);
    arr_ptr[0] = Dummy(1);
    arr_ptr[1] = Dummy(2);
    arr_ptr[2] = Dummy(3);
    BOOST_CHECK(arr_ptr.Get() != nullptr);
    BOOST_CHECK_EQUAL(arr_ptr[0].value, 1);
    BOOST_CHECK_EQUAL(arr_ptr[1].value, 2);
    BOOST_CHECK_EQUAL(arr_ptr[2].value, 3);
} 
BOOST_AUTO_TEST_SUITE_END()

}  // namespace Moon::Test
