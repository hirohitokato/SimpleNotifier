//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <boost/any.hpp>

#include "Notifier.hpp"
struct NotificationToken {
    int id;
};

void Notifier::RemoveObserver(const NotificationToken *token)
{
    std::unique_lock<std::mutex> guard(mutex_);
    if (observers_.count(notify) > 0) {
        observers_[notify] = std::vector<boost::any>{};
    }
}

