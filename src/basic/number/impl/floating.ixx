module;
#include <type_traits>
#include <stdexcept>
#include <compare>
export module original.basic.number.impl:floating;
import :details;


export namespace original
{
    /**
     * @class Floating
     * @brief Strongly-typed wrapper around floating-point types providing type safety and consistent interface.
     * Floating behaves like the underlying floating-point type but prevents implicit mixing with other types.
     * @tparam T Underlying floating-point type (must satisfy StdFloating).
     */
    template<StdFloating T>
    class Floating final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        /**
         * @brief The underlying floating-point type.
         */
        using Type = T;

        /** @brief Default constructor. */
        constexpr Floating() noexcept = default;

        /**
         * @brief Converting constructor from a smaller floating-point type.
         * @tparam U Source floating-point type.
         * @param value The value to convert and wrap.
         */
        template<StdFloating U>
        requires (sizeof(U) <= sizeof(T))
        explicit constexpr Floating(U value) noexcept : Base(static_cast<T>(value)) {}

        /**
         * @brief Explicit conversion to the underlying type.
         */
        explicit constexpr operator T() const noexcept
        {
            return this->value_;
        }

        /**
         * @brief Explicit conversion to bool (non-zero check).
         */
        explicit constexpr operator bool() const noexcept
        {
            return this->value_ != 0;
        }

        /**
         * @brief Three-way comparison with another Floating.
         * Uses std::partial_ordering due to possible NaN values.
         * @param rhs Right-hand side operand.
         * @return Partial ordering result.
         */
        constexpr std::partial_ordering operator<=>(const Floating& rhs) const noexcept
        {
            return this->value_ <=> rhs.value();
        }

        /**
         * @brief Equality comparison with another Floating.
         * @param rhs Right-hand side operand.
         * @return true if values are equal.
         */
        constexpr bool operator==(const Floating& rhs) const noexcept
        {
            return this->value_ == rhs.value();
        }

