//
// Created by Zero on 2020/9/1.
//

#ifndef LUMINOUS_COBJECT_H
#define LUMINOUS_COBJECT_H

#include "util/header.h"

LUMINOUS_BEGIN

inline namespace utility {

    class CObject {

    };

    struct Noncopyable {
        Noncopyable() = default;

        Noncopyable(const Noncopyable &) = delete;

        Noncopyable &operator=(const Noncopyable &) = delete;
    };

};

LUMINOUS_END


#endif //LUMINOUS_COBJECT_H
