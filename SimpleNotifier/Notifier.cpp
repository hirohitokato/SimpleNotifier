//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <boost/any.hpp>

#include "Notifier.hpp"
struct NotificationToken {
    const int id;
    const NotificationBase &notification;

    NotificationToken(int id, const NotificationBase &notification)
    :id(id), notification(notification){}
};

void Notifier::RemoveObserver(const NotificationToken *token)
{
    auto notification = token->notification;
    std::unique_lock<std::mutex> guard(mutex_);

    auto it = observers_.find(notification);
    if (it != observers_.end()) {
        observers_.erase(it);
    }
}

void Notifier::notify(const Notification<void> notification)
{
    if (observers_.count(notification) == 0) {
        printf("no match notification(%s) found.\n", notification.GetName().c_str());
        return;
    }
    for (auto any_observer : observers_[notification]) {
        auto callback = boost::any_cast<std::function<void()>>(any_observer);
        callback();
    }
}
