#ifndef Z_AKR_ENUMERATOR_HH
#define Z_AKR_ENUMERATOR_HH

#include <type_traits>

namespace akr
{
    template<class ForwardIterator>
    struct ForwardEnumerator final
    {
        private:
        ForwardIterator forwardBegin;

        ForwardIterator forwardEnd;

        public:
        template<class T, std::size_t N>
        explicit constexpr ForwardEnumerator(T(&array_)[N]) noexcept:
            ForwardEnumerator(array_, array_ + N)
        {
        }

        template<class T>
        explicit constexpr ForwardEnumerator(T&& container)
            noexcept(noexcept(container.begin()) && noexcept(container.end())
                     && noexcept(ForwardEnumerator(std::declval<ForwardIterator>(),
                                                   std::declval<ForwardIterator>()))):
            ForwardEnumerator(container.begin(), container.end())
        {
        }

        explicit constexpr ForwardEnumerator(ForwardIterator begin, ForwardIterator end)
            noexcept(noexcept(ForwardIterator(std::declval<ForwardIterator>()))):
            forwardBegin { begin },
            forwardEnd   { end   }
        {
        }

        public:
        constexpr auto begin ()       noexcept ->       ForwardIterator&
        {
            return forwardBegin;
        }
        constexpr auto end   ()       noexcept ->       ForwardIterator&
        {
            return forwardEnd;
        }

        constexpr auto begin () const noexcept -> const ForwardIterator&
        {
            return forwardBegin;
        }
        constexpr auto end   () const noexcept -> const ForwardIterator&
        {
            return forwardEnd;
        }

        constexpr auto cbegin() const noexcept -> const ForwardIterator&
        {
            return forwardBegin;
        }
        constexpr auto cend  () const noexcept -> const ForwardIterator&
        {
            return forwardEnd;
        }
    };

    template<class T, std::size_t N>
    explicit ForwardEnumerator(T(&array_)[N]) -> ForwardEnumerator<T*>;

    template<class T>
    explicit ForwardEnumerator(T&& container) -> ForwardEnumerator<decltype(container.begin())>;

    template<class ForwardIterator>
    struct ReverseEnumerator final
    {
        private:
        struct ReverseIterator final
        {
            private:
            ForwardIterator iterator;

            bool            continue_ = true;

            public:
            explicit constexpr ReverseIterator(ForwardIterator iterator_)
                noexcept(noexcept(ForwardIterator(std::declval<ForwardIterator>()))):
                iterator { iterator_ }
            {
            }

            public:
            constexpr auto operator* () const noexcept(noexcept(*std::declval<ForwardIterator>())) -> auto&&
            {
                return *iterator;
            }

            constexpr auto operator->() const noexcept -> const ForwardIterator&
            {
                return  iterator;
            }

            constexpr auto operator++(   ) noexcept(isDecOperatorNoexcept<ForwardIterator>()) -> ReverseIterator&
            {
                auto&& rhs = *this;

                if (rhs.continue_)
                {
                    --rhs.iterator;
                }

                return rhs;
            }
            constexpr auto operator++(int) noexcept(noexcept(ReverseIterator(std::declval<ReverseIterator>()))
                                                 && isIncOperatorNoexcept<ReverseIterator>()) -> ReverseIterator
            {
                auto&& lhs = *this;

                ReverseIterator tmp(lhs);

                ++lhs;

                return tmp;
            }

            constexpr auto operator--(   ) noexcept(isIncOperatorNoexcept<ForwardIterator>()) -> ReverseIterator&
            {
                auto&& rhs = *this;

                if (rhs.continue_)
                {
                    ++rhs.iterator;
                }

                return rhs;
            }
            constexpr auto operator--(int) noexcept(noexcept(ReverseIterator(std::declval<ReverseIterator>()))
                                                 && isDecOperatorNoexcept<ReverseIterator>()) -> ReverseIterator
            {
                auto&& lhs = *this;

                ReverseIterator tmp(lhs);

                --lhs;

                return tmp;
            }

            friend constexpr auto operator!=(ReverseIterator& lhs, const ReverseIterator& rhs) noexcept -> bool
            {
                if (lhs.iterator != rhs.iterator)
                {
                    return true;
                }

                if (lhs.continue_)
                {
                    lhs.continue_ = false;

                    return true;
                }

                return false;
            }
        };

