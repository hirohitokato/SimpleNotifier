//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#pragma once

#include <string>

class NotifyBase {
    const std::string name_;
public:
    virtual ~NotifyBase() {}
    std::string GetName() { return name_; }

    bool operator<(const NotifyBase& other) const
    {
        return name_ < other.name_;
    }

protected:
    NotifyBase(const std::string &name): name_(name) {}
};

template <typename T>
class Notification: public NotifyBase {
    T* typeholder_;

public:
    Notification(const std::string &name): NotifyBase(name) {}
};
