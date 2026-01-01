module;
#include <compare>
export module original.basic.algorithm;
import original.basic.types;
import original.basic.range;
import original.basic.iterator;
import original.basic.container;

export namespace original::algorithms {
    /**
     * @brief Checks if two ranges are equal element-wise.
     *
     * Compares elements of the first range with elements of the second range
     * using operator==. The second range must have at least as many elements
     * as the first range.
     *
     * @tparam Range1 Type of the first range.
     * @tparam Range2 Type of the second range.
     * @param range1 The first range to compare.
     * @param range2 The second range to compare.
     * @return true if all elements in both ranges are equal, false otherwise.
     *
     * @note This is the simplest form that takes two ranges.
     */
    template <Range Range1, Range Range2>
        requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                              typename RangeTraits<Range2>::ReferenceType> &&
                 EqualityComparable<
                     CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                   typename RangeTraits<Range2>::ReferenceType>>
    bool equal(Range1 &range1, Range2 &range2)
    {
        auto it1 = RangeTraits<Range1>::begin(range1);
        auto it2 = RangeTraits<Range2>::begin(range2);
        auto end1 = RangeTraits<Range1>::end(range1);
        auto end2 = RangeTraits<Range2>::end(range2);
        using CommonRef = CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                        typename RangeTraits<Range2>::ReferenceType>;

        while (it1 != end1 && it2 != end2)
        {
            auto &type1 = static_cast<CommonRef>(*it1);
            auto &type2 = static_cast<CommonRef>(*it2);
            if (!(type1 == type2))
            {
                return false;
            }
            ++it1;
            ++it2;
        }

        return it1 == end1 && it2 == end2;
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
    template <Range Range1, Range Range2, Invokable Pred>
        requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                              typename RangeTraits<Range2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                         typename RangeTraits<Range2>::ReferenceType>,
                           CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                         typename RangeTraits<Range2>::ReferenceType>>
    bool equal(Range1 &range1, Range2 &range2, Pred pred)
    {
        auto it1 = RangeTraits<Range1>::begin(range1);
        auto it2 = RangeTraits<Range2>::begin(range2);
        auto end1 = RangeTraits<Range1>::end(range1);
        auto end2 = RangeTraits<Range2>::end(range2);
        using CommonRef = CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                        typename RangeTraits<Range2>::ReferenceType>;

        while (it1 != end1 && it2 != end2)
        {
            auto &type1 = static_cast<CommonRef>(*it1);
            if (auto &type2 = static_cast<CommonRef>(*it2); !pred(type1, type2))
            {
                return false;
            }
            ++it1;
            ++it2;
        }

        return it1 == end1 && it2 == end2;
    }

    /**
     * @brief Checks if two iterator ranges are equal element-wise.
     *
     * Compares elements from first1 to last1 with elements starting at first2
     * using operator==.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @return true if all elements in the first range equal corresponding elements
     *         in the second range, false otherwise.
     */
    template <Iterator Iter1, Iterator Iter2>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType> &&
                 EqualityComparable<
                     CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                   typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;

        while (first1 != last1)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            auto& ref2 = static_cast<CommonRef>(*first2);
            if (!(ref1 == ref2))
            {
                return false;
            }
            ++first1;
            ++first2;
        }

