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
    notifier.notify(hello_notification, 1);
    notifier.AddObserver(hello_notification,
                         std::function([&](int a){
                             printf("(1)called: %d\n",a);
                         }));
    notifier.AddObserver(hello_notification,
                         std::function([&](int a){
                             printf("(2)called: %d\n",a);
                         }));

    notifier.notify(hello_notification, 1);

    auto void_notification = Notification<void>("VOID");
    notifier.AddObserver(void_notification, std::function<void()>([&](){
        printf("called\n");
    }));
    notifier.notify(void_notification);
    std::cout << "Hello, World!\n";
    return 0;
}
