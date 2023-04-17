//
//  SimpleNotifier
//
//  Created by Hirohiyo Kato
//

#include <iostream>
#include <cstdio>
#include <memory>
#include "simple_notifier.hpp"

using namespace SimpleNotifier;

static const auto HelloNotification = Notification<int>("Hello");
static const auto VoidNotification = Notification<void>("Non Argument");

Notifier sharedNotifier;

class Functor {
public:
    Functor(int x): x_(x) {}
    void operator()(int a) {
        std::cout << "Called " << x_ << " with " << a << ".\n";
    }
private:
    int x_ = 0;
};

int main(int argc, const char * argv[]) {
    {
        Notifier notifier;


        notifier.notify(HelloNotification, 1);
        auto token1 = notifier.add_observer(HelloNotification,
                             std::function([&](int a){
                                 printf("(1-1) called: %d\n",a);
                             }));
        auto token2 = notifier.add_observer(HelloNotification,
                             std::function([&](int a){
                                 printf("(1-2) called: %d\n",a);
                             }));
        notifier.notify(HelloNotification, 1);
        std::cout << "----------------------\n";
        {
            Functor f(3);
            auto token = notifier.add_observer(HelloNotification,
                                               std::function([&](int a){f(a);}));
        }
        std::cout << "----------------------\n";
        {

            auto token = std::shared_ptr<NotificationToken>(notifier.add_observer(HelloNotification,
                                 std::function([&](int a){
                                     printf("(2-1) called: %d\n",a);
                                 })));
            notifier.notify(HelloNotification, 2);
        }
        notifier.notify(HelloNotification, 3);
        std::cout << "----------------------\n";
        notifier.remove_observer(token2);
        notifier.notify(HelloNotification, 4);
        std::cout << "----------------------\n";


        notifier.add_observer(VoidNotification, std::function<void()>([&](){
            printf("called\n");
        }));
        notifier.notify(VoidNotification);
    }

    return 0;
}
