
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>

// ---------------------------------------------------------------------
// UnitTestCoordXY

class UnitTestCoordXY : public WsjcppUnitTestBase {
    public:
        UnitTestCoordXY();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestCoordXY)

UnitTestCoordXY::UnitTestCoordXY()
    : WsjcppUnitTestBase("UnitTestCoordXY") {
}

// ---------------------------------------------------------------------

bool UnitTestCoordXY::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestCoordXY::executeTest() {
    compare("Not implemented", true, false);
    // TODO unit test source code here
}

// ---------------------------------------------------------------------

bool UnitTestCoordXY::doAfterTest() {
    // do somethig after test
    return true;
}


