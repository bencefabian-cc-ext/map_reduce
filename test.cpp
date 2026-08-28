#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>
#include "word_count.h"

class WordCountTestWithParam : public ::testing::TestWithParam<std::string_view> {
public:
    static WordCount GetWordCount() { return transform_to_word_count(GetParam()); }
};

INSTANTIATE_TEST_SUITE_P(ParameterizedWordCountTests, WordCountTestWithParam, ::testing::Values("one,two", "one;two", "one/two"));

TEST(WordCountTests, EmptyWordCountTest) {
    auto wordcount = WordCount{};
    EXPECT_EQ(wordcount.to_number(), 0);
}

TEST(WordCountTests, EmptyWordCountTestFromStringView) {
    auto wordcount = transform_to_word_count("");
    EXPECT_EQ(wordcount.to_number(), 0);
}

TEST(WordCountTests, SingleWhitespaceTest) {
    auto wordcount = transform_to_word_count(" ");
    EXPECT_EQ(wordcount.to_number(), 0);
}

TEST(WordCountTests, SingleCommaTest) {
    auto wordcount = transform_to_word_count(",");
    EXPECT_EQ(wordcount.to_number(), 0);
}

TEST(WordCountTests, NoAlphaTest) {
    auto wordcount = transform_to_word_count("    ;.;';.;'.   ;';., ");
    EXPECT_EQ(wordcount.to_number(), 0);
}

TEST(WordCountTests, OneWordCountTest) {
    auto wordcount = transform_to_word_count("one");
    EXPECT_EQ(wordcount.to_number(), 1);
}

TEST(WordCountTests, OneWordCountLeadingWhitespaceTest) {
    auto wordcount = transform_to_word_count(" one");
    EXPECT_EQ(wordcount.to_number(), 1);
}

TEST(WordCountTests, TwoWordsWhitespaceTest) {
    auto wordcount = transform_to_word_count("one two");
    EXPECT_EQ(wordcount.to_number(), 2);
}

TEST_P(WordCountTestWithParam, TwoWordsCommaTest) {
    auto wordcount = GetWordCount();
    EXPECT_EQ(wordcount.to_number(), 2);
}

TEST(WordCountMonoidTests, LeftIdentityTest) {
    auto left = WordCount::default_value();
    auto right = transform_to_word_count("something something");
    auto combined = left.combine(right);
    EXPECT_EQ(combined.to_number(), right.to_number());
    EXPECT_EQ(combined.to_number(), 2);
}

RC_GTEST_PROP(WordCountProp, LeftIdentity, ()) {
    int amount = *rc::gen::inRange(0, 1024 * 8);
    // Printable ASCII only (space through tilde: 0x20–0x7E)
    auto printableAscii = rc::gen::inRange<char>(' '+1, '~');
    auto word = *rc::gen::nonEmpty(rc::gen::container<std::string>(printableAscii));
    std::erase_if(word, ::isspace);
    std::stringstream ss{};
    for (int i = 0; i < amount; i++) {
        ss << " " << word;
    }
    if (long count = std::count_if(word.begin(), word.end(), ::WordCount::is_special_character); count > 0) {
        amount *= count + 1;
    }
    auto text = ss.str();
    auto left = WordCount::default_value();
    auto right = transform_to_word_count(text);
    auto combined = left.combine(right);
    RC_ASSERT(combined.to_number() == right.to_number());
    RC_ASSERT(combined.to_number() ==  amount);
}

TEST(WordCountMonoidTests, LeftIdentityWhitespaceTest) {
    auto left = WordCount::default_value();
    auto right = transform_to_word_count(" something something");
    auto combined = left.combine(right);
    EXPECT_EQ(combined.to_number(), right.to_number());
    EXPECT_EQ(combined.to_number(), 2);
}

TEST(WordCountMonoidTests, AssociativityOneTest) {
    auto a = transform_to_word_count("oneW");
    auto b = transform_to_word_count("ordSp");
    auto c = transform_to_word_count("litTwice");

    auto left_first = a.combine(b).combine(c);
    auto right_first = a.combine(b.combine(c));

    EXPECT_EQ(left_first.to_number(), right_first.to_number());
    EXPECT_EQ(left_first.to_number(), 1);
}
TEST(WordCountMonoidTests, AssociativityTest) {
    auto a = transform_to_word_count("one ");
    auto b = transform_to_word_count("two");
    auto c = transform_to_word_count(" three");

    auto left_first = a.combine(b).combine(c);
    auto right_first = a.combine(b.combine(c));

    EXPECT_EQ(left_first.to_number(), right_first.to_number());
    EXPECT_EQ(left_first.to_number(), 3);
}
