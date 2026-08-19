#include <iostream>
#include <ostream>
#include <vector>
#include <string_view>
#include <cctype>
#include "MapReduce.h"

const char *lorem_ipsum =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sit amet ante odio. Curabitur finibus congue nisl sed cursus. Curabitur elementum, risus sed facilisis blandit, lorem nulla ultrices neque, id pellentesque tortor erat laoreet nisl. Etiam nec turpis feugiat, porttitor est quis, dapibus lorem. Etiam tristique, nisl bibendum pellentesque mattis, nisi leo tempus quam, consectetur ullamcorper justo lorem ut ligula. Vestibulum non volutpat massa, a finibus purus. Mauris vulputate sed nibh eget malesuada. Morbi eu gravida arcu, facilisis cursus arcu."
        "Mauris vestibulum massa quis leo feugiat molestie. Nulla placerat ipsum sapien, ac consequat elit sodales ut. Fusce in auctor libero. Etiam consequat eu elit quis molestie. Quisque ac iaculis felis. Duis condimentum commodo vulputate. Integer vitae vulputate turpis. Nulla pulvinar quis nulla vel vestibulum. Duis interdum mollis turpis, sed porttitor nisi porta at. Vestibulum porttitor mi purus, nec malesuada risus tristique feugiat. Sed at lobortis enim. ";

constexpr std::size_t MAX_CHUNK_LEN = 64;

class WordCount {
public:
    WordCount() = default;

    WordCount(bool beginning_in_middle, unsigned long long count, bool ending_in_middle) : count_{count},
        beginning_in_middle_(beginning_in_middle), ending_in_middle_(ending_in_middle) {
    }

    static WordCount default_value() {
        return WordCount{};
    }

    unsigned long long to_number() const { return count_; }

    WordCount combine(const WordCount& right) const {
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

WordCount transform_to_word_count(const std::string_view &part) {
    if (part.empty()) {
        return WordCount::default_value();
    }
    char first = *part.begin();
    char last = *part.rbegin();
    // TODO: count words
    unsigned long long count = 42;
    return WordCount{!!std::isalpha(first), count, !!std::isalpha(last)};
}

// TODO: How many times does a given word reside in the text
// hint: bool -> std::optional<std::string_view>
class FindWord {

};

int main() {
    std::string_view lorem_ipsum_s = lorem_ipsum;
    std::vector<std::string_view> parts;
    for (int i = 0; i < lorem_ipsum_s.length(); i += MAX_CHUNK_LEN) {
        auto chunk_len = std::min(MAX_CHUNK_LEN, lorem_ipsum_s.length() - i);
        parts.push_back(lorem_ipsum_s.substr(i, chunk_len));
    }
    auto mr = MapReduce(parts);
    for (auto part: parts) {
        std::cout << part << "\n";
    }
    auto word_count_mr = mr.map<WordCount>(transform_to_word_count);
    std::cout << word_count_mr.reduce().to_number() << "\n";
}