        /**
         * @brief Three-way comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return Partial ordering result.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr std::partial_ordering operator<=>(U rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        /**
         * @brief Equality comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return true if values are equal.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr bool operator==(U rhs) const noexcept
        {
            return this->value_ == rhs;
        }

        /**
         * @brief Compound addition assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator+=(Floating rhs) noexcept
        {
            this->value_ += rhs.value();
            return *this;
        }

        /**
         * @brief Compound addition assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator+=(U rhs) noexcept
        {
            this->value_ += rhs;
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator-=(Floating rhs) noexcept
        {
            this->value_ -= rhs.value();
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator-=(U rhs) noexcept
        {
            this->value_ -= rhs;
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator*=(Floating rhs) noexcept
        {
            this->value_ *= rhs.value();
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator*=(U rhs) noexcept
        {
            this->value_ *= rhs;
            return *this;
        }

        /**
         * @brief Compound division assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator/=(Floating rhs)
        {
            this->value_ /= rhs.value();
            return *this;
        }

        /**
         * @brief Compound division assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator/=(U rhs)
        {
            this->value_ /= rhs;
            return *this;
        }

        /**
         * @brief Prefix increment.
         * @return Reference to this Floating after increment.
         */
        constexpr Floating& operator++() noexcept
        {
            ++this->value_;
            return *this;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to this Floating after decrement.
         */
        constexpr Floating& operator--() noexcept
        {
            --this->value_;
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @param postfix Dummy parameter to distinguish from prefix increment.
         * @return Copy of this Floating before increment.
         */
        constexpr Floating operator++(int postfix) noexcept
        {
            Floating tmp{*this};
            ++this->value_;
            return tmp;
        }

        /**
         * @brief Postfix decrement.
         * @param postfix Dummy parameter to distinguish from prefix decrement.
         * @return Copy of this Floating before decrement.
         */
        constexpr Floating operator--(int postfix) noexcept
        {
            Floating tmp{*this};
            --this->value_;
            return tmp;
        }

        /**
         * @brief Unary plus operator.
         * @return Copy of this Floating.
         */
        constexpr Floating operator+() const noexcept
        {
            return *this;
        }

        /**
         * @brief Unary minus operator.
         * @return Floating with negated value.
         */
        constexpr Floating operator-() const noexcept
        {
            return Floating{-this->value_};
        }

        /**
         * @brief Addition with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the sum.
         */
        constexpr Floating operator+(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Addition with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the sum.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator+(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the difference.
         */
        constexpr Floating operator-(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the difference.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator-(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the product.
         */
        constexpr Floating operator*(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the product.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator*(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Division with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the quotient.
         */
        constexpr Floating operator/(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        /**
         * @brief Division with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the quotient.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator/(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }
    };

        /**
     * @brief Addition operator for mixed Floating types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Floating of appropriate size containing the sum.
     */
    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr auto operator+(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = static_cast<To>(lhs.value()) + static_cast<To>(rhs.value());
        return Floating<To>{result};
    }

    /**
     * @brief Subtraction operator for mixed Floating types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Floating of appropriate size containing the difference.
     */
    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr auto operator-(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = static_cast<To>(lhs.value()) - static_cast<To>(rhs.value());
        return Floating<To>{result};
    }

    /**
     * @brief Multiplication operator for mixed Floating types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Floating of appropriate size containing the product.
     */
    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr auto operator*(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = static_cast<To>(lhs.value()) * static_cast<To>(rhs.value());
        return Floating<To>{result};
    }

    /**
     * @brief Division operator for mixed Floating types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Floating of appropriate size containing the quotient.
     */
    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr auto operator/(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = static_cast<To>(lhs.value()) / static_cast<To>(rhs.value());
        return Floating<To>{result};
    }

    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr bool operator==(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        return static_cast<To>(lhs.value()) == static_cast<To>(rhs.value());
    }

    template<StdFloating U, StdFloating V>
    requires (!SameType<U, V>)
    constexpr std::partial_ordering operator<=>(Floating<U> lhs, Floating<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        return static_cast<To>(lhs.value()) <=> static_cast<To>(rhs.value());
    }

    namespace literals
    {
        /**
         * @brief User-defined literal for creating F32 (single-precision floating-point) values.
         * @param v Long double literal value.
         * @return F32 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range result in compilation errors.
         */
        consteval F32 operator""_f32(const long double v)
        {
            using Type = float;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F32 literal out of range"};

            return F32{static_cast<Type>(v)};
        }

        /**
         * @brief User-defined literal for creating F64 (double-precision floating-point) values.
         * @param v Long double literal value.
         * @return F64 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range result in compilation errors.
         */
        consteval F64 operator""_f64(const long double v)
        {
            using Type = double;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F64 literal out of range"};

            return F64{static_cast<Type>(v)};
        }

        /**
         * @brief User-defined literal for creating F80 (extended-precision floating-point) values.
         * @param v Long double literal value.
         * @return F80 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range result in compilation errors.
         */
        consteval F80 operator""_f80(const long double v)
        {
            using Type = long double;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F80 literal out of range"};

            return F80{static_cast<Type>(v)};
        }

        /**
         * @brief User-defined literal for creating exact F32 values with precision checking.
         * @param v Long double literal value.
         * @return F32 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range or loses precision result in compilation errors.
         */
        consteval F32 operator""_f32e(const long double v)
        {
            using Type = float;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F32 literal out of range"};
            if (v != static_cast<Type>(v))
                throw std::invalid_argument{"F32 loss precision"};

            return F32{static_cast<Type>(v)};
        }

        /**
         * @brief User-defined literal for creating exact F64 values with precision checking.
         * @param v Long double literal value.
         * @return F64 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range or loses precision result in compilation errors.
         */
        consteval F64 operator""_f64e(const long double v)
        {
            using Type = double;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F64 literal out of range"};
            if (v != static_cast<Type>(v))
                throw std::invalid_argument{"F64 loss precision"};

            return F64{static_cast<Type>(v)};
        }

        /**
         * @brief User-defined literal for creating exact F80 values with precision checking.
         * @param v Long double literal value.
         * @return F80 instance with the given value.
         * @note This is a consteval function that performs compile-time validation.
         *       Values out of range or loses precision result in compilation errors.
         */
        consteval F80 operator""_f80e(const long double v)
        {
            using Type = long double;
            if (v > std::numeric_limits<Type>::max() ||
                v < -std::numeric_limits<Type>::max())
                throw std::out_of_range{"F80 literal out of range"};
            if (v != static_cast<Type>(v))
                throw std::invalid_argument{"F80 loss precision"};

            return F80{static_cast<Type>(v)};
        }
    }
}