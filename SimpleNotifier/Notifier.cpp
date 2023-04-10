//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <algorithm>
#include <boost/any.hpp>

#include "Notifier.hpp"

NotificationToken::~NotificationToken()
{
    notifier_->RemoveObserver(this);
}

Notifier::~Notifier()
{
    for (auto observer : observers_) {
        for (auto token : observer.second) {
            delete token;
        }
    }
}

NotificationToken* Notifier::AddObserver(const Notification<void> &notification, const std::function<void()> callback)
{
    auto any_callback = boost::any(callback);
    return InternalAddObserver(notification, any_callback);
}

void Notifier::RemoveObserver(const NotificationToken *token)
{
    auto &notification = token->notification_;
    std::unique_lock<std::mutex> guard(mutex_);

    auto it_notification = std::find_if(observers_.begin(), observers_.end(),
                                        [&notification](auto o){ return o.first.GetName() == notification.GetName(); });
    if (it_notification != observers_.end()) {
        auto it = std::find((*it_notification).second.begin(),
                            (*it_notification).second.end(),
                            token);
        if (it != (*it_notification).second.end()) {
            (*it_notification).second.erase(it);
        }
    }
}

void Notifier::Notify(const Notification<void> &notification)
{
    if (observers_.count(notification) == 0
        || observers_[notification].size() == 0) {
        printf("no match notification(%s) found.\n", notification.GetName().c_str());
        return;
    }
    
    for (auto token : observers_[notification]) {
        auto callback = boost::any_cast<std::function<void()>>(token->any_callback_);
        callback();
    }
}

NotificationToken* Notifier::InternalAddObserver(const NotificationBase &notification, const boost::any &any_callback)
{
    std::unique_lock<std::mutex> guard(mutex_);

    auto token = new NotificationToken(token_counter_++, this, notification, any_callback);
    auto it = std::find_if(observers_.begin(), observers_.end(),
                           [&notification](auto o){ return o.first.GetName() == notification.GetName(); });
    if (it == observers_.end()) {
        observers_[notification] = std::vector<NotificationToken*>{};
    }
    observers_[notification].push_back(token);

    return token;
}
