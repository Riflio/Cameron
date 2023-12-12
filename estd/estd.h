#ifndef ESTD_H
#define ESTD_H

/**
* @brief Дополнительные возможности для std
*/

#include <utility>

/**
* @brief Компоратор для первого значения структуры
* @example std::find_if(list.begin(), list.end(), CompareFirst(42))
*/
template <typename A, typename B>
struct CompareFirst
{
  CompareFirst(const A &val): _val(val) {}
  bool operator()(const std::pair<A, B> &element) const {
    return _val==element.first;
  }
  private:
    A _val;
};

#endif // ESTD_H
