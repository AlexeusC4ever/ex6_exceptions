#include <iostream>
#include "symmetric_matrix.h"
#include <crtdbg.h>
#include <vector>

//struct CHECK {
//    int h;
//    CHECK(int h_ = 0) : h(h_) {}
//
//    CHECK(const CHECK&) {
//        std::cout << "COPY CONSTRUCTOR" << std::endl;
//    }
//
//    CHECK& operator= (const CHECK&) {
//        std::cout << "COPY OPERATOR" << std::endl;
//    }
//
//    CHECK(CHECK&&) {
//        std::cout << "MOVE CONSTRUCTOR" << std::endl;
//    }
//
//    CHECK& operator= (CHECK&&) {
//        std::cout << "MOVE OPERATOR" << std::endl;
//    }
//};

void check_leaks() {
    Symmetric_Matrix<int> a(10);
    a.fill(2);
    //Symmetric_Matrix<int> b(a);
    Symmetric_Matrix<int> c(3);
    c = a;
    Symmetric_Matrix<int> d(a);

    Symmetric_Matrix<int> b(std::move(a));



    Symmetric_Matrix<int> for_reserve(c);
    for_reserve.fill(0);

 /*   for_reserve.reserve(200);*/
    std::cout << for_reserve.get_capacity() << std::endl;

    print(for_reserve);

    for_reserve.emplace_back(1);
    for_reserve.push_back(2);

    for_reserve.emplace_front(3);
    for_reserve.push_front(4);

    std::cout << for_reserve.get_capacity() << std::endl;
    std::cout << for_reserve.get_size() << std::endl;


    for_reserve = std::move(b);

    print(for_reserve);

    for_reserve.erase(for_reserve.begin());

    std::cout << for_reserve.get_capacity() << std::endl;
    std::cout << for_reserve.get_size() << std::endl;
    print(for_reserve);

}

int main()
{
    try {
        check_leaks();
    }
    catch (...) {
        std::cout << "error";
    }

    _CrtDumpMemoryLeaks();
    return 0;
}
