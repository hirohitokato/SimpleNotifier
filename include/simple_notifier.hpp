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

#include "_simple_notifier.hpp"

/// A token object to act as the observer. Notifier strongly holds this return value until you remove the observer registration.
///
/// It is not possible to instantiate the object, because the constructor is not public. You can get the object
/// via a Notifier's add_observer() method.
class NotificationToken {
    // Make Notifier a friend to act as a kind of opaque object.
    friend Notifier;

    _Removable *notifier_;
    const NotificationBase &notification_;
    const boost::any any_callback_;

public:
    /// An identifier of the token object.
    const int id;

private:
    NotificationToken(int id,
                      _Removable *notifier,
                      const NotificationBase &notification,
                      const boost::any &any_callback):
    id(id), notifier_(notifier), notification_(notification), any_callback_(any_callback)
    {}

    ~NotificationToken()
    {
        notifier_->remove_observer(this);
    }
};


/// A notification dispatch mechanism that enables the broadcast of information to registered observers.
class Notifier: public _Removable {
    std::mutex mutex_;
    int token_counter_ = 0;
    std::map<NotificationBase, std::vector<NotificationToken*>> observers_;

public:
    Notifier(): observers_() {}
    virtual ~Notifier()
    {
        for (auto observer : observers_) {
            for (auto token : observer.second) {
                delete token;
            }
        }
    }

    /// Add an entry to the notifier to receive notifications that passed to the provided functional object.
    ///
    /// - Parameters:
    ///   - notification: The object of the notification to register for delivery to the observer.
    ///   - callback: The callback that executes when receiving a notification.
    /// - Returns:
    ///  - A token for the added entry. Since this method returns newed instance, delete it when it becomes unnecessary.
    template <typename T>
    NotificationToken* add_observer(const Notification<T> &notification, const std::function<void(T)> callback)
    {
        auto any_callback = boost::any(callback);
        return add_observer_prv(notification, any_callback);
    }

    /// Add an entry to the notifier to receive notifications that passed to the provided functional object.
    ///
    /// This is an overload of add_observer\<T\>().
    /// - FIXME: Use explicit specialization
    ///
    /// - Parameters:
    ///   - notification: The object of the notification to register for delivery to the observer.
    ///   - callback: The callback that executes when receiving a notification.
    /// - Returns:
    ///  - A token for the added entry. Since this method returns newed instance, delete it when it becomes unnecessary.
    NotificationToken* add_observer(const Notification<void> &notification, const std::function<void(void)> callback)
    {
        auto any_callback = boost::any(callback);
        return add_observer_prv(notification, any_callback);
    }

    /// Removes matching entries from the notifier's dispatch table.
    ///
    /// - Parameter token: The token to remove from dispatch table.
    void remove_observer(const NotificationToken *token)
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

    /// Posts a notification with a given Notification object and information.
    ///
    /// - Parameters:
    ///   - notification: The object of the notification.
    ///   - value: An associated value with the notification.
    template <typename T>
    void notify(const Notification<T> &notification, T value)
    {
        if (observers_.count(notification) == 0
            || observers_[notification].size() == 0) {
            printf("no match notification(%s) found.\n", notification.GetName().c_str());
            return;
        }

        for (auto token : observers_[notification]) {
            auto callback = boost::any_cast<std::function<void(T)>>(token->any_callback_);
            callback(value);
        }
    }

    /// Posts a notification with a given Notification object and information.
    ///
    /// This is an overload of notify\<T\>().
    /// - FIXME: Use explicit specialization
    ///
    /// - Parameters:
    ///   - notification: The object of the notification.
    ///   - value: An associated value with the notification.
    void notify(const Notification<void> &notification)
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

private:
    NotificationToken* add_observer_prv(const NotificationBase &notification, const boost::any &any_callback)
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
};
