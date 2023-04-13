//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#pragma once

#include <string>

namespace SimpleNotifier {

    /// A base class for Notification class, to hold any type of Notification objects to one container.
    class NotificationBase {
        const std::string name_;
    public:
        virtual ~NotificationBase() {}
        const std::string &GetName() const { return name_; }

        bool operator<(const NotificationBase& other) const { return name_ < other.name_; }

    protected:
        NotificationBase(const std::string &name): name_(name) {}
    };

    /// An object containing information broadcast to all registered observers.
    template <typename T>
    class Notification: public NotificationBase {
        T* typeholder_;

    public:
        Notification(const std::string &name): NotificationBase(name) {}
    };


    class Notifier;
    class NotificationToken;

    struct _Removable {
        virtual void remove_observer(const NotificationToken *token) = 0;
    };
}
