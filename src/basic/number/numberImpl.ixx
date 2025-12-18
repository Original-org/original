module;
#include <compare>
#include <cstdint>
export module original.basic.number.numberImpl;
import original.basic.types;


namespace original
{
    namespace details
    {
        template<StdArithmetic T>
        class Number
        {
        protected:
            T value_{};

            explicit constexpr Number(T value) noexcept : value_(value) {}

            constexpr Number(const Number&) noexcept = default;
            constexpr Number& operator=(const Number&) noexcept = default;
            constexpr Number(Number&&) noexcept = default;
            constexpr Number& operator=(Number&&) noexcept = default;

        public:
            [[nodiscard]] constexpr T value() const noexcept { return this->value_; }
        };
    }

    export template<StdIntegral T>
    class Integer final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        using Type = T;

        explicit constexpr Integer(T value) noexcept : Base(value) {}

        template<StdIntegral U>
        requires (sizeof(U) <= sizeof(T))
        explicit constexpr Integer(U value) noexcept : Base(static_cast<T>(value)) {}

        explicit constexpr operator T() const noexcept
        {
            return this->value_;
        }

        explicit constexpr operator bool() const noexcept
        {
            return this->value_ != 0;
        }

        constexpr std::strong_ordering operator<=>(const Integer& rhs) const noexcept
        {
            return this->value_ <=> rhs.value();
        }

        constexpr bool operator==(const Integer& rhs) const noexcept
        {
            return this->value_ == rhs.value();
        }

