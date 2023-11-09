#include <check.h>
#include <stdio.h>

#include "s21_matrix.h"

// create

START_TEST(create_matrix) {
  int result = 0;
  matrix_t A = {0};
  result = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_1) {
  int rows = 0;
  int columns = 10;
  matrix_t m = {0};
  ck_assert_int_eq(s21_create_matrix(rows, columns, &m), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_matrix_2) {
  int rows = 10;
  int columns = 0;
  matrix_t m = {0};
  ck_assert_int_eq(s21_create_matrix(rows, columns, &m), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_natrix_3) {
  int rows = -10;
  int columns = 10;
  matrix_t matrix = {0};
  int status = s21_create_matrix(rows, columns, &matrix);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  if (status == OK) {
    s21_remove_matrix(&matrix);
  }
}
END_TEST

START_TEST(create_matrix_4) {
  int rows = 10;
  int columns = 10;
  matrix_t matrix = {0};
  int status = s21_create_matrix(rows, columns, &matrix);

  if (matrix.matrix) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        ck_assert(fabs(matrix.matrix[i][j]) < 1e-7);
      }
    }
    ck_assert_int_eq(matrix.rows, rows);
    ck_assert_int_eq(matrix.columns, columns);
    ck_assert_int_eq(status, OK);
  } else {
    ck_assert_int_eq(status, CALCULATION_ERROR);
  }

  if (status == OK) {
    s21_remove_matrix(&matrix);
  }
}
END_TEST

// eq

START_TEST(eq_matrix_1) {
  int rows = rand() % 100 + 1;
  int columns = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);
  matrix_t m2 = {0};
  s21_create_matrix(rows, columns, &m2);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      double rand_value = rand() + DBL_MIN;
      m.matrix[i][j] = rand_value;
      m2.matrix[i][j] = rand_value;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &m2), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(eq_matrix_2) {
  int rows = rand() % 100;
  int columns = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);
  matrix_t m2 = {0};
  s21_create_matrix(rows, columns, &m2);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      m.matrix[i][j] = rand() + DBL_MAX;
      m2.matrix[i][j] = rand() + DBL_MAX;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &m2), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(eq_matrix_3) {
  int rows1 = rand() % 100;
  int cols1 = rand() % 100;
  matrix_t m = {0};
  s21_create_matrix(rows1, cols1, &m);
  int rows2 = rand() % 100;
  int cols2 = rand() % 100;
  matrix_t m2 = {0};
  s21_create_matrix(rows2, cols2, &m2);

  for (int i = 0; i < rows1; ++i) {
    for (int j = 0; j < cols1; ++j) {
      m.matrix[i][j] = rand() * DBL_MIN + 1;
    }
  }
  for (int i = 0; i < rows2; ++i) {
    for (int j = 0; j < cols2; ++j) {
      m2.matrix[i][j] = rand() * DBL_MIN;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &m2), FAILURE);
  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
}
END_TEST

// sum

START_TEST(sum_matrix) {
  int rows = rand() % 10 + 1;
  int columns = rand() % 10 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);
  matrix_t m2 = {0};
  s21_create_matrix(rows, columns, &m2);
  matrix_t check = {0};
  s21_create_matrix(rows, columns, &check);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      m.matrix[i][j] = 100;
      m2.matrix[i][j] = 100;
      check.matrix[i][j] = m.matrix[i][j] + m2.matrix[i][j];
    }
  }
  matrix_t result = {0};

  ck_assert_int_eq(s21_sum_matrix(&m, &m2, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &result), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(sum_matrix_1) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 1.25;
  B.matrix[0][0] = 2.25;

  R2.matrix[0][0] = 3.5;

  s21_sum_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sum_matrix_2) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(2, 1, &B);

  A.matrix[0][0] = 1.25;
  B.matrix[0][0] = 2.25;

  int result = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(result, CALCULATION_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_3) {
  matrix_t *A = NULL;
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int result = s21_sum_matrix(A, B, R);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

// sub

START_TEST(sub_matrix_1) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 0.10;
  B.matrix[0][0] = 1.100;

  R2.matrix[0][0] = -1;

  s21_sub_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sub_matrix_2) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(2, 1, &B);

  A.matrix[0][0] = 1.25;
  B.matrix[0][0] = 2.25;

  int result = s21_sub_matrix(&A, &B, &R);

  ck_assert_int_eq(result, CALCULATION_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_3) {
  matrix_t *A = NULL;
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int result = s21_sub_matrix(A, B, R);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

// mult

START_TEST(mult_matrix) {
  int rows = rand() % 100 + 1;
  int columns = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);
  matrix_t m2 = {0};
  s21_create_matrix(columns, rows, &m2);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      m.matrix[i][j] = rand() + DBL_MIN;
    }
  }

  for (int i = 0; i < columns; ++i) {
    for (int j = 0; j < rows; ++j) {
      m2.matrix[i][j] = rand() + DBL_MIN;
    }
  }

  matrix_t check = {0};
  s21_create_matrix(m.rows, m2.columns, &check);

  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m2.columns; ++j) {
      check.matrix[i][j] = 0;
      for (int k = 0; k < m.columns; ++k) {
        check.matrix[i][j] += m.matrix[i][k] * m2.matrix[k][j];
      }
    }
  }

  matrix_t result = {0};
  ck_assert_double_eq(s21_mult_matrix(&m, &m2, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &result), SUCCESS);

  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(mult_matrix_1) {
  matrix_t m1 = {0};
  matrix_t m2 = {0};
  s21_create_matrix(1, 1, &m1);
  s21_create_matrix(2, 2, &m2);

  matrix_t result = {0};
  int mul_code = s21_mult_matrix(&m1, &m2, &result);
  ck_assert_int_eq(mul_code, CALCULATION_ERROR);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_2) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = -1;
  B.matrix[0][0] = 2;

  R2.matrix[0][0] = -2;

  s21_mult_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_matrix_4) {
  matrix_t *A = NULL, *B = NULL, *R = NULL;
  ck_assert_int_eq(s21_mult_matrix(A, B, R), INCORRECT_MATRIX);
}
END_TEST

