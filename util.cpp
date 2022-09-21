#include "include/util.h"

// template <EnumWithEnd E> E &operator++(E &e) {
//     using IntType = typename std::underlying_type<E>::type;
//     e = static_cast<E>(static_cast<IntType>(e) + 1);
//     if (e == E::END_OF_LIST)
//         e = static_cast<E>(0);
//     return e;
// }
