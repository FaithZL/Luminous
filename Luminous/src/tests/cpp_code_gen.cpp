//
// Created by Zero on 2020/9/20.
//

#include <compute/dsl.h>
#include <compute/codegen.h>
#include <compute/buffer.h>

using namespace std;

using namespace luminous;

int main() {
    variant<int , float> a = 1;

    cout << a.valueless_by_exception() << endl;

    return 0;
}
