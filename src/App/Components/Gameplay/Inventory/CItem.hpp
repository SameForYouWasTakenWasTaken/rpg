#pragma once

#include "Types.hpp"

namespace ssg
{
struct CItem
{
    String itemTypeId{};
    ItemCount_t currentCount{1};
    ItemCount_t maxStack{1};
};
} // namespace ssg