#include <thread>

#include "product.h"


int main() {
    std::shared_ptr<IShop> shopPtr1 = std::make_shared<Auchan>();
    std::shared_ptr<IShop> shopPtr2 = std::make_shared<Steam>();
    std::shared_ptr<IShop> shopPtr3 = std::make_shared<Odgi>();

    std::thread sales([&]() {
        Pinapple prod1(15);
        prod1.StartSales();
        prod1.Attach(shopPtr1);
        prod1.Attach(shopPtr2);
        Warcraft prod2(13);
        prod2.StartSales();
        prod2.Attach(shopPtr3);
        prod2.Attach(shopPtr1);
        prod1.Detach(shopPtr2);
        prod2.Detach(shopPtr3);
        prod2.ChangePrice(12.99);
        prod1.ChangePrice(16);
        BFG9000 prod3(45);
        prod3.Attach(shopPtr1);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    shopPtr2.reset();
    shopPtr3.reset();

    sales.join();
}