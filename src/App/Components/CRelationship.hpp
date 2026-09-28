#pragma once

#include <Entity.hpp>
#include <cstddef>

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
    std::size_t children{0};   // number of direct children
    Entity first{entt::null};  // head of the child list
    Entity prev{entt::null};   // previous sibling
    Entity next{entt::null};   // next sibling
    Entity parent{entt::null}; // up-link to the parent
};

} // namespace ssg
