#ifndef READERCONVERTER_H
#define READERCONVERTER_H

int coo2csc(
  int       ** const row,       /*!< CSC row start indices */
  int       ** const col,       /*!< CSC column indices */
  int const *  const row_coo,   /*!< COO row indices */
  int const *  const col_coo,   /*!< COO column indices */
  int const         nnz,       /*!< Number of nonzero elements */
  int const         n,         /*!< Number of rows/columns */
  int const         isOneBased /*!< Whether COO is 0- or 1-based */
);

/* Reads a MMfile */
int cooReader(char* name, int **CSCrows, int **CSCcols);

#endif