#include <gtest/gtest.h>
#include <string_view>

import original.basic.metaInfo;

using namespace original;

class MetaInfoTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MetaInfoTest, NameIsCorrect) {
    EXPECT_EQ(std::string_view(info::NAME), "Original");
    EXPECT_NE(std::string_view(info::NAME), "");
}

TEST_F(MetaInfoTest, DescriptionIsCorrect) {
    EXPECT_EQ(std::string_view(info::DESCRIPTION), "A C++ tool library");
    EXPECT_NE(std::string_view(info::DESCRIPTION), "");
}

TEST_F(MetaInfoTest, RepoPageIsCorrect) {
    EXPECT_EQ(std::string_view(info::REPO_PAGE), "https://github.com/Original-org/original");
    EXPECT_NE(std::string_view(info::REPO_PAGE), "");

    constexpr std::string_view repo(info::REPO_PAGE);
    EXPECT_TRUE(repo.find("github.com") != std::string_view::npos);
    EXPECT_TRUE(repo.find("Original-org") != std::string_view::npos);
}

TEST_F(MetaInfoTest, LicenseIsCorrect) {
    EXPECT_EQ(std::string_view(info::LICENSE), "MIT license");
    EXPECT_NE(std::string_view(info::LICENSE), "");
}

TEST_F(MetaInfoTest, OrganizationIsCorrect) {
    EXPECT_EQ(std::string_view(info::ORGANIZATION), "Original-org");
    EXPECT_NE(std::string_view(info::ORGANIZATION), "");
}

TEST_F(MetaInfoTest, AllConstantsAreNonEmpty) {
    EXPECT_FALSE(std::string_view(info::NAME).empty());
    EXPECT_FALSE(std::string_view(info::DESCRIPTION).empty());
    EXPECT_FALSE(std::string_view(info::REPO_PAGE).empty());
    EXPECT_FALSE(std::string_view(info::LICENSE).empty());
    EXPECT_FALSE(std::string_view(info::ORGANIZATION).empty());
}

TEST_F(MetaInfoTest, ConstantsAreAccessible) {
    static_assert(std::string_view(info::NAME) == "Original");
    static_assert(std::string_view(info::ORGANIZATION) == "Original-org");
    EXPECT_NO_THROW({
        [[maybe_unused]] auto name = info::NAME;
        [[maybe_unused]] auto desc = info::DESCRIPTION;
        [[maybe_unused]] auto repo = info::REPO_PAGE;
        [[maybe_unused]] auto license = info::LICENSE;
        [[maybe_unused]] auto org = info::ORGANIZATION;
    });
}