#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>

class SimpleRNG {
public:
    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = double;
        using difference_type = std::ptrdiff_t;
        using pointer = const double*;
        using reference = const double&;

        Iterator(double current, double a, double c, double m, double eps, bool is_end = false)
            : current_(current), a_(a), c_(c), m_(m), eps_(eps), start_(current), is_end_(is_end) {}

        double operator*() const {
            return current_;
        }

        Iterator& operator++() {
            current_ = std::fmod(a_ * current_ + c_, m_);
            if (std::abs(current_ - start_) < eps_) {
                is_end_ = true;
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return is_end_ != other.is_end_;
        }

    private:
        double current_;
        double a_;
        double c_;
        double m_;
        double eps_;
        double start_;
        bool is_end_;
    };

    SimpleRNG(double m, double a, double c)
        : m_(m), a_(a), c_(c), x0_(0), current_(0) {}

    void reset(double x0 = 0) {
        x0_ = x0;
        current_ = x0_;
    }

    Iterator begin() {
        return Iterator(current_, a_, c_, m_, 0.05);
    }

    Iterator end(double eps = 0.05) {
        return Iterator(current_, a_, c_, m_, eps, true);
    }

private:
    double m_;
    double a_;
    double c_;
    double x0_;
    double current_;
};
