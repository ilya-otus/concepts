#include <boost/concept_check.hpp>
#include <algorithm>
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <type_traits>

namespace otus {
template <typename T>
struct DefaultConstructibleConcept {
    BOOST_CONCEPT_USAGE(DefaultConstructibleConcept) {
        static_assert(std::is_default_constructible_v<T>, "Type is not default constructible&");
    }
};
template <typename T>
struct EqualityComparableConcept {
    BOOST_CONCEPT_USAGE(EqualityComparableConcept) {
        static_assert(std::is_same_v<decltype(a==b), bool>, "Type is not equality comparable");
        static_assert(std::is_same_v<decltype(a==a), bool>, "Type is not equality comparable");
    }
private:
    T a;
    const T b;
};

template <typename T>
struct IteratorConcept {
    using value_type = typename std::iterator_traits<T>::value_type;
    using difference_type = typename std::iterator_traits<T>::difference_type;
    using reference = typename std::iterator_traits<T>::reference;
    using pointer = typename std::iterator_traits<T>::pointer;
    using iterator_category = typename std::iterator_traits<T>::iterator_category;
    BOOST_CONCEPT_USAGE(IteratorConcept) {
        static_assert(std::is_copy_constructible_v<T>, "Type is not an iterator because it is not copy constructible");
        static_assert(std::is_copy_assignable_v<T>, "Type is not an iterator because it is not copy assignable");
        static_assert(std::is_destructible_v<T>, "Type is not an iterator because it is not destructable");
        static_assert(std::is_swappable_v<T>, "Type is not an iterator because it is not swappable");
        *t;
        T &_t = ++t;
    }
private:
    T t;
};

template <typename T>
struct InputIteratorConcept : public EqualityComparableConcept<T>, public IteratorConcept<T> {
    using value_type = typename std::iterator_traits<T>::value_type;
    using reference = typename std::iterator_traits<T>::reference;
    BOOST_CONCEPT_USAGE(InputIteratorConcept) {
        static_assert(std::is_same_v<decltype(i!=j), bool>, "Type is not InputIterator because comparasion is not a bool");
        static_assert(std::is_same_v<decltype(*i), reference>, "Type is not InputIterator because operator* doesn't reference");
        static_assert(std::is_convertible_v<decltype(*i), value_type>, "Type is not InputIterator because reference doesn't convertible to value_type");
        //i->m;
        static_assert(std::is_same_v<decltype(++i), T&>, "Type is not InputIterator because operator++ doesn't reference");
        (void)i++;
        static_assert(std::is_convertible_v<decltype(*i++), value_type>, "Type is not InputIterator because *i++ doesn't convertible to value_type");
    }
private:
    T i;
    const T j;
};

template <typename T>
struct ForwardIteratorConcept : public DefaultConstructibleConcept<T>, public InputIteratorConcept<T> {
    using reference = typename std::iterator_traits<T>::reference;
    BOOST_CONCEPT_USAGE(ForwardIteratorConcept) {
        static_assert(std::is_same_v<decltype(i++), T>, "Type is not ForwardIterator because increment doesn't return an iterator");
        static_assert(std::is_same_v<decltype(*i++), reference>, "Type is not ForwardIterator because increment doesn't return an iterator");
    }
private:
    T i;
};

template <typename T>
struct BidirectionalIteratorConcept : public ForwardIteratorConcept<T> {
    using reference = typename std::iterator_traits<T>::reference;
    BOOST_CONCEPT_USAGE(BidirectionalIteratorConcept) {
        static_assert(std::is_same_v<decltype(--i), T&>, "Type is not BidirectionalIterator because decrement operator doesn't return a reference of the type");
        static_assert(std::is_convertible_v<decltype(i--), const T&>, "Type is not BidirectionalIterator because decrement operator doesn't convertible to a const reference of the type");
        static_assert(std::is_same_v<decltype(*i--), reference>, "Type is not BidirectionalIterator because *i-- doesn't return reference");
    }
private:
    T i;
};

template <typename T>
struct RandomAccessIteratorConcept : public BidirectionalIteratorConcept<T> {
    using value_type = typename std::iterator_traits<T>::value_type;
    using difference_type = typename std::iterator_traits<T>::difference_type;
    using reference = typename std::iterator_traits<T>::reference;
    BOOST_CONCEPT_USAGE(RandomAccessIteratorConcept) {
        static_assert(std::is_same_v<decltype(r += n), T&>, "Type is not RandomAccessIterator because an operator+= doesn't return a reference to an iterator");
        static_assert(std::is_same_v<decltype(a + n), T>, "Type is not RandomAccessIterator because a sum of an iterator and difference_type isn't an iterator");
        static_assert(std::is_same_v<decltype(n + a), T>, "Type is not RandomAccessIterator because a sum of difference_type and an iterator isn't an iterator");
        static_assert(std::is_same_v<decltype(r -= n), T&>, "Type is not RandomAccessIterator because an operator-= doesn't return a reference to an iterator");
        static_assert(std::is_same_v<decltype(i - n), T>, "Type is not RandomAccessIterator because a difference of an iterator and a difference_type isn't an iterator");
        static_assert(std::is_same_v<decltype(b - a), difference_type>, "Type is not RandomAccessIterator because a difference of iterators isn't a difference_type");
        static_assert(std::is_convertible_v<decltype(i[n]), reference>, "Type is not RandomAccessIterator because index operator doesn't return a reference");
        static_assert(std::is_same_v<decltype(a < b), bool>, "Type is not RandomAccessIterator because comparison doesn't return a bool");
        static_assert(std::is_same_v<decltype(a > b), bool>, "Type is not RandomAccessIterator because comparison doesn't return a bool");
        static_assert(std::is_same_v<decltype(a >= b), bool>, "Type is not RandomAccessIterator because comparison doesn't return a bool");
        static_assert(std::is_same_v<decltype(a <= b), bool>, "Type is not RandomAccessIterator because comparison doesn't return a bool");
    }
private:
    T i;
    const T a, b;
    T& r;
    difference_type n;
};
} // namespace otus

struct Disabled {
    Disabled() = delete;
    bool operator==(const Disabled &) = delete;
};

void tests() {
    // Positive cases
    BOOST_CONCEPT_ASSERT((otus::RandomAccessIteratorConcept<std::vector<int>::iterator>));
    BOOST_CONCEPT_ASSERT((otus::BidirectionalIteratorConcept<std::list<int>::iterator>));
    BOOST_CONCEPT_ASSERT((otus::ForwardIteratorConcept<std::forward_list<int>::iterator>));
    BOOST_CONCEPT_ASSERT((otus::InputIteratorConcept<std::istreambuf_iterator<int>>));
    // Negative ones
    //BOOST_CONCEPT_ASSERT((otus::RandomAccessIteratorConcept<std::list<int>::iterator>));
    //BOOST_CONCEPT_ASSERT((otus::BidirectionalIteratorConcept<std::forward_list<int>::iterator>));
    //BOOST_CONCEPT_ASSERT((otus::InputIteratorConcept<std::ostreambuf_iterator<int>>));
    //BOOST_CONCEPT_ASSERT((otus::EqualityComparableConcept<Disabled>));
    //BOOST_CONCEPT_ASSERT((otus::DefaultConstructibleConcept<Disabled>));
}

int main(int argc, char * argv[]) {
    tests();
}

