#pragma once
namespace ssg
{

class IInputCaptureProvider
{
  public:
    virtual ~IInputCaptureProvider() = default;

    virtual bool WantsCaptureKeyboard() const = 0;
    virtual bool WantsCaptureMouse() const = 0;
};

} // namespace ssg