        private:
        ReverseIterator reverseBegin;

        ReverseIterator reverseEnd;

        public:
        template<class T, std::size_t N>
        explicit constexpr ReverseEnumerator(T(&array_)[N]) noexcept:
            ReverseEnumerator(array_, array_ + N)
        {
        }

        template<class T>
        explicit constexpr ReverseEnumerator(T&& container)
            noexcept(noexcept(container.begin()) && noexcept(container.end())
                     && noexcept(ReverseEnumerator(std::declval<ForwardIterator>(),
                                                   std::declval<ForwardIterator>()))):
            ReverseEnumerator(container.begin(), container.end())
        {
        }

        explicit constexpr ReverseEnumerator(ForwardIterator begin, ForwardIterator end)
            noexcept(noexcept(ReverseIterator(std::declval<ForwardIterator>()))
                     && isDecOperatorNoexcept<ForwardIterator>()):
            reverseBegin { --end },
            reverseEnd   { begin }
        {
        }

        public:
        constexpr auto begin ()       noexcept ->       ReverseIterator&
        {
            return reverseBegin;
        }
        constexpr auto end   ()       noexcept ->       ReverseIterator&
        {
            return reverseEnd;
        }

        constexpr auto begin () const noexcept -> const ReverseIterator&
        {
            return reverseBegin;
        }
        constexpr auto end   () const noexcept -> const ReverseIterator&
        {
            return reverseEnd;
        }

        constexpr auto cbegin() const noexcept -> const ReverseIterator&
        {
            return reverseBegin;
        }
        constexpr auto cend  () const noexcept -> const ReverseIterator&
        {
            return reverseEnd;
        }

        private:
        template<class T>
        static consteval auto isIncOperatorNoexcept() noexcept -> bool
        {
            if constexpr (!std::is_class_v<T>)
            {
                return true;
            }
            else
            {
                return noexcept(std::declval<T>().operator++());
            }
        }

        template<class T>
        static consteval auto isDecOperatorNoexcept() noexcept -> bool
        {
            if constexpr (!std::is_class_v<T>)
            {
                return true;
            }
            else
            {
                return noexcept(std::declval<T>().operator--());
            }
        }
    };

    template<class T, std::size_t N>
    explicit ReverseEnumerator(T(&array_)[N]) -> ReverseEnumerator<T*>;

    template<class T>
    explicit ReverseEnumerator(T&& container) -> ReverseEnumerator<decltype(container.begin())>;
}

#ifdef  D_AKR_TEST
#include <vector>
#include <list>
#include <string>

