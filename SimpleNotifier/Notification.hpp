//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#pragma once

#include <string>

class NotificationBase {
    const std::string name_;
public:
    virtual ~NotificationBase() {}
    const std::string &GetName() const { return name_; }

    bool operator<(const NotificationBase& other) const
    {
        return name_ < other.name_;
    }

protected:
    NotificationBase(const std::string &name): name_(name) {}
};

template <typename T>
class Notification: public NotificationBase {
    T* typeholder_;

public:
    Notification(const std::string &name): NotificationBase(name) {}
};
