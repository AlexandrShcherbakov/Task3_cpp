#include <gtest/gtest.h>

#include <product.h>


TEST(IProductTest, Price) {
    std::shared_ptr<IProduct> prod = std::make_shared<Pinapple>(300);
    EXPECT_DOUBLE_EQ(300, prod->GetPrice());
    prod->ChangePrice(20);
    EXPECT_DOUBLE_EQ(20, prod->GetPrice());
    prod = std::make_shared<Warcraft>(15000);
    EXPECT_DOUBLE_EQ(15000, prod->GetPrice());
}

TEST(IShopIProductTest, Events) {
    testing::internal::CaptureStdout();
    std::shared_ptr<IShop> steam = std::make_shared<Steam>();
    Pinapple prod(10);
    prod.Attach(steam);
    prod.ChangePrice(20);
    prod.StopSales();
    prod.StartSales();
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Pinapple 10 -> 10\nPinapple 10 -> 20\nPinapple 20 -> 20\n");
}

TEST(IShopIProductTest, SeveralShops) {
    testing::internal::CaptureStdout();
    std::shared_ptr<IShop> odgi = std::make_shared<Odgi>();
    Pinapple prod(10);
    Warcraft wc(100);
    prod.StopSales();
    wc.StopSales();
    prod.Attach(odgi);
    wc.Attach(odgi);
    prod.StartSales();
    wc.StartSales();
    prod.StopSales();
    wc.ChangePrice(200);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Price:\nPinapple 10\nPrice:\nPinapple 10\nWarcraft 100\nPrice:\nWarcraft 200\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}