// num

START_TEST(num_1) {
  int rows = rand() % 10 + 1;
  int columns = rand() % 10 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);
  matrix_t check = {0};
  s21_create_matrix(rows, columns, &check);
  double mult_number = 10.1;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      m.matrix[i][j] = 10.1;
      check.matrix[i][j] = m.matrix[i][j] * mult_number;
    }
  }
  matrix_t result = {0};
  ck_assert_int_eq(s21_mult_number(&m, mult_number, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &result), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(num_2) {
  matrix_t A, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 1.25;

  R2.matrix[0][0] = -5;

  s21_mult_number(&A, -4, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(num_3) {
  matrix_t A, R, R2;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &R2);

  A.matrix[0][0] = 0.25;
  A.matrix[0][1] = 1.25;
  A.matrix[0][2] = 2.25;
  A.matrix[1][0] = 3.25;
  A.matrix[1][1] = 4.25;
  A.matrix[1][2] = 5.25;

  R2.matrix[0][0] = 1;
  R2.matrix[0][1] = 5;
  R2.matrix[0][2] = 9;
  R2.matrix[1][0] = 13;
  R2.matrix[1][1] = 17;
  R2.matrix[1][2] = 21;

  s21_mult_number(&A, 4, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(num_4) {
  matrix_t *B = NULL;
  double num = 0;
  matrix_t *R = NULL;
  int result = s21_mult_number(B, num, R);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

// transpose

START_TEST(transpose_1) {
  matrix_t A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.columns = -1;

  int ret = s21_transpose(&A, &B);
  ck_assert_int_eq(ret, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  ck_assert_int_eq(s21_transpose(NULL, NULL), INCORRECT_MATRIX);
}
END_TEST

START_TEST(transpose_2) {
  int rows = rand() % 10 + 1;
  int columns = rand() % 10 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, columns, &m);

  matrix_t check = {0};
  s21_create_matrix(columns, rows, &check);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      double rand_value = rand() + DBL_MAX;
      m.matrix[i][j] = rand_value;
      check.matrix[j][i] = rand_value;
    }
  }

  matrix_t result = {0};
  ck_assert_int_eq(s21_transpose(&m, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &result), SUCCESS);

  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

// complement

START_TEST(complement_1) {
  matrix_t B, R;
  s21_create_matrix(1, 1, &B);
  B.matrix[0][0] = 5;
  ck_assert_int_eq(s21_calc_complements(&B, &R), OK);
  ck_assert_int_eq(R.matrix[0][0], 1);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(complement_2) {
  matrix_t B, R;
  s21_create_matrix(2, 3, &B);

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 4;
  B.matrix[1][2] = 2;

  int code = s21_calc_complements(&B, &R);

  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(complement_3) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 5;

  s21_calc_complements(&A, &R);

  B.matrix[0][0] = 1;

  ck_assert_int_eq(s21_eq_matrix(&R, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(complement_4) {
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int result = s21_calc_complements(B, R);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(complement_5) {
  matrix_t A, B, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  s21_calc_complements(&A, &B);
  C.matrix[0][0] = 0.0;
  C.matrix[0][1] = 10.0;
  C.matrix[0][2] = -20.0;
  C.matrix[1][0] = 4.0;
  C.matrix[1][1] = -14.0;
  C.matrix[1][2] = 8.0;
  C.matrix[2][0] = -8.0;
  C.matrix[2][1] = -2.0;
  C.matrix[2][2] = 4.0;
  int result = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(result, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(complement_6) {
  matrix_t a = {0};
  matrix_t b = {0};
  ck_assert_int_eq(s21_calc_complements(&a, &b), INCORRECT_MATRIX);
}
END_TEST

// determ

START_TEST(determinant_1) {
  matrix_t A;
  double B, R = 1.25;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 1.25;

  s21_determinant(&A, &B);
  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_2) {
  matrix_t A;
  double B, R = -2;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = -5;
  A.matrix[1][1] = -7;

  s21_determinant(&A, &B);

  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_3) {
  matrix_t A;
  double B, R = -2;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 4;
  A.matrix[0][2] = -6;
  A.matrix[1][0] = -5;
  A.matrix[1][1] = -7;
  A.matrix[1][2] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 5;
  A.matrix[2][2] = -6;

  s21_determinant(&A, &B);

  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_4) {
  matrix_t A;
  double B, R = 18;
  s21_create_matrix(4, 4, &A);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = -3;
  A.matrix[0][2] = -5;
  A.matrix[0][3] = 8;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 4;
  A.matrix[1][3] = -6;
  A.matrix[2][0] = 2;
  A.matrix[2][1] = -5;
  A.matrix[2][2] = -7;
  A.matrix[2][3] = 5;
  A.matrix[3][0] = -4;
  A.matrix[3][1] = 3;
  A.matrix[3][2] = 5;
  A.matrix[3][3] = -6;

  s21_determinant(&A, &B);

  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_5) {
  matrix_t A;
  double B;
  s21_create_matrix(4, 5, &A);
  int result = s21_determinant(&A, &B);
  ck_assert_int_eq(result, CALCULATION_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinate_6) {
  matrix_t *B = NULL;
  double re = 0;
  int result = s21_determinant(B, &re);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

// inverse

START_TEST(inverse) {
  matrix_t A, B, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);
  C.matrix[0][0] = 44300.0 / 367429.0;
  C.matrix[0][1] = -236300.0 / 367429.0;
  C.matrix[0][2] = 200360.0 / 367429.0;
  C.matrix[1][0] = 20600.0 / 367429.0;
  C.matrix[1][1] = 56000.0 / 367429.0;
  C.matrix[1][2] = -156483.0 / 367429.0;
  C.matrix[2][0] = 30900.0 / 367429.0;
  C.matrix[2][1] = 84000.0 / 367429.0;
  C.matrix[2][2] = -51010.0 / 367429.0;
  A.matrix[0][0] = 2.8;
  A.matrix[0][1] = 1.3;
  A.matrix[0][2] = 7.01;
  A.matrix[1][0] = -1.03;
  A.matrix[1][1] = -2.3;
  A.matrix[1][2] = 3.01;
  A.matrix[2][0] = 0;
  A.matrix[2][1] = -3;
  A.matrix[2][2] = 2;
  s21_inverse_matrix(&A, &B);

  int result = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(result, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(inverse_1) {
  matrix_t m = {0};
  matrix_t expected = {0};
  int check, code;
  check = s21_create_matrix(3, 3, &m);
  if (!check) {
    code = s21_create_matrix(3, 3, &expected);
  }
  if (!check && !code) {
    m.matrix[0][0] = 2;
    m.matrix[0][1] = 5;
    m.matrix[0][2] = 7;

    m.matrix[1][0] = 6;
    m.matrix[1][1] = 3;
    m.matrix[1][2] = 4;

    m.matrix[2][0] = 5;
    m.matrix[2][1] = -2;
    m.matrix[2][2] = -3;

    expected.matrix[0][0] = 1;
    expected.matrix[0][1] = -1;
    expected.matrix[0][2] = 1;

    expected.matrix[1][0] = -38;
    expected.matrix[1][1] = 41;
    expected.matrix[1][2] = -34;

    expected.matrix[2][0] = 27;
    expected.matrix[2][1] = -29;
    expected.matrix[2][2] = 24;
    matrix_t result = {0};
    int code = s21_inverse_matrix(&m, &result);

    ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
    ck_assert_int_eq(code, OK);

    s21_remove_matrix(&m);
    s21_remove_matrix(&result);
    s21_remove_matrix(&expected);
  }
}
END_TEST

START_TEST(inverse_2) {
  matrix_t m = {0};
  matrix_t result = {0};
  int check = s21_create_matrix(1, 4, &m);
  if (!check) {
    int code = s21_inverse_matrix(&m, &result);
    ck_assert_int_eq(code, CALCULATION_ERROR);
    s21_remove_matrix(&m);
  }
}
END_TEST

START_TEST(inverse_3) {
  matrix_t m = {0};
  matrix_t result = {0};
  int check = s21_create_matrix(1, 1, &m);
  if (!check) {
    m.matrix[0][0] = 1431.12312331;
    int code = s21_inverse_matrix(&m, &result);
    ck_assert_int_eq(result.matrix[0][0] == (1.0 / m.matrix[0][0]), 1);
    ck_assert_int_eq(code, OK);
    s21_remove_matrix(&m);
    s21_remove_matrix(&result);
  }
}
END_TEST

START_TEST(inverse_4) {
  matrix_t m = {0};
  matrix_t result = {0};
  int check = s21_create_matrix(1, 1, &m);
  if (!check) {
    int code = s21_inverse_matrix(&m, &result);
    ck_assert_int_eq(code, CALCULATION_ERROR);
    s21_remove_matrix(&m);
  }
}
END_TEST

START_TEST(inverse_5) {
  matrix_t m = {0};
  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);
  ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

Suite 

*s21_test_matrix(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tCase = tcase_create("s21_matrix");

  // rceate + remove

  tcase_add_test(tCase, create_matrix);
  tcase_add_test(tCase, create_matrix_1);
  tcase_add_test(tCase, create_matrix_2);
  tcase_add_test(tCase, create_natrix_3);
  tcase_add_test(tCase, create_matrix_4);

  // eq

  tcase_add_test(tCase, eq_matrix_1);
  tcase_add_test(tCase, eq_matrix_2);
  tcase_add_test(tCase, eq_matrix_3);

  // sum_m

  tcase_add_test(tCase, sum_matrix);
  tcase_add_test(tCase, sum_matrix_1);
  tcase_add_test(tCase, sum_matrix_2);
  tcase_add_test(tCase, sum_matrix_3);
  tcase_add_loop_test(tCase, sum_matrix, 0, 10);

  // sub_m

  tcase_add_test(tCase, sub_matrix_1);
  tcase_add_test(tCase, sub_matrix_2);
  tcase_add_test(tCase, sub_matrix_3);

  // mult_n

  tcase_add_loop_test(tCase, num_1, 0, 100);
  tcase_add_test(tCase, num_2);
  tcase_add_test(tCase, num_3);
  tcase_add_test(tCase, num_4);

  // mult_m

  tcase_add_loop_test(tCase, mult_matrix, 0, 100);
  tcase_add_test(tCase, mult_matrix_1);
  tcase_add_test(tCase, mult_matrix_2);
  tcase_add_test(tCase, mult_matrix_4);

  // transpose

  tcase_add_test(tCase, transpose_1);
  tcase_add_loop_test(tCase, transpose_2, 0, 100);

  // complements

  tcase_add_test(tCase, complement_1);
  tcase_add_test(tCase, complement_2);
  tcase_add_test(tCase, complement_3);
  tcase_add_test(tCase, complement_4);
  tcase_add_test(tCase, complement_5);
  tcase_add_test(tCase, complement_6);

  // determ

  tcase_add_test(tCase, determinant_1);
  tcase_add_test(tCase, determinate_6);
  tcase_add_test(tCase, determinant_2);
  tcase_add_test(tCase, determinant_3);
  tcase_add_test(tCase, determinant_4);
  tcase_add_test(tCase, determinant_5);
  tcase_add_test(tCase, determinant_5);

  // inverse

  tcase_add_test(tCase, inverse);
  tcase_add_test(tCase, inverse_1);
  tcase_add_test(tCase, inverse_2);
  tcase_add_test(tCase, inverse_4);
  tcase_add_test(tCase, inverse_5);
  tcase_add_test(tCase, inverse_3);

  suite_add_tcase(suite, tCase);

  return suite;
}

int main() {
  int status;
  Suite *suite = s21_test_matrix();
  SRunner *sRunner = srunner_create(suite);
  srunner_set_fork_status(sRunner, CK_NOFORK);
  srunner_run_all(sRunner, CK_VERBOSE);
  status = srunner_ntests_failed(sRunner);
  srunner_free(sRunner);
  return (status == 0) ? 0 : 1;
}