#pragma once

#include <cstddef>
#include <entt/entt.hpp>

namespace ssg
{

// Intrusive linked-list hierarchy (the EnTT relationship pattern).
//
// A parent does not store a list of children directly. Instead it points at
// its FIRST child, and each child points at its NEXT/PREV sibling. Walk a
// parent's children by starting at `first` and following `next` until null.
//
//   parent
//     |  first
//     v
//   child A  <->  child B  <->  child C        (prev/next sibling links)
//     ^ parent      ^ parent      ^ parent
//
// Entities without this component (or with parent == null) are roots.
struct CRelationship
{
    std::size_t children{0};           // number of direct children
    entt::entity first{entt::null};    // head of the child list
    entt::entity prev{entt::null};     // previous sibling
    entt::entity next{entt::null};     // next sibling
    entt::entity parent{entt::null};   // up-link to the parent
};

} // namespace ssg
