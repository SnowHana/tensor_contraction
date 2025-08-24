#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"

/** Matrix test */
TEST_CASE("Matrix basic construction") {
    Matrix M(2, 2, {1, 2, 3, 4});
    REQUIRE(M.at(0, 0) == 1);
    REQUIRE(M.at(0, 1) == 2);
    REQUIRE(M.at(1, 0) == 3);
    REQUIRE(M.at(1, 1) == 4);
}


TEST_CASE("Matrix chanigng entry") {
    Matrix M(2, 2, {1, 2, 3, 4});
    REQUIRE(M.at(0, 0) == 1);
    M.at(0,0) = -1;
    REQUIRE(M.at(0, 0) == -1);
    REQUIRE(M.at(1, 0) == 3);
    REQUIRE(M.at(1, 1) == 4);
}


TEST_CASE("Matrix rectangular construction - 2x3") {
    Matrix M(2, 3, {1, 2, 3,
                    4, 5, 6});
    REQUIRE(M.rows() == 2);
    REQUIRE(M.cols() == 3);

    REQUIRE(M.at(0, 0) == 1);
    REQUIRE(M.at(0, 1) == 2);
    REQUIRE(M.at(0, 2) == 3);

    REQUIRE(M.at(1, 0) == 4);
    REQUIRE(M.at(1, 1) == 5);
    REQUIRE(M.at(1, 2) == 6);
}

TEST_CASE("Matrix rectangular construction - 3x2") {
    Matrix M(3, 2, {10, 20,
                    30, 40,
                    50, 60});
    REQUIRE(M.rows() == 3);
    REQUIRE(M.cols() == 2);

    REQUIRE(M.at(0, 0) == 10);
    REQUIRE(M.at(0, 1) == 20);

    REQUIRE(M.at(1, 0) == 30);
    REQUIRE(M.at(1, 1) == 40);

    REQUIRE(M.at(2, 0) == 50);
    REQUIRE(M.at(2, 1) == 60);
}

TEST_CASE("Matrix single row (1x4)") {
    Matrix M(1, 4, {7, 8, 9, 10});
    REQUIRE(M.rows() == 1);
    REQUIRE(M.cols() == 4);

    REQUIRE(M.at(0, 0) == 7);
    REQUIRE(M.at(0, 1) == 8);
    REQUIRE(M.at(0, 2) == 9);
    REQUIRE(M.at(0, 3) == 10);
}

TEST_CASE("Matrix single column (4x1)") {
    Matrix M(4, 1, {11, 12, 13, 14});
    REQUIRE(M.rows() == 4);
    REQUIRE(M.cols() == 1);

    REQUIRE(M.at(0, 0) == 11);
    REQUIRE(M.at(1, 0) == 12);
    REQUIRE(M.at(2, 0) == 13);
    REQUIRE(M.at(3, 0) == 14);
}

TEST_CASE("Matrix at() throws on out-of-range indices") {
    Matrix M(2, 2, {1, 2,
                    3, 4});

    REQUIRE_THROWS_AS(M.at(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(M.at(0, -1), std::out_of_range);
    REQUIRE_THROWS_AS(M.at(2, 0), std::out_of_range); // row too big
    REQUIRE_THROWS_AS(M.at(0, 2), std::out_of_range); // col too big
}

TEST_CASE("Matrix at() allows modifying entries") {
    Matrix M(2, 2, {0, 0,
                    0, 0});

    M.at(0, 0) = 42;
    M.at(1, 1) = 99;

    REQUIRE(M.at(0, 0) == 42);
    REQUIRE(M.at(1, 1) == 99);
}

/** MatrixCalculator class test */
// ---- Matrix multiplication ----
TEST_CASE("Matrix multiply: 2x3 * 3x2 (normal A*B)") {
    Matrix A(2, 3, { 1, 2, 3,
                     4, 5, 6 });         // 2x3
    Matrix B(3, 2, { 7,  8,
                     9, 10,
                    11, 12 });           // 3x2
    MatrixCalculator calc(A, B);          // A*B is valid

    Matrix C = calc.multiply();
    REQUIRE(C.rows() == 2);
    REQUIRE(C.cols() == 2);

    // Expected:
    // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139,154]
    REQUIRE(C.at(0,0) == 58);
    REQUIRE(C.at(0,1) == 64);
    REQUIRE(C.at(1,0) == 139);
    REQUIRE(C.at(1,1) == 154);
}

TEST_CASE("Matrix multiply with identity") {
    Matrix I(3, 3, { 1,0,0,
                     0,1,0,
                     0,0,1 });
    Matrix X(3, 3, { 2, 3, 4,
                     5, 6, 7,
                     8, 9,10 });

    MatrixCalculator left(I, X);   // I*X = X
    Matrix XL = left.multiply();
    REQUIRE(XL.rows() == 3);
    REQUIRE(XL.cols() == 3);
    REQUIRE(XL.at(0,0) == 2);
    REQUIRE(XL.at(0,1) == 3);
    REQUIRE(XL.at(0,2) == 4);
    REQUIRE(XL.at(2,0) == 8);
    REQUIRE(XL.at(2,1) == 9);
    REQUIRE(XL.at(2,2) == 10);

    MatrixCalculator right(X, I);  // X*I = X
    Matrix XR = right.multiply();
    REQUIRE(XR.at(0,0) == 2);
    REQUIRE(XR.at(0,1) == 3);
    REQUIRE(XR.at(0,2) == 4);
    REQUIRE(XR.at(2,0) == 8);
    REQUIRE(XR.at(2,1) == 9);
    REQUIRE(XR.at(2,2) == 10);
}

TEST_CASE("Matrix multiply with zeros") {
    Matrix Z(2, 3, {0,0,0, 0,0,0});
    Matrix B(3, 2, {1,2, 3,4, 5,6});
    MatrixCalculator calc(Z, B);   // Z*B = 0
    Matrix C = calc.multiply();
    REQUIRE(C.rows() == 2);
    REQUIRE(C.cols() == 2);
    REQUIRE(C.at(0,0) == 0);
    REQUIRE(C.at(0,1) == 0);
    REQUIRE(C.at(1,0) == 0);
    REQUIRE(C.at(1,1) == 0);
}

TEST_CASE("Matrix multiply: 1xN · Nx1 gives 1x1 (dot product)") {
    Matrix row(1, 4, {1, 2, 3, 4});      // 1x4
    Matrix col(4, 1, {5, 6, 7, 8});      // 4x1
    MatrixCalculator calc(row, col);     // (1x4)*(4x1) = 1x1
    Matrix C = calc.multiply();

    REQUIRE(C.rows() == 1);
    REQUIRE(C.cols() == 1);
    // 1*5 + 2*6 + 3*7 + 4*8 = 70
    REQUIRE(C.at(0,0) == 70);
}

TEST_CASE("MatrixCalculator rejects incompatible shapes") {
    Matrix A(2, 2, {1,2,3,4});
    Matrix B(2, 3, {1,2,3, 4,5,6});     // A*B invalid (2x2 * 2x3 is valid actually) — pick truly invalid
    Matrix Cbad(4, 4, { // 16 zeros
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    });

    // Choose a pair that is invalid in both orders, e.g., 2x3 and 4x4
    Matrix A23(2, 3, {1,2,3, 4,5,6});
    // 4x4 already defined as Cbad
    REQUIRE_THROWS_AS(MatrixCalculator(A23, Cbad), std::runtime_error);
}
