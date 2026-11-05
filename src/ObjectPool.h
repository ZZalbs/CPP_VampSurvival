#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include <utility>

template<class T, std::size_t Capacity>
class ObjectPool {
public:
    template<class... Args>
    bool TryCreate(Args&&... args) {
        for(auto& slot:slots_) {
            if(slot) continue;
            slot.emplace(std::forward<Args>(args)...);
            ++count_;
            return true;
        }
        return false;
    }

    void Clear() {
        for(auto& slot:slots_) slot.reset();
        count_=0;
    }

    template<class Predicate>
    void ReleaseIf(Predicate predicate) {
        for(auto& slot:slots_) {
            if(slot && predicate(*slot)) {
                slot.reset();
                --count_;
            }
        }
    }

    std::size_t Size() const {
        return count_;
    }

    const std::array<std::optional<T>,Capacity>& Slots() const {
        return slots_;
    }

    template<class Function>
    void ForEach(Function function) {
        for(auto& slot:slots_) {
            if(slot) function(*slot);
        }
    }

    T& At(std::size_t index) {
        return slots_.at(index).value();
    }

private:
    std::array<std::optional<T>,Capacity> slots_{};
    std::size_t count_=0;
};