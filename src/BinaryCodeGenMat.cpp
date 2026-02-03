#include "BinaryCodeGenMat/BinaryCodeGenMat.hpp"

#include <algorithm>

void BinaryCodeGenMat::pushRow(const BinaryCodeWord& row) {
    if (m_initialized) {
        throw std::logic_error("BinaryCodeGenMat::pushRow: cannot push after initialize()");
    }
    if (!row.initialized()) {
        throw std::logic_error("BinaryCodeGenMat::pushRow: codeword is uninitialized");
    }
    m_staging.push_back(row);
}

void BinaryCodeGenMat::initialize() {
    if (m_initialized) {
        throw std::logic_error("BinaryCodeGenMat::initialize: already initialized");
    }
    if (m_staging.empty()) {
        throw std::logic_error("BinaryCodeGenMat::initialize: no rows supplied");
    }

    m_length = m_staging[0].length();
    for (const auto& r : m_staging) {
        if (r.length() != m_length) {
            throw std::invalid_argument("BinaryCodeGenMat::initialize: row length mismatch");
        }
    }

    m_numRows = static_cast<int>(m_staging.size());
    m_rows = std::make_unique<BinaryCodeWord[]>(static_cast<std::size_t>(m_numRows));
    for (int i = 0; i < m_numRows; ++i) {
        m_rows[i] = m_staging[i];
    }

    m_initialized = true;
}

int BinaryCodeGenMat::length() const {
    requireInitialized();
    return m_length;
}

int BinaryCodeGenMat::numRows() const {
    requireInitialized();
    return m_numRows;
}

const BinaryCodeWord& BinaryCodeGenMat::operator[](int i) const {
    requireInitialized();
    requireValidIndex(i);
    return m_rows[i];
}

BinaryCodeWord& BinaryCodeGenMat::operator[](int i) {
    requireInitialized();
    requireValidIndex(i);
    return m_rows[i];
}

void BinaryCodeGenMat::swapColumns(std::vector<BinaryCodeWord>& rows, int c1, int c2) {
    if (c1 == c2) return;
    for (auto& r : rows) {
        r.swap(c1, c2); // uses BinaryCodeWord::swap
    }
}

BinaryCodeGenMat BinaryCodeGenMat::getSystematic() const {
    requireInitialized();

    std::vector<BinaryCodeWord> work;
    work.reserve(static_cast<std::size_t>(m_numRows));
    for (int i = 0; i < m_numRows; ++i) {
        work.push_back(m_rows[i]);
    }

    int pivotRow = 0;
    int pivotCol = 0;

    while (pivotRow < static_cast<int>(work.size()) && pivotCol < m_length) {
        int sel = -1;
        for (int r = pivotRow; r < static_cast<int>(work.size()); ++r) {
            if (work[r].getBit(pivotCol) == 1) {
                sel = r;
                break;
            }
        }

        if (sel == -1) {
            ++pivotCol;
            continue;
        }

        if (sel != pivotRow) {
            std::swap(work[sel], work[pivotRow]);
        }

        if (pivotCol != pivotRow) {
            swapColumns(work, pivotCol, pivotRow);
            pivotCol = pivotRow;
        }

        for (int r = 0; r < static_cast<int>(work.size()); ++r) {
            if (r == pivotRow) continue;
            if (work[r].getBit(pivotRow) == 1) {
                work[r] += work[pivotRow];
            }
        }

        ++pivotRow;
        ++pivotCol;
    }

    std::vector<BinaryCodeWord> reduced;
    for (auto& r : work) {
        if (!r.isZero()) {
            reduced.push_back(r);
        }
    }

    BinaryCodeGenMat sys;
    for (auto& r : reduced) {
        sys.pushRow(r);
    }
    sys.initialize();
    return sys;
}

void BinaryCodeGenMat::requireInitialized() const {
    if (!m_initialized) {
        throw std::logic_error("BinaryCodeGenMat: not initialized");
    }
}

void BinaryCodeGenMat::requireValidIndex(int i) const {
    if (i < 0 || i >= m_numRows) {
        throw std::out_of_range("BinaryCodeGenMat: row index out of range");
    }
}
