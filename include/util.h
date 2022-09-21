#pragma once

#include <iomanip>
#include <sstream>
#include <string>

template <class T> std::string FormatWithCommas(T value) {
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << value;
    return ss.str();
}

#define ROW_UP(y) WIDTH *(y - 1)
#define ROW_OFF(y) WIDTH *y
#define ROW_DOWN(y) WIDTH *(y + 1)
#define TO_INDEX(x, y) WIDTH *(y) + (x)
#define IN_BOUNDS(x, y) ((x) >= 0 && (x) < WIDTH && (y) >= 0 && (y) < HEIGHT)

template <typename E>
concept Enum = std::is_enum<E>::value;

template <typename E>
concept EnumWithEnd = requires(E e) {
    Enum<E>;
    E::END_OF_LIST;
};

template <typename E> E &operator++(E &e) requires EnumWithEnd<E> {
    using IntType = typename std::underlying_type<E>::type;
    e = static_cast<E>(static_cast<IntType>(e) + 1);
    if (e == E::END_OF_LIST)
        e = static_cast<E>(0);
    return e;
}
