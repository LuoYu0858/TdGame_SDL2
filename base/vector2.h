// Created by LuoYu on 2026/6/12.
#pragma once

#include <complex>
#include <compare>

class Vector2 {
public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(const double x, const double y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& v) const {
        return {x + v.x, y + v.y};
    }

    void operator+=(const Vector2& v) {
        x += v.x, y += v.y;
    }

    Vector2 operator-(const Vector2& v) const {
        return {x - v.x, y - v.y};
    }

    void operator-=(const Vector2& v) {
        x -= v.x, y -= v.y;
    }

    double operator*(const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    Vector2 operator*(const double v) const {
        return {x * v, y * v};
    }

    void operator*=(const double v) {
        x *= v, y *= v;
    }

    [[nodiscard]] bool operator==(const Vector2& v) const {
        return std::abs(x - v.x) < 1e-8 && std::abs(y - v.y) < 1e-8;
    }

    [[nodiscard]] bool operator>(const Vector2& v) const {
        return this->length() > v.length();
    }

    [[nodiscard]] bool operator>=(const Vector2& v) const {
        return this->length() >= v.length();
    }

    [[nodiscard]] bool operator<(const Vector2& v) const {
        return this->length() < v.length();
    }

    [[nodiscard]] bool operator<=(const Vector2& v) const {
        return this->length() <= v.length();
    }

    [[nodiscard]] auto operator<=>(const Vector2& v) const {
        if (const auto ld = length() - v.length(); ld > 0) return std::strong_ordering::greater;
        else if (ld < 0) return std::strong_ordering::less;
        return std::strong_ordering::equal;
    }

    [[nodiscard]] double length() const {
        return std::sqrt(x * x + y * y);
    }

    [[nodiscard]] Vector2 normalize() const {
        const auto l = length();
        return l == 0 ? Vector2{0, 0} : Vector2{x / l, y / l};
    }

    [[nodiscard]] bool approx_zero() const {
        return length() < 1e-8;
    }

public:
    union {
        double v[2]{};

        struct {
            double x;
            double y;
        };
    };
};