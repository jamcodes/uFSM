#pragma once

#include <utility>
#include "traits.hpp"

template<size_type Idx, typename FsmT>
inline constexpr decltype(auto) Get(FsmT&& fsm) noexcept;

// template<size_type Idx, typename FsmT>
// inline constexpr decltype(auto) Get(FsmT&& fsm) noexcept
// {
//     return Get_impl<Idx>(std::forward<FsmT>(fsm));
// }

template<size_type Idx, typename FsmT>
using state_at = decltype( Get<Idx>(std::declval<FsmT>()) );
