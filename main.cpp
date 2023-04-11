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
        notifier.RemoveObserver(token2);
        notifier.Notify(HelloNotification, 3);
        std::cout << "----------------------\n";

        notifier.AddObserver(VoidNotification, std::function<void()>([&](){
            printf("called\n");
        }));
        notifier.Notify(VoidNotification);
    }

    return 0;
}
