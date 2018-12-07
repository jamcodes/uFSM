#pragma once

#include <type_traits>
#include <utility>
#include "traits.hpp"
#include "logging.hpp"


template<typename State, typename FsmT, typename = void_t<>>
struct has_exit : std::false_type { };

template<typename State, typename FsmT>
struct has_exit<State, FsmT,
    void_t<decltype(std::declval<State>().exit(std::declval<FsmT>()))>>
    : std::true_type
{
};

template<typename State, typename FsmT>
constexpr inline auto has_exit_v{has_exit<State,FsmT>::value};


template<typename FsmT, typename State>
constexpr inline std::enable_if_t<has_exit_v<State, Self<FsmT>>>
fsm_exit(FsmT&& fsm, State&& state) noexcept {
    fsm_log_exit(fsm.self(), state);
    std::forward<State>(state).exit(std::forward<FsmT>(fsm).self());
}

template<typename FsmT, typename State>
constexpr inline std::enable_if_t<!has_exit_v<State, Self<FsmT>>>
fsm_exit(FsmT&&, State&&) noexcept {/* nop */}