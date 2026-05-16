#ifndef SORT_H
#define SORT_H

namespace my {

    template <typename Iterator, typename Comparator>
    void sort(Iterator begin, Iterator end, Comparator comp)
    {
        for (auto i = begin; i != end; ++i) {
            auto min = i;
            for (auto j = i + 1; j != end; ++j) {
                if (comp(*j, *min)) {
                    min = j;
                }
            }
            if (min != i) {
                auto tmp = *i;
                *i = *min;
                *min = tmp;
            }
        }
    }

    template <typename Iterator>
    void sort(Iterator begin, Iterator end)
    {
        my::sort(begin, end, [](const auto& a, const auto& b) {
            return a < b;
        });
    }

}

#endif // SORT_H