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
    std::map<NotifyBase, std::vector<boost::any>> observers_;

public:
    Notifier(): observers_() {}

public:
    template <typename T>
    NotificationToken* AddObserver(Notification<T> notify, std::function<void(T)> callback)
    {
        std::unique_lock<std::mutex> guard(mutex_);

        if (observers_.count(notify) == 0) {
            observers_[notify] = std::vector<boost::any>{};
        }
        observers_[notify].push_back(boost::any(callback));

        return nullptr;
    }

    void RemoveObserver(const NotificationToken *token);
};
