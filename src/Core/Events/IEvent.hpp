#pragma once

inline unsigned int GetID() {
    static unsigned int count = 0;
    return count++;
}

#define GENERATE_EVENT_TYPE() \
static unsigned int GetStaticType(){ \
    static unsigned int id = GetID(); \
    return id; \
}


namespace ssg
{
class IEvent
{
public:
    IEvent() = default;
    virtual ~IEvent() = default;


    virtual bool operator==(const IEvent& other)
    {
        return GetStaticType() == other.GetStaticType();
    }   

    GENERATE_EVENT_TYPE();
};
}