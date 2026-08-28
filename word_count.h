#pragma once
#include <unordered_set>

class WordCount {
public:
    WordCount() = default;

    WordCount(bool beginning_in_middle, unsigned long long count, bool ending_in_middle) : count_{count},
        beginning_in_middle_(beginning_in_middle), ending_in_middle_(ending_in_middle) {
    }

    static WordCount default_value() {
        return WordCount{};
    }

    static bool is_special_character(const char c) {
        static std::unordered_set<char> special_characters = {'{', '}', '!'};
        return special_characters.contains(c);
    }

    [[nodiscard]] unsigned long long to_number() const { return count_; }

    [[nodiscard]] WordCount combine(const WordCount &right) const {
        bool begin = this->beginning_in_middle_;
        unsigned long long count = this->count_ + right.count_;
        bool end = right.ending_in_middle_;
        if (this->ending_in_middle_ && right.beginning_in_middle_) {
            count -= 1;
        }
        return WordCount{begin, count, end};
    }

private:
    unsigned long long count_;
    bool beginning_in_middle_;
    bool ending_in_middle_;
};

WordCount transform_to_word_count(const std::string_view &);
