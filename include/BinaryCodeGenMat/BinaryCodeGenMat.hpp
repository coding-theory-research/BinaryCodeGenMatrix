#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include <BinaryCodeWord/BinaryCodeWord.hpp>

class BinaryCodeGenMat {
public:
    // Empty, uninitialized matrix
    BinaryCodeGenMat() = default;

    // Supply rows (before initialize)
    void pushRow(const BinaryCodeWord& row);

    // Freeze staged rows into internal array (not vector)
    void initialize();

    bool initialized() const noexcept { return m_initialized; }

    int length() const;
    int numRows() const;
    BinaryCodeWord row(int i) const { return (*this)[i]; }

    // Access ORIGINAL supplied rows
    const BinaryCodeWord& operator[](int i) const;
    BinaryCodeWord& operator[](int i);

    // Return NEW matrix in systematic form
    BinaryCodeGenMat getSystematic() const;

private:
    bool m_initialized{false};
    int m_length{0};
    int m_numRows{0};

    // staging area
    std::vector<BinaryCodeWord> m_staging;

    // stored original rows
    std::unique_ptr<BinaryCodeWord[]> m_rows;

private:
    void requireInitialized() const;
    void requireValidIndex(int i) const;

    static void swapColumns(std::vector<BinaryCodeWord>& rows, int c1, int c2);
};