namespace akr::test
{
    AKR_TEST(ForwardEnumerator,
    {
        const auto test = [](auto&& elems_)
        {
            {
                auto fe1_ = ForwardEnumerator(elems_);

                auto i_ = 1;
                for (auto&& e_ : fe1_)
                {
                    assert(e_ == i_++);
                    e_ += 10;
                }
                assert(i_ == 6);
                {
                    i_ = 1;
                    for (auto iter_ = fe1_.begin(); iter_ != fe1_.end(); iter_++)
                    {
                        assert(*iter_ == i_++ + 10);
                        *iter_ -= 10;
                    }
                    assert(i_ == 6);
                }
                {
                    i_ = 1;
                    for (auto iter_ = fe1_.cbegin(); iter_ != fe1_.cend(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
            }
            {
                const auto cfe1_ = ForwardEnumerator(elems_);

                auto i_ = 1;
                for (auto&& e_ : cfe1_)
                {
                    assert(e_ == i_++);
                }
                assert(i_ == 6);
                {
                    i_ = 1;
                    for (auto iter_ = cfe1_.begin(); iter_ != cfe1_.end(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
                {
                    i_ = 1;
                    for (auto iter_ = cfe1_.cbegin(); iter_ != cfe1_.cend(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
            }
            {
                auto fe2_ = ForwardEnumerator(std::begin(elems_), std::end(elems_));

                auto i_ = 1;
                for (auto&& e_ : fe2_)
                {
                    assert(e_ == i_++);
                    e_ += 10;
                }
                assert(i_ == 6);
                {
                    i_ = 1;
                    for (auto iter_ = fe2_.begin(); iter_ != fe2_.end(); iter_++)
                    {
                        assert(*iter_ == i_++ + 10);
                        *iter_ -= 10;
                    }
                    assert(i_ == 6);
                }
                {
                    i_ = 1;
                    for (auto iter_ = fe2_.cbegin(); iter_ != fe2_.cend(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
            }
            {
                const auto cfe2_ = ForwardEnumerator(std::begin(elems_), std::end(elems_));

                auto i_ = 1;
                for (auto&& e_ : cfe2_)
                {
                    assert(e_ == i_++);
                }
                assert(i_ == 6);
                {
                    i_ = 1;
                    for (auto iter_ = cfe2_.begin(); iter_ != cfe2_.end(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
                {
                    i_ = 1;
                    for (auto iter_ = cfe2_.cbegin(); iter_ != cfe2_.cend(); iter_++)
                    {
                        assert(*iter_ == i_++);
                    }
                    assert(i_ == 6);
                }
            }
        };

        int a[5];
        for (auto i = 0; i < 5; i++) a[i] = i + 1;

        static_assert(noexcept(ForwardEnumerator(a)));
        static_assert(noexcept(ForwardEnumerator(std::begin(a), std::end(a))));

        struct Test1
        {
            auto begin() noexcept(true) -> Test1*;
            auto end  () noexcept(true) -> Test1*;
        };

        using Test1Iterator = decltype(std::declval<Test1>().begin());

        static_assert(noexcept(Test1Iterator(std::declval<Test1Iterator>())));

        static_assert((noexcept(std::declval<Test1>().begin()) && noexcept(std::declval<Test1>().end())));
        static_assert(noexcept(ForwardEnumerator(std::declval<Test1Iterator>(), std::declval<Test1Iterator>())));
        static_assert(noexcept(ForwardEnumerator(std::declval<Test1>())));

        static_assert(noexcept(ForwardEnumerator(std::declval<Test1>().begin(), std::declval<Test1>().end())));

        struct Test2
        {
            auto begin() noexcept(false) -> Test2*;
            auto end  () noexcept(false) -> Test2*;
        };

        using Test2Iterator = decltype(std::declval<Test2>().begin());

        static_assert(noexcept(Test2Iterator(std::declval<Test2Iterator>())));

        static_assert(!(noexcept(std::declval<Test2>().begin()) && noexcept(std::declval<Test2>().end())));
        static_assert(noexcept(ForwardEnumerator(std::declval<Test2Iterator>(), std::declval<Test2Iterator>())));
        static_assert(!noexcept(ForwardEnumerator(std::declval<Test2>())));

        static_assert(!noexcept(ForwardEnumerator(std::declval<Test2>().begin(), std::declval<Test2>().end())));

        struct Test3
        {
            struct Iterator
            {
                auto operator* (   ) const noexcept(true) -> Test3&;

                auto operator++(   ) const noexcept(true) -> Test3*;
                auto operator++(int) const noexcept(true) -> Test3*;

                auto operator--(   ) const noexcept(true) -> Test3*;
                auto operator--(int) const noexcept(true) -> Test3*;
            };

            auto begin() noexcept -> Iterator;
            auto end  () noexcept -> Iterator;
        };

        using BeginType3 = decltype(std::declval<ForwardEnumerator<Test3::Iterator>>().begin());

        static_assert(noexcept(* std::declval<BeginType3>()));
        static_assert(noexcept(++std::declval<BeginType3>()));
        static_assert(noexcept(std::declval<BeginType3>()++));
        static_assert(noexcept(--std::declval<BeginType3>()));
        static_assert(noexcept(std::declval<BeginType3>()--));

        struct Test4
        {
            struct Iterator
            {
                auto operator* (   ) const noexcept(false) -> Test4&;

                auto operator++(   ) const noexcept(false) -> Test4*;
                auto operator++(int) const noexcept(false) -> Test4*;

                auto operator--(   ) const noexcept(false) -> Test4*;
                auto operator--(int) const noexcept(false) -> Test4*;
            };

            auto begin() noexcept -> Iterator;
            auto end  () noexcept -> Iterator;
        };

        using BeginType4 = decltype(std::declval<ForwardEnumerator<Test4::Iterator>>().begin());

        static_assert(!noexcept(* std::declval<BeginType4>()));
        static_assert(!noexcept(++std::declval<BeginType4>()));
        static_assert(!noexcept(std::declval<BeginType4>()++));
        static_assert(!noexcept(--std::declval<BeginType4>()));
        static_assert(!noexcept(std::declval<BeginType4>()--));

        test(a);
        test(std::vector({ 1, 2, 3, 4, 5 }));
        test(std::list  ({ 1, 2, 3, 4, 5 }));

        using namespace std::string_literals;

        auto vec = std::vector({ "1"s, "2"s, "3"s, "4"s, "5"s });

        auto fbegin = ForwardEnumerator(vec).begin();
        assert(!fbegin->compare("1"s));
        fbegin++;
        assert(!fbegin->compare("2"s));
        fbegin++;
        assert(!fbegin->compare("3"s));
        fbegin--;
        assert(!fbegin->compare("2"s));
        fbegin--;
        assert(!fbegin->compare("1"s));
        ++fbegin;
        assert(!fbegin->compare("2"s));
        ++fbegin;
        assert(!fbegin->compare("3"s));
        --fbegin;
        assert(!fbegin->compare("2"s));
        --fbegin;
        assert(!fbegin->compare("1"s));
        fbegin->clear();
        assert(fbegin->empty());
    });

    AKR_TEST(ReverseEnumerator,
    {
        const auto test = [](auto&& elems_)
        {
            {
                auto re1_ = ReverseEnumerator(elems_);

                auto i_ = 5;
                for (auto&& e_ : re1_)
                {
                    assert(e_ == i_--);
                    e_ += 10;
                }
                assert(i_ == 0);
                {
                    i_ = 5;
                    for (auto iter_ = re1_.begin(); iter_ != re1_.end(); iter_++)
                    {
                        assert(*iter_ == i_-- + 10);
                        *iter_ -= 10;
                    }
                    assert(i_ == 0);
                }
                {
                    i_ = 5;
                    for (auto iter_ = re1_.cbegin(); iter_ != re1_.cend(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
            }
            {
                const auto cre1_ = ReverseEnumerator(elems_);

                auto i_ = 5;
                for (auto&& e_ : cre1_)
                {
                    assert(e_ == i_--);
                }
                assert(i_ == 0);
                {
                    i_ = 5;
                    for (auto iter_ = cre1_.begin(); iter_ != cre1_.end(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
                {
                    i_ = 5;
                    for (auto iter_ = cre1_.cbegin(); iter_ != cre1_.cend(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
            }
            {
                auto re2_ = ReverseEnumerator(std::begin(elems_), std::end(elems_));

                auto i_ = 5;
                for (auto&& e_ : re2_)
                {
                    assert(e_ == i_--);
                    e_ += 10;
                }
                assert(i_ == 0);
                {
                    i_ = 5;
                    for (auto iter_ = re2_.begin(); iter_ != re2_.end(); iter_++)
                    {
                        assert(*iter_ == i_-- + 10);
                        *iter_ -= 10;
                    }
                    assert(i_ == 0);
                }
                {
                    i_ = 5;
                    for (auto iter_ = re2_.cbegin(); iter_ != re2_.cend(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
            }
            {
                const auto cre2_ = ReverseEnumerator(std::begin(elems_), std::end(elems_));

                auto i_ = 5;
                for (auto&& e_ : cre2_)
                {
                    assert(e_ == i_--);
                }
                assert(i_ == 0);
                {
                    i_ = 5;
                    for (auto iter_ = cre2_.begin(); iter_ != cre2_.end(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
                {
                    i_ = 5;
                    for (auto iter_ = cre2_.cbegin(); iter_ != cre2_.cend(); iter_++)
                    {
                        assert(*iter_ == i_--);
                    }
                    assert(i_ == 0);
                }
            }
        };

        int a[5];
        for (auto i = 0; i < 5; i++) a[i] = i + 1;

        static_assert(noexcept(ReverseEnumerator(a)));
        static_assert(noexcept(ReverseEnumerator(std::begin(a), std::end(a))));

        struct Test1
        {
            auto begin() noexcept(true) -> Test1*;
            auto end  () noexcept(true) -> Test1*;
        };

        using Test1Iterator = decltype(std::declval<Test1>().begin());

        static_assert(noexcept(Test1Iterator(std::declval<Test1Iterator>())));

        static_assert((noexcept(std::declval<Test1>().begin()) && noexcept(std::declval<Test1>().end())));
        static_assert(noexcept(ReverseEnumerator(std::declval<Test1Iterator>(), std::declval<Test1Iterator>())));
        static_assert(noexcept(ReverseEnumerator(std::declval<Test1>())));

        static_assert(noexcept(ReverseEnumerator(std::declval<Test1>().begin(), std::declval<Test1>().end())));

        struct Test2
        {
            auto begin() noexcept(false) -> Test2*;
            auto end  () noexcept(false) -> Test2*;
        };

        using Test2Iterator = decltype(std::declval<Test2>().begin());

        static_assert(noexcept(Test2Iterator(std::declval<Test2Iterator>())));

        static_assert(!(noexcept(std::declval<Test2>().begin()) && noexcept(std::declval<Test2>().end())));
        static_assert(noexcept(ReverseEnumerator(std::declval<Test2Iterator>(), std::declval<Test2Iterator>())));
        static_assert(!noexcept(ReverseEnumerator(std::declval<Test2>())));

        static_assert(!noexcept(ReverseEnumerator(std::declval<Test2>().begin(), std::declval<Test2>().end())));

        struct Test3
        {
            struct Iterator
            {
                auto operator* (   ) const noexcept(true) -> Test3&;

                auto operator++(   ) const noexcept(true) -> Test3*;
                auto operator++(int) const noexcept(true) -> Test3*;

                auto operator--(   ) const noexcept(true) -> Test3*;
                auto operator--(int) const noexcept(true) -> Test3*;
            };

            auto begin() noexcept -> Iterator;
            auto end  () noexcept -> Iterator;
        };

        using BeginType3 = decltype(std::declval<ReverseEnumerator<Test3::Iterator>>().begin());

        static_assert(noexcept(* std::declval<BeginType3>()));
        static_assert(noexcept(++std::declval<BeginType3>()));
        static_assert(noexcept(std::declval<BeginType3>()++));
        static_assert(noexcept(--std::declval<BeginType3>()));
        static_assert(noexcept(std::declval<BeginType3>()--));

        struct Test4
        {
            struct Iterator
            {
                auto operator* (   ) const noexcept(false) -> Test4&;

                auto operator++(   ) const noexcept(false) -> Test4*;
                auto operator++(int) const noexcept(false) -> Test4*;

                auto operator--(   ) const noexcept(false) -> Test4*;
                auto operator--(int) const noexcept(false) -> Test4*;
            };

            auto begin() noexcept -> Iterator;
            auto end  () noexcept -> Iterator;
        };

        using BeginType4 = decltype(std::declval<ReverseEnumerator<Test4::Iterator>>().begin());

        static_assert(!noexcept(* std::declval<BeginType4>()));
        static_assert(!noexcept(++std::declval<BeginType4>()));
        static_assert(!noexcept(std::declval<BeginType4>()++));
        static_assert(!noexcept(--std::declval<BeginType4>()));
        static_assert(!noexcept(std::declval<BeginType4>()--));

        test(a);
        test(std::vector({ 1, 2, 3, 4, 5 }));
        test(std::list  ({ 1, 2, 3, 4, 5 }));

        using namespace std::string_literals;

        auto vec = std::vector({ "1"s, "2"s, "3"s, "4"s, "5"s });

        auto rbegin = ReverseEnumerator(vec).begin();
        assert(!rbegin->compare("5"s));
        rbegin++;
        assert(!rbegin->compare("4"s));
        rbegin++;
        assert(!rbegin->compare("3"s));
        rbegin--;
        assert(!rbegin->compare("4"s));
        rbegin--;
        assert(!rbegin->compare("5"s));
        ++rbegin;
        assert(!rbegin->compare("4"s));
        ++rbegin;
        assert(!rbegin->compare("3"s));
        --rbegin;
        assert(!rbegin->compare("4"s));
        --rbegin;
        assert(!rbegin->compare("5"s));
        rbegin->clear();
        assert(rbegin->empty());
    });
}
#endif//D_AKR_TEST

#endif//Z_AKR_ENUMERATOR_HH
