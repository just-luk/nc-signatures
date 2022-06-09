// // Copyright Steinwurf ApS 2019.
// // Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// // See accompanying file LICENSE.rst or
// // http://www.steinwurf.com/licensing

#include <fifi/field_metric.hpp>
#include <fifi/finite_field.hpp>
#include <fifi/math.hpp>
#include <gtest/gtest.h>

/// Field Metrics
/// -------------
///
/// If you add a member to the field_metric enum class, make sure that it is in
/// the right category (is it field-specific? Acceleration metric? etc.) and
/// make sure to update this test with the indexing. The last member is now
/// field_polynomial and if this changes, makes sure to update this in the
/// number_of_members test.
///
/// If you make changes to the field_metric enum class, you should also be vary
/// of the side-effects in e.g kodo. Kodo inherits the metrics from a fifi::math
/// object, but kodo has its own kodo::field_metric enum class. Make sure to
/// update this and others to reflect the changes in fifi.
TEST(test_field_metric, values)
{
    /// Check that the indexing is correct. Should be zero-indexed
    EXPECT_EQ(
        0U, static_cast<uint64_t>(fifi::field_metric::bytes_processed_no_simd));
    EXPECT_EQ(1U,
              static_cast<uint64_t>(fifi::field_metric::bytes_processed_sse2));
    EXPECT_EQ(2U,
              static_cast<uint64_t>(fifi::field_metric::bytes_processed_ssse3));
    EXPECT_EQ(3U,
              static_cast<uint64_t>(fifi::field_metric::bytes_processed_sse42));
    EXPECT_EQ(4U,
              static_cast<uint64_t>(fifi::field_metric::bytes_processed_avx2));
    EXPECT_EQ(5U,
              static_cast<uint64_t>(fifi::field_metric::bytes_processed_neon));
    EXPECT_EQ(6U, static_cast<uint64_t>(fifi::field_metric::has_sse2));
    EXPECT_EQ(7U, static_cast<uint64_t>(fifi::field_metric::has_ssse3));
    EXPECT_EQ(8U, static_cast<uint64_t>(fifi::field_metric::has_sse42));
    EXPECT_EQ(9U, static_cast<uint64_t>(fifi::field_metric::has_avx2));
    EXPECT_EQ(10U, static_cast<uint64_t>(fifi::field_metric::has_neon));
    EXPECT_EQ(11U, static_cast<uint64_t>(fifi::field_metric::field_degree));
    EXPECT_EQ(12U, static_cast<uint64_t>(fifi::field_metric::field_polynomial));
}

TEST(test_field_metric, number_of_members)
{
    /// Check if members have been added to the enum class. neon is currently
    /// the last member
    fifi::math math(fifi::finite_field::binary8);

    EXPECT_EQ(13U, math.metrics());
}
