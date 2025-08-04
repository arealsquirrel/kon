#ifndef KN_VIEW_HPP
#define KN_VIEW_HPP

#include <functional>
namespace kon {

template<typename T>
using foreach_function = std::function<void(T&)>;

template<typename T>
using view_function = std::function<bool(const T&)>;

}

#endif
