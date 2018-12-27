#include <algorithm>
#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "shop.h"


class IProduct {
    std::vector<std::weak_ptr<IShop>> shops;
    std::atomic_bool isActive;

protected:
    std::string name;
    std::atomic<double> Price;

public:
    IProduct(const std::string& n, const double price) : isActive(true), name(n), Price(price) {}

    void Attach(std::weak_ptr<IShop> shop) {
        shops.emplace_back(std::move(shop));
        if (isActive) {
            auto locked_shop = shops.back().lock();
            if (locked_shop) {
                locked_shop->AddProduct(name, Price);
            }
        }
    }

    void Detach(std::weak_ptr<IShop> shop) {
        auto locked_shop = shop.lock();
        if (locked_shop) {
            locked_shop->EraseProduct(name);
        }
        auto shopIt = std::find_if(shops.begin(), shops.end(), [&shop](std::weak_ptr<IShop>& s2){return shop.lock() == s2.lock();});
        if (shopIt != shops.end()) {
            shops.erase(shopIt);
        }
    }

    double GetPrice() const {
        return Price;
    }

    void StartSales() {
        isActive = true;
        for (auto shop: shops) {
            auto locked_shop = shop.lock();
            if (locked_shop) {
                locked_shop->AddProduct(name, Price);
            }
        }
    }

    void StopSales() {
        isActive = false;
        for (auto shop: shops) {
            auto locked_shop = shop.lock();
            if (locked_shop) {
                locked_shop->EraseProduct(name);
            }
        }
    }

    virtual void ChangePrice(const double price) {
        Price = price;
        notify();
    }

    virtual ~IProduct() {
        StopSales();
    }

    void notify() {
        for (auto shop: shops) {
            auto locked_shop = shop.lock();
            if (locked_shop) {
                locked_shop->onSaleChange(name, Price);
            }
        }
    }
};

class Pinapple : public IProduct {
public:
    Pinapple(double price) : IProduct("Pinapple", price) {}
};

class Warcraft : public IProduct {
public:
    Warcraft(double price) : IProduct("Warcraft", price) {}
};

class BFG9000 : public IProduct {
public:
    BFG9000(double price) : IProduct("BFG9000", price) {}
};