//
//  any.hpp
//
//  Created by Hirohiyo Kato
//

#pragma once

/// A variant type holder by using type erasure technique.
class Any {
private:
    // non template base class
    struct _AnyBase {
        virtual ~_AnyBase() {}
        virtual const std::type_info& type() const = 0;
        virtual _AnyBase *clone() const = 0;
    };

    template <typename T>
    struct _Any: public _AnyBase {
        T value_;

        _Any(T value): value_(value) {}

        const std::type_info& type() const {
            return typeid(T);
        }

        _AnyBase* clone() const {
            return new _Any<T>(value_);
        }
    };

    _AnyBase* obj_;

public:
    template <typename T>
    Any(const T& value): obj_(new _Any<T>(value)) {}

    Any(const Any& obj) {
        if (obj.obj_) {
            obj_ = obj.obj_->clone();
        } else {
            obj_ = 0;
        }
    }

    Any& operator=(const Any& obj) {
        delete obj_;
        if (obj.obj_) {
            obj_ = obj.obj_->clone();
        } else {
            obj_ = 0;
        }
        return *this;
    }

    template <typename T>
    Any& operator=(const T& value) {
        delete obj_;
        obj_ = new _Any<T>(value);
        return *this;
    }

    template <typename T>
    const T& any_cast() const {
        return dynamic_cast<_Any<T>&>(*obj_).value_;
    }

    const std::type_info& type() const {
        return obj_->type();
    }

    ~Any() {
        delete obj_;
    }
};
