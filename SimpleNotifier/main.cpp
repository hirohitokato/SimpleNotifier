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
    notifier.AddObserver(notify,
                         std::function([&](int a){
                             printf("called: %d\n",a);
                         }));
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
