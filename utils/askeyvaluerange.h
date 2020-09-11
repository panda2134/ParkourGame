#ifndef ASKEYVALUERANGE_H
#define ASKEYVALUERANGE_H

namespace parkour {
template <typename T>
class AsKeyValueRange {
    T& container;

public:
    AsKeyValueRange(T& container_)
        : container(container_) {
    }

    auto begin() { return container.keyValueBegin(); }
    auto end() { return container.keyValueEnd(); }
};

template <typename T>
auto asKeyValueRange(T& container) {
    return AsKeyValueRange<T>(container);
}
}

#endif // ASKEYVALUERANGE_H
