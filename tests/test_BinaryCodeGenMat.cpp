#include <gtest/gtest.h>

#include <BinaryCodeGenMat/BinaryCodeGenMat.hpp>

TEST(BinaryCodeGenMat, DefaultSystematicHasIdentityLeft) {
    BinaryCodeGenMat G(6, 4);

    ASSERT_EQ(G.dim(), 4);
    for (int r = 0; r < G.dim(); ++r) {
        for (int c = 0; c < G.dim(); ++c) {
            const int expected = (r == c) ? 1 : 0;
            EXPECT_EQ(G.row(r).getBit(c), expected);
        }
    }
}

TEST(BinaryCodeGenMat, ZeroRowsRemoved) {
    BinaryCodeGenMat G(5, 3);

    BinaryCodeWord z(5);
    G.setRow(2, z);

    G.createSystematic();
    EXPECT_EQ(G.dim(), 2);
    EXPECT_EQ(G.numRows(), 2);
}

TEST(BinaryCodeGenMat, EncodeMessage) {
    BinaryCodeGenMat G(7, 4);
    ASSERT_EQ(G.dim(), 4);

    BinaryCodeWord m(4);
    m.setBit(1, 1);
    m.setBit(3, 1);

    BinaryCodeWord c = m * G;

    EXPECT_EQ(c.getBit(0), 0);
    EXPECT_EQ(c.getBit(1), 1);
    EXPECT_EQ(c.getBit(2), 0);
    EXPECT_EQ(c.getBit(3), 1);
}

TEST(BinaryCodeGenMat, AugmentDoublesLength) {
    BinaryCodeGenMat G(6, 4);
    BinaryCodeGenMat H = G.augment(G);
    EXPECT_EQ(H.length(), 12);
    EXPECT_EQ(H.dim(), 4);
}

TEST(BinaryCodeGenMat, AddRequiresSameShape) {
    BinaryCodeGenMat A(6, 4);
    BinaryCodeGenMat B(7, 4);
    EXPECT_THROW((void)(A + B), std::invalid_argument);
}
