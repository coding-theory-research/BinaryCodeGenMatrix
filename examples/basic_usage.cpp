#include <iostream>

#include <BinaryCodeGenMat/BinaryCodeGenMat.hpp>

int main() {
    // Create some codewords
    BinaryCodeWord r1(6);
    r1.setBit(0, 1);
    r1.setBit(3, 1);

    BinaryCodeWord r2(6);
    r2.setBit(1, 1);
    r2.setBit(4, 1);

    // Duplicate of r1 (linearly dependent)
    BinaryCodeWord r3 = r1;

    // Build generator matrix from supplied rows
    BinaryCodeGenMat G;
    G.pushRow(r1);
    G.pushRow(r2);
    G.pushRow(r3);
    r1.swap(0,1);
    r2.swap(0,1);
    G.initialize();

    std::cout << "Original matrix:\n";
    std::cout << "numRows = " << G.numRows() << "\n";
    for (int i = 0; i < G.numRows(); ++i) {
        std::cout << "G[" << i << "] = " << G[i] << "\n";
    }

    // Compute systematic form (new matrix)
    BinaryCodeGenMat S = G.getSystematic();
    r1.swap(0,1);
    r2.swap(0,1);
    std::cout << "\nSystematic matrix:\n";
    std::cout << "numRows = " << S.numRows() << "\n";
    for (int i = 0; i < S.numRows(); ++i) {
        std::cout << "S[" << i << "] = " << S[i] << "\n";
    }

    return 0;
}
