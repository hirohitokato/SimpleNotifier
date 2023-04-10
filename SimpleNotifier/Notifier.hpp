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

class Notifier;


/// A token object to act as the observer. Notifier strongly holds this return value until you remove the observer registration.
///
/// It is not possible to instantiate the object, because the constructor is not public. You can get the object
/// via a Notifier's AddObserver() method.
struct NotificationToken {
    friend Notifier;
private:
    const int id;
    Notifier *notifier;
    const NotificationBase &notification;
    const boost::any any_callback;

    NotificationToken(int id,
                      Notifier *notifier,
                      const NotificationBase &notification,
                      const boost::any &any_callback):
    id(id), notifier(notifier), notification(notification), any_callback(any_callback)
    {}

    ~NotificationToken();
};


/// A notification dispatch mechanism that enables the broadcast of information to registered observers.
class Notifier {
    std::mutex mutex_;
    int token_counter_ = 0;
    std::map<NotificationBase, std::vector<NotificationToken*>> observers_;

public:
    Notifier(): observers_() {}
    virtual ~Notifier();

    /// Add an entry to the notifier to receive notifications that passed to the provided functional object.
    ///
    /// - Parameters:
    ///   - notification: The object of the notification to register for delivery to the observer.
    ///   - callback: The callback that executes when receiving a notification.
    /// - Returns:
    ///  - A token for the added entry. Since this method returns newed instance, delete it when it becomes unnecessary.
    template <typename T>
    NotificationToken* AddObserver(const Notification<T> &notification, const std::function<void(T)> callback)
    {
        auto any_callback = boost::any(callback);
        return InternalAddObserver(notification, any_callback);
    }

    /// Add an entry to the notifier to receive notifications that passed to the provided functional object.
    ///
    /// This is an overload of AddObserver\<T\>().
    /// - FIXME: Use explicit specialization
    ///
    /// - Parameters:
    ///   - notification: The object of the notification to register for delivery to the observer.
    ///   - callback: The callback that executes when receiving a notification.
    /// - Returns:
    ///  - A token for the added entry. Since this method returns newed instance, delete it when it becomes unnecessary.
    NotificationToken* AddObserver(const Notification<void> &notification, const std::function<void(void)> callback);


    /// Removes matching entries from the notifier's dispatch table.
    ///
    /// - Parameter token: The token to remove from dispatch table.
    void RemoveObserver(const NotificationToken *token);


    /// Posts a notification with a given Notification object and information.
    ///
    /// - Parameters:
    ///   - notification: The object of the notification.
    ///   - value: An associated value with the notification.
    template <typename T>
    void Notify(const Notification<T> &notification, T value)
    {
        if (observers_.count(notification) == 0
            || observers_[notification].size() == 0) {
            printf("no match notification(%s) found.\n", notification.GetName().c_str());
            return;
        }

        for (auto token : observers_[notification]) {
            auto callback = boost::any_cast<std::function<void(T)>>(token->any_callback);
            callback(value);
        }
    }

    /// Posts a notification with a given Notification object and information.
    ///
    /// This is an overload of Notify\<T\>().
    /// - FIXME: Use explicit specialization
    ///
    /// - Parameters:
    ///   - notification: The object of the notification.
    ///   - value: An associated value with the notification.
    void Notify(const Notification<void> &notification);

private:
    NotificationToken* InternalAddObserver(const NotificationBase &notification, const boost::any &callback);
};
