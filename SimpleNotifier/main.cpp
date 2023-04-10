//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <iostream>
#include <cstdio>
#include "Notifier.hpp"
#include "Notification.hpp"

int main(int argc, const char * argv[]) {
    Notifier notifier;

    auto hello_notification = Notification<int>("Hello");
    notifier.Notify(hello_notification, 1);
    auto token1 = notifier.AddObserver(hello_notification,
                         std::function([&](int a){
                             printf("(1)called: %d\n",a);
                         }));
    auto token2 = notifier.AddObserver(hello_notification,
                         std::function([&](int a){
                             printf("(2)called: %d\n",a);
                         }));

    notifier.Notify(hello_notification, 1);
    delete token1;
    notifier.Notify(hello_notification, 2);
    notifier.RemoveObserver(token2);
    notifier.Notify(hello_notification, 3);

    auto void_notification = Notification<void>("VOID");
    notifier.AddObserver(void_notification, std::function<void()>([&](){
        printf("called\n");
    }));
    notifier.Notify(void_notification);
    std::cout << "Hello, World!\n";
    return 0;
}