        constexpr std::strong_ordering operator<=>(T rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        constexpr bool operator==(T rhs) const noexcept
        {
            return this->value_ == rhs;
        }

        constexpr Integer& operator+=(Integer rhs) noexcept
        {
            this->value_ += rhs.value();
            return *this;
        }

        constexpr Integer& operator+=(T rhs) noexcept
        {
            return *this += Integer{rhs};
        }

        constexpr Integer& operator-=(Integer rhs) noexcept
        {
            this->value_ -= rhs.value();
            return *this;
        }

        constexpr Integer& operator-=(T rhs) noexcept
        {
            return *this -= Integer{rhs};
        }

        constexpr Integer& operator*=(Integer rhs) noexcept
        {
            this->value_ *= rhs.value();
            return *this;
        }

        constexpr Integer& operator*=(T rhs) noexcept
        {
            return *this *= Integer{rhs};
        }

        constexpr Integer& operator/=(Integer rhs)
        {
            this->value_ /= rhs.value();
            return *this;
        }

        constexpr Integer& operator/=(T rhs)
        {
            return *this /= Integer{rhs};
        }

        constexpr Integer& operator%=(Integer rhs)
        {
            this->value_ %= rhs.value();
            return *this;
        }

        constexpr Integer& operator%=(T rhs)
        {
            return *this %= Integer{rhs};
        }

        constexpr Integer& operator++() noexcept
        {
            ++this->value_;
            return *this;
        }

        constexpr Integer& operator--() noexcept
        {
            --this->value_;
            return *this;
        }

        constexpr Integer operator++(int) noexcept
        {
            Integer tmp{*this};
            this->value_ += 1;
            return tmp;
        }

        constexpr Integer operator--(int) noexcept
        {
            Integer tmp{*this};
            this->value_ -= 1;
            return tmp;
        }

        constexpr Integer operator+(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp += rhs;
            return tmp;
        }

        constexpr Integer operator+(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp += rhs;
            return tmp;
        }

        constexpr Integer operator-(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        constexpr Integer operator-(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        constexpr Integer operator*(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        constexpr Integer operator*(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp *= rhs;
            return tmp;
        }


        constexpr Integer operator/(Integer rhs) const
        {
            Integer tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        constexpr Integer operator/(T rhs) const
        {
            Integer tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        constexpr Integer operator%(Integer rhs) const
        {
            Integer tmp{*this};
            tmp %= rhs;
            return tmp;
        }

        constexpr Integer operator%(T rhs) const
        {
            Integer tmp{*this};
            tmp %= rhs;
            return tmp;
        }

        constexpr Integer& operator<<=(std::size_t shift) noexcept
        {
            this->value_ <<= shift;
            return *this;
        }

        constexpr Integer& operator>>=(std::size_t shift) noexcept
        {
            this->value_ >>= shift;
            return *this;
        }

        constexpr Integer operator<<(std::size_t shift) const noexcept
        {
            Integer tmp{*this};
            tmp <<= shift;
            return tmp;
        }

        constexpr Integer operator>>(std::size_t shift) const noexcept
        {
            Integer tmp{*this};
            tmp >>= shift;
            return tmp;
        }

        constexpr Integer& operator&=(Integer rhs) noexcept
        {
            this->value_ &= rhs.value();
            return *this;
        }

        constexpr Integer& operator&=(T rhs) noexcept
        {
            this->value_ &= rhs;
            return *this;
        }

        constexpr Integer& operator|=(Integer rhs) noexcept
        {
            this->value_ |= rhs.value();
            return *this;
        }

        constexpr Integer& operator|=(T rhs) noexcept
        {
            this->value_ |= rhs;
            return *this;
        }

        constexpr Integer& operator^=(Integer rhs) noexcept
        {
            this->value_ ^= rhs.value();
            return *this;
        }

        constexpr Integer& operator^=(T rhs) noexcept
        {
            this->value_ ^= rhs;
            return *this;
        }

        constexpr Integer operator&(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp &= rhs;
            return tmp;
        }

        constexpr Integer operator&(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp &= rhs;
            return tmp;
        }

        constexpr Integer operator|(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp |= rhs;
            return tmp;
        }

        constexpr Integer operator|(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp |= rhs;
            return tmp;
        }

        constexpr Integer operator^(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp ^= rhs;
            return tmp;
        }

        constexpr Integer operator^(T rhs) const noexcept
        {
            Integer tmp{*this};
            tmp ^= rhs;
            return tmp;
        }

        constexpr Integer operator~() const noexcept
        {
            return Integer{static_cast<T>(~this->value_)};
        }

        constexpr Integer operator+() const noexcept
        {
            return *this;
        }

        constexpr Integer operator-() const noexcept
        requires StdSignedIntegral<T>
        {
            return Integer{static_cast<T>(-this->value_)};
        }
    };

    export template<StdFloating T>
    class Floating final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        using Type = T;

        explicit constexpr Floating(T value) noexcept : Base(value) {}

        template<StdFloating U>
        requires (sizeof(U) <= sizeof(T))
        explicit constexpr Floating(U value) noexcept : Base(static_cast<T>(value)) {}

        explicit constexpr operator T() const noexcept
        {
            return this->value_;
        }

        explicit constexpr operator bool() const noexcept
        {
            return this->value_ != 0;
        }

        constexpr std::partial_ordering operator<=>(const Floating& rhs) const noexcept
        {
            return this->value_ <=> rhs.value();
        }

        constexpr bool operator==(const Floating& rhs) const noexcept
        {
            return this->value_ == rhs.value();
        }

        constexpr std::partial_ordering operator<=>(T rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        constexpr Floating& operator+=(Floating rhs) noexcept
        {
            this->value_ += rhs.value();
            return *this;
        }

        constexpr Floating& operator+=(T rhs) noexcept
        {
            this->value_ += rhs;
            return *this;
        }

        constexpr Floating& operator-=(Floating rhs) noexcept
        {
            this->value_ -= rhs.value();
            return *this;
        }

        constexpr Floating& operator-=(T rhs) noexcept
        {
            this->value_ -= rhs;
            return *this;
        }

        constexpr Floating& operator*=(Floating rhs) noexcept
        {
            this->value_ *= rhs.value();
            return *this;
        }

        constexpr Floating& operator*=(T rhs) noexcept
        {
            this->value_ *= rhs;
            return *this;
        }

        constexpr Floating& operator/=(Floating rhs)
        {
            this->value_ /= rhs.value();
            return *this;
        }

        constexpr Floating& operator/=(T rhs)
        {
            this->value_ /= rhs;
            return *this;
        }

        constexpr Floating& operator++() noexcept
        {
            ++this->value_;
            return *this;
        }

        constexpr Floating& operator--() noexcept
        {
            --this->value_;
            return *this;
        }

        constexpr Floating operator++(int) noexcept
        {
            Floating tmp{*this};
            ++this->value_;
            return tmp;
        }

        constexpr Floating operator--(int) noexcept
        {
            Floating tmp{*this};
            --this->value_;
            return tmp;
        }

        constexpr Floating operator+() const noexcept
        {
            return *this;
        }

        constexpr Floating operator-() const noexcept
        {
            return Floating{-this->value_};
        }

        constexpr Floating operator+(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        constexpr Floating operator+(T rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        constexpr Floating operator-(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        constexpr Floating operator-(T rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        constexpr Floating operator*(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        constexpr Floating operator*(T rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        constexpr Floating operator/(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        constexpr Floating operator/(T rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }
    };

    export using i8 = Integer<std::int8_t>;
    export using i16 = Integer<std::int16_t>;
    export using i32 = Integer<std::int32_t>;
    export using i64 = Integer<std::int64_t>;
    export using u8 = Integer<std::uint8_t>;
    export using u16 = Integer<std::uint16_t>;
    export using u32 = Integer<std::uint32_t>;
    export using u64 = Integer<std::uint64_t>;
    export using byte = u8;
    export using f32 = Floating<float>;
    export using f64 = Floating<double>;
    export using f80 = Floating<long double>;

    export namespace literals
    {
        constexpr i32 operator""_i32(const unsigned long long v)
        {
            return i32{static_cast<std::int32_t>(v)};
        }

        constexpr i64 operator""_i64(const unsigned long long v)
        {
            return i64{static_cast<std::int64_t>(v)};
        }

        constexpr u32 operator""_u32(const unsigned long long v)
        {
            return u32{static_cast<std::uint32_t>(v)};
        }

        constexpr u64 operator""_u64(const unsigned long long v)
        {
            return u64{v};
        }

        constexpr f32 operator""_f32(const long double v)
        {
            return f32{static_cast<float>(v)};
        }

        constexpr f64 operator""_f64(const long double v)
        {
            return f64{static_cast<double>(v)};
        }

        constexpr f80 operator""_f80(const long double v)
        {
            return f80{v};
        }
    }

    export template<StdIntegral To, StdIntegral From>
    requires (sizeof(To) < sizeof(From))
    [[nodiscard]]
    constexpr Integer<To> narrowCast(const Integer<From>& i) noexcept
    {
        return Integer<To>{static_cast<To>(i.value())};
    }

    export [[nodiscard]]
    constexpr u64 fromStdSize(const std::size_t n) noexcept
    {
        return u64{static_cast<std::uint64_t>(n)};
    }

    export constexpr std::size_t toStdSize(const u64& u)
    {
        return u.value();
    }
}
