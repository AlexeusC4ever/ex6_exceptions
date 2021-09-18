#include <iostream>
#include "symmetric_matrix.h"
#include <crtdbg.h>

void check_leaks() {
    Symmetric_Matrix<int> a(5);
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

    for (int i = 0; i < 2; ++i) {
        for_reserve.push_back(2);


        for_reserve.push_front(6);

        auto it = for_reserve.begin();
        it += std::rand() % for_reserve.get_size();


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
