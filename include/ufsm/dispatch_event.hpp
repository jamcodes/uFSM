#pragma once

#include <type_traits>
#include <utility>
#include "traits.hpp"
#include "state_transition.hpp"
#include "logging.hpp"

// 1. Evaluate the guard condition associated with the transition and perform the following steps
//    only if the guard evaluates to TRUE.
// 2. Exit the source state configuration.
// 3. Execute the actions associated with the transition.
// 4. Enter the target state configuration.
// dispatch_event using `transition` for branching during event handling
template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept
{
    if (Idx == fsm.state()) {
        using state_t = std::decay_t<state_at<Idx, FsmT>>;
        using event_t = std::decay_t<Event>;
        auto&& state = Get<Idx>(fsm);
        fsm_log_event(fsm.self(), state, event);
        if constexpr (has_handle_event_v<state_t, decltype(fsm.self()), Event>) {
            state.handle_event(fsm.self(), std::forward<Event>(event));
        }
        state_transition<event_t>(std::forward<FsmT>(fsm), std::forward<decltype(state)>(state));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
}