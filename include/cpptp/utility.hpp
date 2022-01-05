#pragma once

#include <type_traits>

namespace cpptp
{
#if __cplusplus >= 201703L
    template<class F, class... Args>
    using return_type_t = std::invoke_result_t<F, Args...>;
#else
    template<class F, class... Args>
    using return_type_t = std::result_of_t<F(Args...)>;
#endif
} // namespace cpptp