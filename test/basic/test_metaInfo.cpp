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
    EXPECT_EQ(std::string_view(Info::NAME), "Original");
    EXPECT_NE(std::string_view(Info::NAME), "");
}

TEST_F(MetaInfoTest, DescriptionIsCorrect) {
    EXPECT_EQ(std::string_view(Info::DESCRIPTION), "A C++ tool library");
    EXPECT_NE(std::string_view(Info::DESCRIPTION), "");
}

TEST_F(MetaInfoTest, RepoPageIsCorrect) {
    EXPECT_EQ(std::string_view(Info::REPO_PAGE), "https://github.com/Original-org/original");
    EXPECT_NE(std::string_view(Info::REPO_PAGE), "");

    std::string_view repo(Info::REPO_PAGE);
    EXPECT_TRUE(repo.find("github.com") != std::string_view::npos);
    EXPECT_TRUE(repo.find("Original-org") != std::string_view::npos);
}

TEST_F(MetaInfoTest, LicenseIsCorrect) {
    EXPECT_EQ(std::string_view(Info::LICENSE), "MIT license");
    EXPECT_NE(std::string_view(Info::LICENSE), "");
}

TEST_F(MetaInfoTest, OrganizationIsCorrect) {
    EXPECT_EQ(std::string_view(Info::ORGANIZATION), "Original-org");
    EXPECT_NE(std::string_view(Info::ORGANIZATION), "");
}

TEST_F(MetaInfoTest, AllConstantsAreNonEmpty) {
    EXPECT_FALSE(std::string_view(Info::NAME).empty());
    EXPECT_FALSE(std::string_view(Info::DESCRIPTION).empty());
    EXPECT_FALSE(std::string_view(Info::REPO_PAGE).empty());
    EXPECT_FALSE(std::string_view(Info::LICENSE).empty());
    EXPECT_FALSE(std::string_view(Info::ORGANIZATION).empty());
}

TEST_F(MetaInfoTest, ConstantsAreAccessible) {
    static_assert(std::string_view(Info::NAME) == "Original");
    static_assert(std::string_view(Info::ORGANIZATION) == "Original-org");
    EXPECT_NO_THROW({
        [[maybe_unused]] auto name = Info::NAME;
        [[maybe_unused]] auto desc = Info::DESCRIPTION;
        [[maybe_unused]] auto repo = Info::REPO_PAGE;
        [[maybe_unused]] auto license = Info::LICENSE;
        [[maybe_unused]] auto org = Info::ORGANIZATION;
    });
}