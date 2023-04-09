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

    auto notify = Notification<int>("Hello");
    notifier.notify(notify, 1);
    notifier.AddObserver(notify,
                         std::function([&](int a){
                             printf("called: %d\n",a);
                         }));

    notifier.notify(notify, 1);

    std::cout << "Hello, World!\n";
    return 0;
}
