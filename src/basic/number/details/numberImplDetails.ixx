module;
module original.basic.number.numberImpl:details;
import original.basic.types;


namespace original::details
{
    /**
     * @brief Base class providing common functionality for strongly-typed numbers.
     * This class is intended for internal use only and stores the underlying value.
     * @tparam T Underlying arithmetic type satisfying StdArithmetic.
     */
    template<StdArithmetic T>
    class Number
    {
    protected:
        T value_{};

        constexpr Number() noexcept = default;

        /**
         * @brief Constructs from an underlying value.
         */
        explicit constexpr Number(T value) noexcept : value_(value) {}

        constexpr Number(const Number&) noexcept = default;
        constexpr Number& operator=(const Number&) noexcept = default;
        constexpr Number(Number&&) noexcept = default;
        constexpr Number& operator=(Number&&) noexcept = default;

    public:
        /**
         * @brief Returns the underlying value.
         */
        [[nodiscard]] constexpr T value() const noexcept { return this->value_; }
    };
}