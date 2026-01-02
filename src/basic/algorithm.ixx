module;
#include <utility>
export module original.basic.algorithm;
import original.basic.types;
import original.basic.range;
import original.basic.iterator;
import original.basic.container;


export namespace original::algorithms
{
    struct EqualTo;
    struct ThreeWayCompare;
    struct LessThan;
    struct GreaterThan;
}

namespace original::details
{
    /**
     * @brief Internal implementation of mismatch for two bounded ranges.
     *
     * Finds the first position where the two ranges differ using the provided predicate.
     * Stops at the end of either range.
     *
     * @tparam Iter1    Iterator type for the first range.
     * @tparam Iter2    Iterator type for the second range.
     * @tparam Pred     Predicate type (defaults to algorithms::EqualTo).
     * @param first1    Beginning of the first range.
     * @param last1     End of the first range.
     * @param first2    Beginning of the second range.
     * @param last2     End of the second range.
     * @param pred      Predicate used to compare elements.
     * @return Pair of iterators pointing to the first mismatching elements or the ends.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = algorithms::EqualTo>
    std::pair<Iter1, Iter2> mismatchImpl(Iter1 first1, Iter1 last1,
                                         Iter2 first2, Iter2 last2, Pred pred)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;
        while (first1 != last1 && first2 != last2)
        {
            const auto& type1 = static_cast<const CommonRef&>(*first1);
            if (const auto& type2 = static_cast<const CommonRef&>(*first2); !pred(type1, type2))
            {
                return {first1, first2};
            }
            ++first1;
            ++first2;
        }
        return {first1, first2};
    }

    /**
     * @brief Internal implementation of mismatch for a bounded first range and an unbounded second range.
     *
     * Finds the first position where the two ranges differ using the provided predicate.
     * Assumes the second range is at least as long as the first.
     *
     * @tparam Iter1    Iterator type for the first range.
     * @tparam Iter2    Iterator type for the second range.
     * @tparam Pred     Predicate type (defaults to algorithms::EqualTo).
     * @param first1    Beginning of the first range.
     * @param last1     End of the first range.
     * @param first2    Beginning of the second range.
     * @param pred      Predicate used to compare elements.
     * @return Pair of iterators pointing to the first mismatching elements or {last1, advanced first2}.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = algorithms::EqualTo>
    std::pair<Iter1, Iter2> mismatchImpl(Iter1 first1, Iter1 last1,
                                          Iter2 first2, Pred pred)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;

        while (first1 != last1) {
            const auto& ref1 = static_cast<const CommonRef&>(*first1);
            if (const auto& ref2 = static_cast<const CommonRef&>(*first2); !pred(ref1, ref2)) {
                return {first1, first2};
            }
            ++first1;
            ++first2;
        }
        return {first1, first2};
    }
}

export namespace original::algorithms {

    /**
     * @brief Default equality comparator.
     *
     * Uses common type conversion and `operator==` to compare two values.
     * Suitable as the default predicate for equality-based algorithms.
     */
    struct EqualTo
    {
        /**
         * @brief Compares two values for equality after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value to compare.
         * @param rhs  Right-hand side value to compare.
         * @return true if the converted values are equal, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && EqualityComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs == type_rhs;
        }
    };

    /**
     * @brief Default three-way comparator.
     *
     * Converts operands to their common type and returns the result of
     * the three-way comparison (`<=>`). This is used by lexicographical
     * comparison helpers that rely on a three-way ordering.
     */
    struct ThreeWayCompare
    {
        /**
         * @brief Performs three-way comparison after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return std::strong_ordering (less, equal, or greater) reflecting the relationship.
         */
        template<typename T, typename U = T>
        requires HasCommonType<T, U> && ThreeWayComparable<CommonType<T, U>>
        constexpr auto operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs <=> type_rhs;
        }
    };

