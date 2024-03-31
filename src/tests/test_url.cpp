#include "url.h"
#include <gtest/gtest.h>

namespace netdisk {

// Test default construction
TEST(URLTest, DefaultConstructor) {
    URL url;
    EXPECT_FALSE(url.is_valid());
}

// Test parsing valid URLs
TEST(URLTest, ParseValidURLs) {
    // HTTP with explicit port
    URL httpUrl("http://example.com:8080/path");
    EXPECT_TRUE(httpUrl.is_valid());
    EXPECT_EQ(httpUrl.scheme, "http");
    EXPECT_EQ(httpUrl.host, "example.com");
    EXPECT_EQ(httpUrl.port.value(), 8080);
    EXPECT_EQ(httpUrl.path, "/path");

    // HTTPS without explicit port (default port)
    URL httpsUrl("https://example.com/path");
    EXPECT_TRUE(httpsUrl.is_valid());
    EXPECT_EQ(httpsUrl.scheme, "https");
    EXPECT_EQ(httpsUrl.host, "example.com");
    EXPECT_TRUE(httpsUrl.port.has_value());
    EXPECT_EQ(httpsUrl.port, 443);
    EXPECT_EQ(httpsUrl.path, "/path");
}

// Test parsing invalid URLs
TEST(URLTest, ParseInvalidURLs) {
    URL invalidUrl("http//example.com");
    EXPECT_FALSE(invalidUrl.is_valid());
}

// Test to_string method
TEST(URLTest, ToString) {
    URL url("http://example.com:8080/path");
    std::string expected = "http://example.com:8080/path";
    EXPECT_EQ(url.to_string(), expected);

    // Test with default port
    URL defaultPortUrl("http://example.com/path");
    expected = "http://example.com/path";
    EXPECT_EQ(defaultPortUrl.to_string(), expected);
}

// Add more tests for user, password, query, and fragment handling
TEST(URLTest, UserInfo) {
    URL urlWithUser("http://user:password@example.com/path");
    EXPECT_TRUE(urlWithUser.is_valid());
    EXPECT_EQ(urlWithUser.user.value(), "user");
    EXPECT_EQ(urlWithUser.password.value(), "password");
}

}