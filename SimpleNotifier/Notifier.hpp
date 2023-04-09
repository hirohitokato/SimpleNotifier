//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#pragma once

#include <map>
#include <vector>
#include <functional>
#include <mutex>
#include <boost/any.hpp>

#include "Notification.hpp"

class NotificationToken;

class Notifier {
    std::mutex mutex_;
    std::map<NotificationBase, std::vector<boost::any>> observers_;

public:
    Notifier(): observers_() {}

public:
    template <typename T>
    NotificationToken* AddObserver(const Notification<T> &notification, const std::function<void(T)> callback)
    {
        auto any_callback = boost::any(callback);
        return InternalAddObserver(notification, any_callback);
    }

    NotificationToken* AddObserver(const Notification<void> &notification, const std::function<void()> callback);

    void RemoveObserver(const NotificationToken *token);

    template <typename T>
    void Notify(const Notification<T> &notification, T value)
    {
        if (observers_.count(notification) == 0) {
            printf("no match notification(%s) found.\n", notification.GetName().c_str());
            return;
        }

        for (auto any_observer : observers_[notification]) {
            auto callback = boost::any_cast<std::function<void(T)>>(any_observer);
            callback(value);
        }
    }

    void Notify(const Notification<void> &notification);

private:
    NotificationToken* InternalAddObserver(const NotificationBase &notification, boost::any callback);
};