    /**
     * @brief Default less-than comparator.
     *
     * Compares two values after converting them to a common type using
     * `operator<`.
     */
    struct LessThan
    {
        /**
         * @brief Compares two values using less-than after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return true if lhs is less than rhs after conversion, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && LessComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs < type_rhs;
        }
    };

    /**
     * @brief Default greater-than comparator.
     *
     * Implemented in terms of `LessThan` (i.e. `GreaterThan{}(a,b)` is
     * equivalent to `LessThan{}(b,a)`).
     */
    struct GreaterThan
    {
        /**
         * @brief Compares two values using greater-than (delegates to LessThan).
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return true if lhs is greater than rhs, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && LessComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            return LessThan{}(rhs, lhs);
        }
    };

    /**
     * @brief Finds the first position where two iterator ranges differ.
     *
     * Compares elements from `[first1, last1)` with elements starting at
     * `first2` (and up to `last2`) using the predicate `pred`. Returns a
     * pair of iterators pointing to the first mismatching elements (or the
     * end positions if no mismatch was found).
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Comparator type, defaults to `EqualTo`.
     * @param first1 Begin of first range.
     * @param last1 End of first range.
     * @param first2 Begin of second range.
     * @param last2 End of second range.
     * @param pred Comparator used to check equality (or custom condition).
     * @return Pair of iterators to the first non-matching elements.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = EqualTo>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                          typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>,
                        CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>>
    std::pair<Iter1, Iter2> mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Pred pred = {})
    {
        return details::mismatchImpl(first1, last1, first2, last2, pred);
    }

    /**
     * @brief Finds the first mismatch between a bounded and an unbounded range.
     *
     * Compares elements from `[first1, last1)` with elements starting at
     * `first2` using `pred`. The second range is assumed to be at least as
     * long as the first; the function returns the pair of iterators where a
     * mismatch occurs or `{last1, corresponding_first2}` when finished.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Comparator type, defaults to `EqualTo`.
     * @param first1 Begin of first range.
     * @param last1 End of first range.
     * @param first2 Begin of second range.
     * @param pred Comparator used to check equality (or custom condition).
     * @return Pair of iterators to the first non-matching elements.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = EqualTo>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                          typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>>
    std::pair<Iter1, Iter2> mismatch(Iter1 first1, Iter1 last1,
                                     Iter2 first2, Pred pred = {})
    {
        return details::mismatchImpl(first1, last1, first2, pred);
    }

    /**
     * @brief Finds the first mismatch between two ranges.
     *
     * Iterates both ranges in lock-step and returns a pair of iterators
     * pointing to the first elements that do not satisfy `pred`. If both
     * ranges are equal (and have the same length), the returned iterators
     * will be the respective `end()` iterators of the ranges.
     *
     * @tparam Range1 Type of the first range.
     * @tparam Range2 Type of the second range.
     * @tparam Pred Comparator type, defaults to `EqualTo`.
     * @param range1 First range to compare.
     * @param range2 Second range to compare.
     * @param pred Comparator used to check equality (or custom condition).
     * @return Pair of iterators to the first non-matching elements.
     */
    template <Range Range1, Range Range2, Invokable Pred = EqualTo>
    requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                          typename RangeTraits<Range2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>>
    auto mismatch(Range1& range1, Range2& range2, Pred pred = {})
    {
        return mismatch(RangeTraits<Range1>::begin(range1),
                        RangeTraits<Range1>::end(range1),
                        RangeTraits<Range2>::begin(range2),
                        RangeTraits<Range2>::end(range2),
                        pred);
    }

    /**
     * @brief Checks if two ranges are equal with custom comparator.
     *
     * Compares elements of the first range with elements of the second range
     * using the provided comparator function. The second range must have at
     * least as many elements as the first range.
     *
     * @tparam Range1 Type of the first range.
     * @tparam Range2 Type of the second range.
     * @tparam Pred Type of the comparator function/object.
     * @param range1 The first range to compare.
     * @param range2 The second range to compare.
     * @param pred Comparator function that takes two elements and returns bool.
     * @return true if all corresponding elements satisfy pred, false otherwise.
     */
    template <Range Range1, Range Range2, Invokable Pred = EqualTo>
        requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                              typename RangeTraits<Range2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                         typename RangeTraits<Range2>::ReferenceType>,
                           CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                         typename RangeTraits<Range2>::ReferenceType>>
    bool equal(Range1 &range1, Range2 &range2, Pred pred = {})
    {
        auto&& [it1, it2] = mismatch(range1, range2, pred);

        return it1 == RangeTraits<Range1>::end(range1) &&
               it2 == RangeTraits<Range2>::end(range2);
    }