        return true;
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
    template <Iterator Iter1, Iterator Iter2, Invokable Pred>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2, Pred pred)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;

        while (first1 != last1)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            if (auto& ref2 = static_cast<CommonRef>(*first2); !pred(ref1, ref2))
            {
                return false;
            }
            ++first1;
            ++first2;
        }

        return true;
    }

    /**
     * @brief Checks if two bounded iterator ranges are equal element-wise.
     *
     * Compares elements from first1 to last1 with elements from first2 to last2
     * using operator==.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param last2 Iterator to the end of the second range.
     * @return true if both ranges are equal and same size, false otherwise.
     */
    template <Iterator Iter1, Iterator Iter2>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                                typename IterTraits<Iter2>::ReferenceType> &&
                 EqualityComparable<
                     CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                   typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
    {

        while (first1 != last1 && first2 != last2)
        {
            if (!(*first1 == *first2))
            {
                return false;
            }
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 == last2;
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
    template <Iterator Iter1, Iterator Iter2, Invokable Pred>
        requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType> &&
                 Predicate<Pred,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>,
                           CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                         typename IterTraits<Iter2>::ReferenceType>>
    bool equal(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Pred pred)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (!pred(*first1, *first2))
            {
                return false;
            }
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 == last2;
    }

    /**
     * @brief Performs lexicographically comparison of two ranges.
     *
     * Compares elements of two ranges lexicographically using three-way comparison
     * operator. Returns the result of the first non-equal element comparison.
     *
     * @tparam Range1 Type of the first range.
     * @tparam Range2 Type of the second range.
     * @param range1 The first range to compare.
     * @param range2 The second range to compare.
     * @return Result of lexicographical comparison (strong_ordering).
     */
    template <Range Range1, Range Range2>
        requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                              typename RangeTraits<Range2>::ReferenceType> &&
                 ThreeWayComparable<
                     CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                   typename RangeTraits<Range2>::ReferenceType>>
    std::strong_ordering lexicographicallyCompare(Range1 &range1, Range2 &range2)
    {
        auto it1 = RangeTraits<Range1>::begin(range1);
        auto it2 = RangeTraits<Range2>::begin(range2);
        auto end1 = RangeTraits<Range1>::end(range1);
        auto end2 = RangeTraits<Range2>::end(range2);
        using CommonRef = CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                        typename RangeTraits<Range2>::ReferenceType>;

        while (it1 != end1 && it2 != end2)
        {
            auto &type1 = static_cast<CommonRef>(*it1);
            auto &type2 = static_cast<CommonRef>(*it2);
            if (auto cmp = type1 <=> type2; cmp != 0)
            {
                return cmp;
            }
            ++it1;
            ++it2;
        }

        return (it1 != end1) <=> (it2 != end2);
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
    template <Range Range1, Range Range2, Invokable Pred>
    requires HasCommonRef<typename RangeTraits<Range1>::ReferenceType,
                          typename RangeTraits<Range2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>,
                       CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                     typename RangeTraits<Range2>::ReferenceType>>
    auto lexicographicallyCompare(Range1 &range1, Range2 &range2, Pred pred)
    {
        auto it1 = RangeTraits<Range1>::begin(range1);
        auto it2 = RangeTraits<Range2>::begin(range2);
        auto end1 = RangeTraits<Range1>::end(range1);
        auto end2 = RangeTraits<Range2>::end(range2);
        using CommonRef = CommonRefType<typename RangeTraits<Range1>::ReferenceType,
                                        typename RangeTraits<Range2>::ReferenceType>;

        while (it1 != end1 && it2 != end2)
        {
            auto &type1 = static_cast<CommonRef>(*it1);
            auto &type2 = static_cast<CommonRef>(*it2);
            if (auto cmp = pred(type1, type2); cmp != 0)
            {
                return cmp;
            }
            ++it1;
            ++it2;
        }

        return (it1 != end1) <=> (it2 != end2);
    }

    /**
     * @brief Performs lexicographically comparison of two iterator ranges.
     *
     * Compares elements from first1 to last1 with elements starting at first2
     * using three-way comparison operator.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @return Result of lexicographical comparison.
     */
    template <Iterator Iter1, Iterator Iter2>
      requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                            typename IterTraits<Iter2>::ReferenceType> &&
               ThreeWayComparable<
                   CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                 typename IterTraits<Iter2>::ReferenceType>>
    std::strong_ordering lexicographicallyCompare(Iter1 first1, Iter1 last1,
                                                  Iter2 first2)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;
        while (first1 != last1)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            auto& ref2 = static_cast<CommonRef>(*first2);
            if (auto cmp = ref1 <=> ref2; cmp != 0)
            {
                return cmp;
            }
            ++first1;
            ++first2;
        }

        return std::strong_ordering::equal;
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
    template <Iterator Iter1, Iterator Iter2, Invokable Pred>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                          typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType>,
                CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                              typename IterTraits<Iter2>::ReferenceType>>
    auto lexicographicallyCompare(Iter1 first1, Iter1 last1, Iter2 first2,
                                  Pred pred)
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

    /**
     * @brief Performs lexicographically comparison of two bounded iterator ranges.
     *
     * Compares elements from first1 to last1 with elements from first2 to last2
     * using three-way comparison operator.
     *
     * @tparam Iter1 Type of the first iterator.
     * @tparam Iter2 Type of the second iterator.
     * @param first1 Iterator to the beginning of the first range.
     * @param last1 Iterator to the end of the first range.
     * @param first2 Iterator to the beginning of the second range.
     * @param last2 Iterator to the end of the second range.
     * @return Result of lexicographical comparison.
     */
    template <Iterator Iter1, Iterator Iter2>
      requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                            typename IterTraits<Iter2>::ReferenceType> &&
               ThreeWayComparable<
                   CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                 typename IterTraits<Iter2>::ReferenceType>>
    std::strong_ordering lexicographicallyCompare(Iter1 first1, Iter1 last1,
                                                  Iter2 first2, Iter2 last2)
    {
        using CommonRef = CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                        typename IterTraits<Iter2>::ReferenceType>;
        while (first1 != last1 && first2 != last2)
        {
            auto& ref1 = static_cast<CommonRef>(*first1);
            auto& ref2 = static_cast<CommonRef>(*first2);
            if (auto cmp = ref1 <=> ref2; cmp != 0)
            {
                return cmp;
            }
            ++first1;
            ++first2;
        }

        return (first1 != last1) <=> (first2 != last2);
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
    template <Iterator Iter1, Iterator Iter2, Invokable Pred>
    requires HasCommonRef<typename IterTraits<Iter1>::ReferenceType,
                        typename IterTraits<Iter2>::ReferenceType> &&
             Predicate<Pred,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>,
                       CommonRefType<typename IterTraits<Iter1>::ReferenceType,
                                     typename IterTraits<Iter2>::ReferenceType>>
    auto lexicographicallyCompare(Iter1 first1, Iter1 last1, Iter2 first2,
                                  Iter2 last2, Pred pred)
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
} // namespace original::algorithms
