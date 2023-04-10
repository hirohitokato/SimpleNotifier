//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <iostream>
#include <cstdio>
#include "Notifier.hpp"
#include "Notification.hpp"

static const auto HelloNotification = Notification<int>("Hello");

int main(int argc, const char * argv[]) {
    
    {
        Notifier notifier;

        notifier.Notify(HelloNotification, 1);
        auto token1 = notifier.AddObserver(HelloNotification,
                             std::function([&](int a){
                                 printf("(1) called: %d\n",a);
                             }));
        auto token2 = notifier.AddObserver(HelloNotification,
                             std::function([&](int a){
                                 printf("(2) called: %d\n",a);
                             }));

        notifier.Notify(HelloNotification, 1);
        std::cout << "----------------------\n";
//        delete token1;
        notifier.Notify(HelloNotification, 2);
        std::cout << "----------------------\n";
//        notifier.RemoveObserver(token2);
        notifier.Notify(HelloNotification, 3);
        std::cout << "----------------------\n";

        auto void_notification = Notification<void>("VOID");
        notifier.AddObserver(void_notification, std::function<void()>([&](){
            printf("called\n");
        }));
        notifier.Notify(void_notification);
    }

    return 0;
}
