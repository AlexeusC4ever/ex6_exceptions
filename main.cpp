#include <iostream>
#include "symmetric_matrix.h"
#include <crtdbg.h>
#include <vector>

struct CHECK {
    int h;
    CHECK(int h_, int a) : h(h_) {}

    CHECK(const CHECK&) {
        std::cout << "COPY CONSTRUCTOR" << std::endl;
    }

    CHECK& operator= (const CHECK&) {
        std::cout << "COPY OPERATOR" << std::endl;
    }

    CHECK(CHECK&&) {
        std::cout << "MOVE CONSTRUCTOR" << std::endl;
    }

    CHECK& operator= (CHECK&&) {
        std::cout << "MOVE OPERATOR" << std::endl;
    }
};

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

    a = c;

 /*   for_reserve.reserve(200);*/
    std::cout << for_reserve.get_capacity() << std::endl;

    //print(for_reserve);

    //print(for_reserve);

    for (int i = 0; i < 10; ++i) {
        for_reserve.push_back(2);


        for_reserve.push_front(4);

        auto it = for_reserve.begin();
        it += std::rand() % for_reserve.get_size();

        for_reserve.erase(it);


    }

    auto it = for_reserve.begin();
    //it += 1245;

    for_reserve.erase(it);


    //it = for_reserve.begin();
    //for_reserve.erase(it);
    //*it = 34;

    std::cout << for_reserve.get_capacity() << std::endl;
    std::cout << for_reserve.get_size() << std::endl;



    print(for_reserve);

    CHECK ch(0, 0);
    ch.h = 2;

    //std::vector<CHECK> checkvec;
    //checkvec.emplace_back(0, 1);

    Symmetric_Matrix<CHECK> che;
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
