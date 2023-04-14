# SimpleNotifier

C++ library for a notification dispatch mechanism that enables the broadcast of information to registered observers.

## Install

Copy header 2 files(`simple_notifier.hpp` & `_simple_notifier.hpp`) to your folder.

## How to use

```cpp
// Include the header file.
#include "simple_notifier.hpp"

// A notification object that it can handle with one integer value.
const auto HelloNotification = Notification<int>("Hello SimpleNotifier");

// A notifier object to manage registrations.
Notifier sharedNotifier;

// Register a callback for the notification, via the notifier.
auto token = notifier.add_observer(
    HelloNotification,
    std::function([&](int a){ printf("Received the notification: %d\n",a); })
);

// Post a notification with an associated value.
// -> "Received the notification: 193" displays.
notifier.notify(HelloNotification, 193);

// Remove observation from the notifier object.
delete token; or
notifier.remove_observer(token);
```

1. Include "simple_notifier.hpp" header file.
1. Create shared `Notificatoin` object with idenfier string.
1. Prepare shared `Notifier` object. Keep the object within your project.
1. Register a callback to the notifier object.
1. Post a notification with an associated value.
1. (Unobserve the notification)

## License

[MIT License](https://github.com/hirohitokato/SimpleNotifier/blob/main/LICENSE).
