#pragma once
#include "MapObject.hpp"
#include "Types.hpp"

namespace ssg::map
{
class ObjectLayer
{
  public:
    ObjectLayer() = default;

    [[nodiscard]] String getName() const { return m_Name; }
    [[nodiscard]] const Vector<MapObject>& getObjects() const { return m_Objects; }

  private:
    String m_Name;

    Vector<MapObject> m_Objects;
};
} // namespace ssg::map