#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (*this).node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            assert((*this).node_ != nullptr);
            (*this).node_ = (*this).node_ -> next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {
        Node* iter_before_begin = &head_;
        return Iterator{iter_before_begin};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        const auto iter_before_begin = &head_;
        return ConstIterator{const_cast<Node*>(iter_before_begin)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        Node* iter_before_begin = &head_;
        return ConstIterator{iter_before_begin};
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* inserted_node_ptr = new Node(value, pos.node_->next_node);
        pos.node_->next_node = inserted_node_ptr;
        ++size_;
        return Iterator{inserted_node_ptr};
    }

    void PopFront() noexcept {
        if(head_.next_node != nullptr) {
            Node* first_erased_el = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            --size_;
            delete first_erased_el;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        if(pos.node_->next_node == nullptr) {
            return Iterator{pos.node_->next_node};
        }
        Node* pos_to_be_erased = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete pos_to_be_erased;
        --size_;
        return Iterator{pos.node_->next_node};
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    SingleLinkedList() {
        head_.next_node = nullptr;
        size_ = 0u;
    }

    ~SingleLinkedList() {
        Clear();
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while(head_.next_node != nullptr) {
            auto buff_ptr = head_.next_node;
            head_.next_node = head_.next_node -> next_node;
            delete buff_ptr;
            --size_;
        }
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        if(size_ == 0) {
            return true;
        }
        return false;
    }

    SingleLinkedList(const std::initializer_list<Type> values) {
        SingleLinkedList<Type> tmp;
        auto from = values.begin();
        auto to = values.end();
        Node** node_ptr = &tmp.head_.next_node;
        while (from != to) {
            *node_ptr = new Node(*from, nullptr);
            ++tmp.size_;
            node_ptr = &((*node_ptr)->next_node);
            ++from;
        }
        swap(tmp);  
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Copy(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        Copy(rhs);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    void Copy(const SingleLinkedList& source) {
    SingleLinkedList tmp;
    auto from = source.begin();
    auto to = source.end();
    Node** node_ptr = &tmp.head_.next_node;
    while (from != to) {
        *node_ptr = new Node(*from, nullptr);
        ++tmp.size_;
        node_ptr = &((*node_ptr)->next_node);
        ++from;
    }
    swap(tmp);
    }

private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}