
#include "../names.h"

double jaccard(DI &a, DI &b) {
    // Нормализуем концы (на случай если down > up)
    a.sortEnds();
    b.sortEnds();

    const double lenA = a.length();
    const double lenB = b.length();

    const double inter = std::max(0.0, std::min(a.getUp(), b.getUp()) - std::max(a.getDown(), b.getDown()));
    const double uni = lenA + lenB - inter;

    // оба пустые/точечные -> считаем совпадающими
    if (uni <= 0.0) return 1.0;
    return inter / uni;
}
