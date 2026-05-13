/* Matrix math. */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algo_matrix.h"

/* This could be reduced to a single malloc if it mattered. */
Matrix alloc_matrix(int rows, int cols) {
  Matrix m;
  m.rows = rows;
  m.cols = cols;
  m.data = (double**) malloc(sizeof(double*) * m.rows);
  for (int i = 0; i < m.rows; ++i) {
    m.data[i] = (double*) malloc(sizeof(double) * m.cols);
    assert(m.data[i]);
    for (int j = 0; j < m.cols; ++j) {
      m.data[i][j] = 0.0;
    }
  }
  return m;
}

void free_matrix(Matrix m) {
  assert(m.data != NULL);
  for (int i = 0; i < m.rows; ++i) {
    free(m.data[i]);
  }
  free(m.data);
}

void set_matrix(Matrix m, ...) {
  va_list ap;
  va_start(ap, m);
  
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      m.data[i][j] = va_arg(ap, double);
    }
  }
  
  va_end(ap);
}

void set_identity_matrix(Matrix m) {
  assert(m.rows == m.cols);
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      if (i == j) {
	m.data[i][j] = 1.0;
      } else {
	m.data[i][j] = 0.0;
      }
    }
  }
}

void copy_matrix(Matrix source, Matrix destination) {
  assert(source.rows == destination.rows);
  assert(source.cols == destination.cols);
  for (int i = 0; i < source.rows; ++i) {
    for (int j = 0; j < source.cols; ++j) {
      destination.data[i][j] = source.data[i][j];
    }
  }
}

void print_matrix(Matrix m) {
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      if (j > 0) {
	//printf(" ");
      }
      //printf("%6.2f", m.data[i][j]);
    }
    //printf("\n");
  }
}

void add_matrix(Matrix a, Matrix b, Matrix c) {
  assert(a.rows == b.rows);
  assert(a.rows == c.rows);
  assert(a.cols == b.cols);
  assert(a.cols == c.cols);
  for (int i = 0; i < a.rows; ++i) {
    for (int j = 0; j < a.cols; ++j) {
      c.data[i][j] = a.data[i][j] + b.data[i][j];
    }
  }
}

void subtract_matrix(Matrix a, Matrix b, Matrix c) {
  assert(a.rows == b.rows);
  assert(a.rows == c.rows);
  assert(a.cols == b.cols);
  assert(a.cols == c.cols);
  for (int i = 0; i < a.rows; ++i) {
    for (int j = 0; j < a.cols; ++j) {
      c.data[i][j] = a.data[i][j] - b.data[i][j];
    }
  }
}

void subtract_from_identity_matrix(Matrix a) {
  assert(a.rows == a.cols);
  for (int i = 0; i < a.rows; ++i) {
    for (int j = 0; j < a.cols; ++j) {
      if (i == j) {
	a.data[i][j] = 1.0 - a.data[i][j];
      } else {
	a.data[i][j] = 0.0 - a.data[i][j];
      }
    }
  }
}

void multiply_matrix(Matrix a, Matrix b, Matrix c) {
  assert(a.cols == b.rows);
  assert(a.rows == c.rows);
  assert(b.cols == c.cols);
  for (int i = 0; i < c.rows; ++i) {
    for (int j = 0; j < c.cols; ++j) {
      /* Calculate element c.data[i][j] via a dot product of one row of a
	 with one column of b */
      c.data[i][j] = 0.0;
      for (int k = 0; k < a.cols; ++k) {
	c.data[i][j] += a.data[i][k] * b.data[k][j];
      }
    }
  }
}

/* This is multiplying a by b-tranpose so it is like multiply_matrix
   but references to b reverse rows and cols. */
void multiply_by_transpose_matrix(Matrix a, Matrix b, Matrix c) {
  assert(a.cols == b.cols);
  assert(a.rows == c.rows);
  assert(b.rows == c.cols);
  for (int i = 0; i < c.rows; ++i) {
    for (int j = 0; j < c.cols; ++j) {
      /* Calculate element c.data[i][j] via a dot product of one row of a
	 with one row of b */
      c.data[i][j] = 0.0;
      for (int k = 0; k < a.cols; ++k) {
	c.data[i][j] += a.data[i][k] * b.data[j][k];
      }
    }
  }
}

void transpose_matrix(Matrix input, Matrix output) {
  assert(input.rows == output.cols);
  assert(input.cols == output.rows);
  for (int i = 0; i < input.rows; ++i) {
    for (int j = 0; j < input.cols; ++j) {
      output.data[j][i] = input.data[i][j];
    }
  }
}

int equal_matrix(Matrix a, Matrix b, double tolerance) {
  assert(a.rows == b.rows);
  assert(a.cols == b.cols);
  for (int i = 0; i < a.rows; ++i) {
    for (int j = 0; j < a.cols; ++j) {
      if (abs(a.data[i][j] - b.data[i][j]) > tolerance) {
	return 0;
      }
    }
  }
  return 1;
}

void scale_matrix(Matrix m, double scalar) {
  assert(scalar != 0.0);
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      m.data[i][j] *= scalar;
    }
  }
}

void swap_rows(Matrix m, int r1, int r2) {
  assert(r1 != r2);
  double* tmp = m.data[r1];
  m.data[r1] = m.data[r2];
  m.data[r2] = tmp;
}

void scale_row(Matrix m, int r, double scalar) {
  assert(scalar != 0.0);
  for (int i = 0; i < m.cols; ++i) {
    m.data[r][i] *= scalar;
  }
}

/* Add scalar * row r2 to row r1. */
void shear_row(Matrix m, int r1, int r2, double scalar) {
  assert(r1 != r2);
  for (int i = 0; i < m.cols; ++i) {
    m.data[r1][i] += scalar * m.data[r2][i];
  }
}

/* Uses Gauss-Jordan elimination.

   The elimination procedure works by applying elementary row
   operations to our input matrix until the input matrix is reduced to
   the identity matrix.
   Simultaneously, we apply the same elementary row operations to a
   separate identity matrix to produce the inverse matrix.
   If this makes no sense, read wikipedia on Gauss-Jordan elimination.
   
   This is not the fastest way to invert matrices, so this is quite
   possibly the bottleneck. */
int destructive_invert_matrix(Matrix input, Matrix output) {
  assert(input.rows == input.cols);
  assert(input.rows == output.rows);
  assert(input.rows == output.cols);

  set_identity_matrix(output);

  /* Convert input to the identity matrix via elementary row operations.
     The ith pass through this loop turns the element at i,i to a 1
     and turns all other elements in column i to a 0. */
  for (int i = 0; i < input.rows; ++i) {
    if (input.data[i][i] == 0.0) {
      /* We must swap rows to get a nonzero diagonal element. */
      int r;
      for (r = i + 1; r < input.rows; ++r) {
		if (input.data[r][i] != 0.0) {
			break;
		}
      }
      if (r == input.rows) {
	/* Every remaining element in this column is zero, so this
	   matrix cannot be inverted. */
	return 0;
      }
      swap_rows(input, i, r);
      swap_rows(output, i, r);
    }

    /* Scale this row to ensure a 1 along the diagonal.
       We might need to worry about overflow from a huge scalar here. */
    double scalar = 1.0 / input.data[i][i];
    scale_row(input, i, scalar);
    scale_row(output, i, scalar);

    /* Zero out the other elements in this column. */
    for (int j = 0; j < input.rows; ++j) {
      if (i == j) {
	continue;
      }
      double shear_needed = -input.data[j][i];
      shear_row(input, j, i, shear_needed);
      shear_row(output, j, i, shear_needed);
    }
  }
  
  return 1;
}
