#include <iostream>
#include <map>
#include <mutex>
#include <string>


class IShop {
protected:
    std::map<std::string, double> prices;
    std::mutex changeLock;
private:
    friend class IProduct;

    void AddProduct(const std::string& name, const double price) {
        prices[name] = price;
        onSaleChange(name, price);
    }

    void EraseProduct(const std::string& name) {
        prices.erase(name);
    }

    virtual void onSaleChange(const std::string& name, const double new_price) = 0;
public:
    virtual ~IShop() = default;
};

class Auchan : public IShop {
public:
    void onSaleChange(const std::string& name, const double new_price) {
        std::cout << "Auchan sales!!!" << std::endl;
        std::cout << "Price for " << name << ": " << new_price << std::endl;
    }
};

class Steam : public IShop {
public:
    void onSaleChange(const std::string& name, const double new_price) {
        std::lock_guard<std::mutex> lock(changeLock);
        std::cout << name << " " << prices[name] << " -> " << new_price << std::endl;
        prices[name] = new_price;
    }
};

class Odgi : public IShop {
public:
    void onSaleChange(const std::string& name, const double new_price) {
        std::lock_guard<std::mutex> lock(changeLock);
        prices[name] = new_price;
        std::cout << "Price:" << std::endl;
        for (const auto prod : prices) {
            std::cout << prod.first << " " << prod.second << std::endl;
        }
    }
};