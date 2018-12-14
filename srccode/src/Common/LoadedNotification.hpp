#pragma once

#include "Poco/Notification.h"

/**
 * @brief Notification that can carry payload
 */
template <typename T>
class LoadedNotification : public Poco::Notification
{
public:
    explicit LoadedNotification(const T& data)
    : _data(data)
    {
    }

    virtual ~LoadedNotification()
    {
    }

    const T& getData() const
    {
        return _data;
    }
private:

    const T _data;
};

