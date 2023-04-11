//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <iostream>
#include <cstdio>
#include "simple_notifier.hpp"

static const auto HelloNotification = Notification<int>("Hello");
static const auto VoidNotification = Notification<void>("Non Argument");

int main(int argc, const char * argv[]) {
    
    {
        Notifier notifier;

        notifier.notify(HelloNotification, 1);
        auto token1 = notifier.add_observer(HelloNotification,
                             std::function([&](int a){
                                 printf("(1) called: %d\n",a);
                             }));
        auto token2 = notifier.add_observer(HelloNotification,
                             std::function([&](int a){
                                 printf("(2) called: %d\n",a);
                             }));

        notifier.notify(HelloNotification, 1);
        std::cout << "----------------------\n";
//        delete token1;
        notifier.notify(HelloNotification, 2);
        std::cout << "----------------------\n";
        notifier.remove_observer(token2);
        notifier.notify(HelloNotification, 3);
        std::cout << "----------------------\n";

        notifier.add_observer(VoidNotification, std::function<void()>([&](){
            printf("called\n");
        }));
        notifier.notify(VoidNotification);
    }

    return 0;
}
