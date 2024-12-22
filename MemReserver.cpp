#include <iostream>
#include <stdexcept>
#include <memory>
#include <optional>

class NotEnoughSlotsError : public std::exception {
    std::string message;
public:
    NotEnoughSlotsError(size_t created) {
        message = "Not enough slots available. Already created: " + std::to_string(created);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class EmptySlotError : public std::exception {
    std::string message;
public:
    EmptySlotError(size_t position) {
        message = "Slot at position " + std::to_string(position) + " is empty.";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ObjectNotFoundError : public std::exception {
    std::string message;
public:
    ObjectNotFoundError() {
        message = "Object not found in storage.";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

template <typename T, size_t N>
class MemReserver {
private:
    struct Slot {
        std::optional<T> object;
    };

    Slot slots[N];
    size_t count_objects = 0;

public:
    template <typename... Args>
    T& create(Args&&... args) {
        if (count_objects >= N) {
            throw NotEnoughSlotsError(count_objects);
        }

        for (size_t i = 0; i < N; ++i) {
            if (!slots[i].object.has_value()) {
                slots[i].object.emplace(std::forward<Args>(args)...);
                ++count_objects;
                return *slots[i].object;
            }
        }

        throw std::runtime_error("Unexpected error in create method.");
    }

    void remove(size_t position) {
        if (position >= N || !slots[position].object.has_value()) {
            throw EmptySlotError(position);
        }

        slots[position].object.reset();
        --count_objects;
    }

    size_t count() const {
        return count_objects;
    }

    T& get(size_t position) {
        if (position >= N || !slots[position].object.has_value()) {
            throw EmptySlotError(position);
        }

        return *slots[position].object;
    }

    size_t position(const T& object) const {
        for (size_t i = 0; i < N; ++i) {
            if (slots[i].object.has_value() && &object == &(*slots[i].object)) {
                return i;
            }
        }

        throw ObjectNotFoundError();
    }
};