    /**
     * @brief Checks if two iterator ranges are equal with custom comparator.
     *
     * Compares elements from first1 to last1 with elements starting at first2
     * using the provided comparator function.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Type of the comparator function/object.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param pred Comparator function that takes two elements and returns bool.
     * @return true if all corresponding elements satisfy pred, false otherwise.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = EqualTo>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2, Pred pred = {})
    {
        auto&& [it1, it2] = mismatch(first1, last1, first2, pred);
        return it1 == last1;
    }

    /**
     * @brief Checks if two bounded iterator ranges are equal with custom
     * comparator.
     *
     * Compares elements from first1 to last1 with elements from first2 to last2
     * using the provided comparator function.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Type of the comparator function/object.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param last2 Iterator to the end of the second range.
     * @param pred Comparator function that takes two elements and returns bool.
     * @return true if all corresponding elements satisfy pred and ranges are same
     * size, false otherwise.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = EqualTo>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Pred pred = {})
    {
        auto&& [iter1, iter2] = mismatch(first1, last1, first2, last2, pred);
        return iter1 == last1 && iter2 == last2;
    }

    /**
     * @brief Performs lexicographically comparison of two bounded iterator ranges
     * with custom comparator.
     *
     * Compares elements from first1 to last1 with elements from first2 to last2
     * using the provided comparator function that returns a three-way comparison
     * result.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Type of the comparator function/object.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param last2 Iterator to the end of the second range.
     * @param pred Comparator function returning three-way comparison result.
     * @return Result of lexicographical comparison.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = ThreeWayCompare>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                        typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>>
    auto lexicographicallyCompare(Iter1 first1, Iter1 last1, Iter2 first2,
                                  Iter2 last2, Pred pred = {})
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;
        while (first1 != last1 && first2 != last2)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            auto& ref2 = static_cast<CommonRef>(*first2);
            if (auto cmp = pred(ref1, ref2); cmp != 0)
            {
              return cmp;
            }
            ++first1;
            ++first2;
        }

        return (first1 != last1) <=> (first2 != last2);
    }

    /**
     * @brief Performs lexicographically comparison of two ranges with custom
     * comparator.
     *
     * Compares elements of two ranges lexicographically using the provided
     * comparator function that returns a three-way comparison result.
     *
     * @tparam Range1 Type of the first range.
     * @tparam Range2 Type of the second range.
     * @tparam Pred Type of the comparator function/object returning three-way
     * result.
     * @param range1 The first range to compare.
     * @param range2 The second range to compare.
     * @param pred Comparator function returning std::strong_ordering or compatible
     * type.
     * @return Result of lexicographical comparison.
     */
    template <Range Range1, Range Range2, Invokable Pred = ThreeWayCompare>
    requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                          typename RangeTraits<Range2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>>
    auto lexicographicallyCompare(Range1 &range1, Range2 &range2, Pred pred = {})
    {
        return lexicographicallyCompare(RangeTraits<Range1>::begin(range1),
                                        RangeTraits<Range1>::end(range1),
                                        RangeTraits<Range2>::begin(range2),
                                        RangeTraits<Range2>::end(range2),
                                        pred);
    }

    /**
     * @brief Performs lexicographically comparison of two iterator ranges with
     * custom comparator.
     *
     * Compares elements from first1 to last1 with elements starting at first2
     * using the provided comparator function that returns a three-way comparison
     * result.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @tparam Pred Type of the comparator function/object.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param pred Comparator function returning three-way comparison result.
     * @return Result of lexicographical comparison.
     */
    template <Iterator Iter1, Iterator Iter2, Invokable Pred = ThreeWayCompare>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                          typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType>,
                CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType>>
    auto lexicographicallyCompare(Iter1 first1, Iter1 last1, Iter2 first2,
                                  Pred pred = {})
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;
        while (first1 != last1)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            auto& ref2 = static_cast<CommonRef>(*first2);
            if (auto cmp = pred(ref1, ref2); cmp != 0)
            {
                return cmp;
            }
            ++first1;
            ++first2;
        }

        return (first1 == last1) <=> true;
    }
} // namespace original::algorithms
