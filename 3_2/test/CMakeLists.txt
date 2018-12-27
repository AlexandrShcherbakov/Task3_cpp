project(shop_test)

add_executable(shop_test test.cpp)
target_include_directories(shop_test PRIVATE ..)

target_link_libraries(shop_test gtest gmock)