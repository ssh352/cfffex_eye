#include <gtest/gtest.h>

/*
TEST(t_main, test) {
    fprintf(stdout, "%s\n", typeid(this).name());
}
*/
/*
class GTest_Class : public ::testing::Test
{
public:
    GTest_Class() {}
private:
    virtual void TestBody() {
        EXPECT_EQ(1, 1);
    }
    static ::testing::TestInfo* test_info_;
    static ::testing::TestInfo* test_info2_;
};

::testing::TestInfo* GTest_Class::test_info_ =
     ::testing::internal::MakeAndRegisterTestInfo(
        "Gtest", "case1", NULL, NULL,
        ::testing::internal::CodeLocation(__FILE__, __LINE__),
        ::testing::internal::GetTestTypeId(),
        ::testing::Test::SetUpTestCase,
        ::testing::Test::TearDownTestCase,
        new ::testing::internal::TestFactoryImpl< GTest_Class>);

::testing::TestInfo* GTest_Class::test_info2_ =
     ::testing::internal::MakeAndRegisterTestInfo(
        "Gtest", "case2", NULL, NULL,
        ::testing::internal::CodeLocation(__FILE__, __LINE__),
        ::testing::internal::GetTestTypeId(),
        ::testing::Test::SetUpTestCase,
        ::testing::Test::TearDownTestCase,
        new ::testing::internal::TestFactoryImpl< GTest_Class>);

*/
