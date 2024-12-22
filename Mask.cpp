#include <vector>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <iterator>

template <std::size_t N>
class Mask {
    static_assert(N > 0, "Size of mask must be greater than 0");

private:
    std::vector<bool> mask;

public:
    Mask(std::initializer_list<bool> init) {
        if (init.size() != N) {
            throw std::invalid_argument("Mask size does not match template parameter");
        }
        mask.assign(init);
    }

    constexpr std::size_t size() const noexcept {
        return N;
    }

    bool at(std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return mask[index];
    }

    template <typename Container>
    void slice(Container& container) const {
        static_assert(std::is_same_v<
                        typename std::remove_const<typename std::remove_reference<decltype(container.at(0))>::type>::type,
                        typename Container::value_type>,
                    "Container must support at method");

        Container result;
        std::size_t mask_index = 0;

        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask[mask_index]) {
                result.push_back(container.at(i));
            }
            mask_index = (mask_index + 1) % N;
        }

        container = std::move(result);
    }


    template <typename Container, typename Func>
    Container transform(const Container& container, Func func) const {
        static_assert(std::is_same_v<
                        typename std::remove_const<typename std::remove_reference<decltype(container.at(0))>::type>::type,
                        typename Container::value_type>,
                    "Container must support at method");

        Container result = container;
        std::size_t mask_index = 0;

        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask[mask_index]) {
                result[i] = func(container.at(i));
            }
            mask_index = (mask_index + 1) % N;
        }

        return result;
    }

    template <typename Container, typename Func>
    Container slice_and_transform(const Container& container, Func func) const {
        static_assert(std::is_same_v<
                        typename std::remove_const<typename std::remove_reference<decltype(container.at(0))>::type>::type,
                        typename Container::value_type>,
                    "Container must support at method");

        Container result;
        std::size_t mask_index = 0;

        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask[mask_index]) {
                result.push_back(func(container.at(i)));
            }
            mask_index = (mask_index + 1) % N;
        }

        return result;
    }

};
