#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = INCORRECT_MATRIX;

  if (rows > 0 && columns > 0 && result != NULL) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));

    if (result->matrix != NULL) {
      for (int i = 0; i < rows; ++i) {
        result->matrix[i] = calloc(columns, sizeof(double));
        if (result->matrix[i] != NULL) {
          status = OK;
        } else {
          status = INCORRECT_MATRIX;
        }
      }
    } else {
      status = INCORRECT_MATRIX;
    }
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    for (int i = 0; i < A->rows; ++i) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;

  if (A->columns != B->columns || A->rows != B->rows) {
    status = FAILURE;
  } else {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          status = FAILURE;
          break;
        }
      }
    }
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;

  if (A != NULL && B != NULL && result != NULL) {
    if (A->rows == B->rows && A->columns == B->columns) {
      status = s21_create_matrix(A->rows, A->columns, result);
      if (status == OK) {
        for (int i = 0; i < A->rows; ++i) {
          for (int j = 0; j < A->columns; ++j) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
          }
        }
      }
    } else {
      status = CALCULATION_ERROR;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;

  if (A != NULL && B != NULL && result != NULL) {
    if (A->rows == B->rows && A->columns == B->columns) {
      status = s21_create_matrix(A->rows, A->columns, result);
      if (status == OK) {
        for (int i = 0; i < A->rows; ++i) {
          for (int j = 0; j < A->columns; ++j) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          }
        }
      }
    } else {
      status = CALCULATION_ERROR;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = OK;

  if (A != NULL) {
    status = s21_create_matrix(A->rows, A->columns, result);
    if (status == OK) {
      for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->columns; ++j) {
          result->matrix[i][j] = number * A->matrix[i][j];
        }
      }
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;

  if (A != NULL && B != NULL) {
    if (A->rows == B->columns && A->columns == B->rows) {
      status = s21_create_matrix(A->rows, B->columns, result);
      if (status == OK) {
        for (int i = 0; i < result->rows; ++i) {
          for (int j = 0; j < result->columns; ++j) {
            result->matrix[i][j] = 0;
            for (int k = 0; k < A->columns; ++k) {
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
            }
          }
        }
      }
    } else
      status = CALCULATION_ERROR;
  } else
    status = INCORRECT_MATRIX;
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (A != NULL) {
    status = s21_create_matrix(A->columns, A->rows, result);
    if (status == OK) {
      for (int i = 0; i < result->rows; ++i) {
        for (int j = 0; j < result->columns; ++j) {
          result->matrix[i][j] = A->matrix[j][i];
        }
      }
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

void s21_minor_matrix(matrix_t *A, matrix_t *minor, int i) {
  for (int j = 1; j < A->rows; ++j) {
    for (int k = 0; k < A->columns - 1; ++k) {
      if (k < i) {
        minor->matrix[j - 1][k] = A->matrix[j][k];
      } else {
        minor->matrix[j - 1][k] = A->matrix[j][k + 1];
      }
    }
  }
}

int s21_determinant(matrix_t *A, double *result) {
  int status = OK;

  if (A == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows == A->columns && A->rows > 0) {
    double determ = 0.0;
    matrix_t minor = {0};
    if (A->rows == 1) {
      determ = A->matrix[0][0];
    } else if (A->rows == 2) {
      determ =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
      for (int i = 0; i < A->rows; ++i) {
        status = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
        if (status == OK) {
          s21_minor_matrix(A, &minor, i);
          double minor_determ = 0.0;
          s21_determinant(&minor, &minor_determ);
          int sign = ((i + 1) % 2 == 0) ? -1 : 1;
          determ += sign * A->matrix[0][i] * minor_determ;
          s21_remove_matrix(&minor);
        }
      }
    }
    *result = determ;
  } else {
    status = CALCULATION_ERROR;
  }
  return status;
}

void s21_additional_minor(matrix_t *A, matrix_t *minor, matrix_t *result, int n,
                          int *status) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int row = 0;
      for (int k = 0; k < n; ++k) {
        if (k == j) {
          continue;
        }
        int column = 0;
        for (int l = 0; l < n; ++l) {
          if (l == i) {
            continue;
          }
          minor->matrix[row][column] = A->matrix[k][l];
          column++;
        }
        row++;
      }
      double determ = 0.0;
      *status = s21_determinant(minor, &determ);
      if (*status == OK) {
        double sign = pow(-1, i + j);
        result->matrix[j][i] = sign * determ;
      }
    }
    if (*status != OK) {
      break;
    }
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = OK;
  if (A != NULL && result != NULL && A->rows == A->columns) {
    int n = A->rows;
    status = s21_create_matrix(n, n, result);
    if (status == OK) {
      if (n > 1) {
        matrix_t minor = {0};
        status = s21_create_matrix(n - 1, n - 1, &minor);
        if (status == OK) {
          s21_additional_minor(A, &minor, result, n, &status);
          s21_remove_matrix(&minor);
        }
      } else if (n == 1)
        result->matrix[0][0] = 1;
    }
  } else {
    status = CALCULATION_ERROR;
    if (A == NULL || result == NULL) {
      status = INCORRECT_MATRIX;
    }
  }
  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (A->rows > 0 && A->columns > 0) {
    double determ = 0;
    if (A != NULL && result != NULL && A->rows == A->columns &&
        s21_determinant(A, &determ) == OK && determ != 0) {
      matrix_t tmp;
      status = s21_calc_complements(A, &tmp);
      if (status == OK) {
        matrix_t tmp1;
        status = s21_transpose(&tmp, &tmp1);
        s21_remove_matrix(&tmp);
        if (status == OK) {
          s21_mult_number(&tmp1, (1 / determ), result);
          s21_remove_matrix(&tmp1);
        }
      }
    } else {
      status = CALCULATION_ERROR;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}