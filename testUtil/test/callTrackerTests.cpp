#define BOOST_TEST_MODULE CallTrackerTests
#include <boost/test/unit_test.hpp>
#include <testUtilLib/callTracker.hpp>

namespace Moon::Test
{

struct Dummy
{
    Dummy() = default;
    Dummy(int v) : value(v) {}
    Dummy(const Dummy&) = default;
    Dummy(Dummy&&) = default;
    Dummy& operator=(const Dummy&) = default;
    Dummy& operator=(Dummy&&) = default;
    ~Dummy() = default;

    int value = 0;
};

BOOST_AUTO_TEST_SUITE(CallTrackerTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    CallTracker<Dummy> t;
    BOOST_CHECK_EQUAL(t.mNoArgConstructorCalls, 1);
    BOOST_CHECK_EQUAL(t.mArgConstructorCalls, 0);
    BOOST_CHECK_EQUAL(t.mCopyConstructorCalls, 0);
    BOOST_CHECK_EQUAL(t.mMoveConstructorCalls, 0);
    BOOST_CHECK_EQUAL(t.mCopyAssignmentCalls, 0);
    BOOST_CHECK_EQUAL(t.mMoveAssignmentCalls, 0);
}

BOOST_AUTO_TEST_CASE(ArgumentConstructor)
{
    CallTracker<Dummy> t(42);
    BOOST_CHECK_EQUAL(t.mNoArgConstructorCalls, 0);
    BOOST_CHECK_EQUAL(t.mArgConstructorCalls, 1);
    BOOST_CHECK_EQUAL(t.value, 42);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
    CallTracker<Dummy> original;
    original.value = 10;

    CallTracker<Dummy> copy(original);
    BOOST_CHECK_EQUAL(copy.mCopyConstructorCalls, 1);
    BOOST_CHECK_EQUAL(copy.value, 10);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
    CallTracker<Dummy> original;
    original.value = 20;

    CallTracker<Dummy> moved(std::move(original));
    BOOST_CHECK_EQUAL(moved.mMoveConstructorCalls, 1);
    BOOST_CHECK_EQUAL(moved.value, 20);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
    CallTracker<Dummy> source;
    source.value = 30;

    CallTracker<Dummy> target;
    target = source;

    BOOST_CHECK_EQUAL(target.mCopyAssignmentCalls, 1);
    BOOST_CHECK_EQUAL(target.value, 30);
}

BOOST_AUTO_TEST_CASE(ChainedAssignment)
{
    CallTracker<Dummy> a, b, c;
    a.value = 1;
    b.value = 2;
    c.value = 3;

    a = b = c;

    BOOST_CHECK_EQUAL(a.value, 3);
    BOOST_CHECK_EQUAL(b.value, 3);
    BOOST_CHECK_EQUAL(b.mCopyAssignmentCalls, 1);
    BOOST_CHECK_EQUAL(a.mCopyAssignmentCalls, 1);
}

BOOST_AUTO_TEST_CASE(SelfAssignment)
{
    CallTracker<Dummy> t;
    t.value = 5;

    t = t;

    BOOST_CHECK_EQUAL(t.mCopyAssignmentCalls, 1);
    BOOST_CHECK_EQUAL(t.value, 5);
}

BOOST_AUTO_TEST_CASE(MoveSelfAssignment)
{
    CallTracker<Dummy> t;
    t.value = 6;

    t = std::move(t);

    BOOST_CHECK_EQUAL(t.mMoveAssignmentCalls, 1);
    BOOST_CHECK_EQUAL(t.value, 6);
}

BOOST_AUTO_TEST_SUITE_END()
}
