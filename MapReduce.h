#pragma once
#include <algorithm>

template<typename T>
class MapReduce final {
    std::vector<T> parts_;

public:
    explicit MapReduce(std::vector<T> parts) : parts_(std::move(parts)) {
    }

    template<typename U, typename F>
    MapReduce<U> transform(F f) {
        std::vector<U> result{parts_.size()};
        std::transform(this->parts_.cbegin(), this->parts_.cend(), result.begin(), [&f](const T& t){ return f(t);});
        return MapReduce<U>{result};
    }

    T get_result() const {
        return std::ranges::fold_left(parts_.cbegin(), parts_.cend(), T::default_value(),
                                      [](const T &t1, const T &t2) { return t1.combine(t2); });
    }
};
