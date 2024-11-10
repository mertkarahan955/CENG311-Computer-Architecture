#include "matrix.h"

#include <stdlib.h>
#include "matrix.h"

/**
 * This function initializes and returns a dense matrix 
 * as a dynamic two-dimensional array.
 */
DenseMatrix initDenseMatrix(int columnLength, int rowLength) {
    DenseMatrix denseMatrix = (DenseMatrix)malloc(sizeof(DenseMatrix));

    denseMatrix->columnLength = columnLength;
    denseMatrix->rowLength = rowLength;
    
    // for 2d matrix 
    denseMatrix->matrix = (int**)malloc(rowLength * sizeof(int*));


   
    for (int i = 0; i < rowLength; i++) {
        denseMatrix->matrix[i] = (int*)malloc(columnLength * sizeof(int));        
        // init locations to 0
        for (int j = 0; j < columnLength; j++) {
            denseMatrix->matrix[i][j] = 0;
        }
    }
    return denseMatrix;
}

/**
 * This function deallocates all the rows and 
 * columns of the given dense matrix.
 */
void freeDenseMatrix(DenseMatrix denseMatrix) {
   // free all rows
    for (int i = 0; i < denseMatrix->rowLength; i++) {
        free(denseMatrix->matrix[i]);
    }
    // free the pointers
    free(denseMatrix->matrix);

    // free the instance 
    free(denseMatrix);
}

/**
 * This function creates a dense matrix and fills 
 * the elements in the given parse matrix as a set.
 */
DenseMatrix parseMatrixToDenseMatrix(Set parseMatrix, int columnLength, int rowLength) {
   // init a dense matrix
    DenseMatrix denseMatrix = initDenseMatrix(columnLength, rowLength);

    for (int i = 0; i < parseMatrix->cardinality; i++) {
        Element element = parseMatrix->elements[i];

        // we need matrix point
        if (element->type == MATRIX_POINT) {
            // reach the values of matrix point
            int* pointData = (int*)element->data;
            int row = pointData[0];
            int column = pointData[1];
            int value = pointData[2];

          denseMatrix->matrix[row][column] = value;
        }
    }

    return denseMatrix;
}

/**
 * This function creates and returns a parse matrix 
 * as a set depending on the given dense matrix's elements.
 */
Set denseMatrixToParseMatrix(DenseMatrix denseMatrix) {
   // init a  set for parsing the matrix and get elements and values
    Set parseMatrix = initSet();

  
    for (int row = 0; row < denseMatrix->rowLength; row++) {
        for (int col = 0; col < denseMatrix->columnLength; col++) {
            int value = denseMatrix->matrix[row][col];

            // only add non-zero
            if (value != 0) {
                // new matrix point element
                Element matrixPoint = createMatrixPointElement(row, col, value);
                insertElement(parseMatrix, matrixPoint);
            }
        }
    }
    return parseMatrix;
}

/**
 * This matrix creates a new dense matrix, and the 
 * matrix is the addition of the given two matrices.
 */
DenseMatrix addDenseMatrices(DenseMatrix dm1, DenseMatrix dm2) {
    // checking dimensions first for avoiding errors
    if (dm1->rowLength != dm2->rowLength || dm1->columnLength != dm2->columnLength) {
        return NULL;
    }
    DenseMatrix resultMatrix = initDenseMatrix(dm1->columnLength, dm1->rowLength);

    
    for (int row = 0; row < dm1->rowLength; row++) {
        for (int col = 0; col < dm1->columnLength; col++) {
            resultMatrix->matrix[row][col] = dm1->matrix[row][col] + dm2->matrix[row][col];
        }
    }
    return resultMatrix;
}

/**
 * This matrix creates a new sparse matrix as a set, 
 * and the matrix is the addition of the given 
 * two sparse matrices.
 */
Set addSparseMatrices(Set sm1, Set sm2, int columnLength, int rowLength) {
    // init a new set
    Set resultSet = initSet();

    for (int i = 0; i < sm1->cardinality; i++) {
        Element element = sm1->elements[i];
        
        if (element->type == MATRIX_POINT) {
            int* data = (int*)element->data;
            Element newElement = createMatrixPointElement(data[0], data[1], data[2]);
            insertElement(resultSet, newElement);
        }
    }

    for (int i = 0; i < sm2->cardinality; i++) {
        Element element = sm2->elements[i];
        if (element->type == MATRIX_POINT) {
            int* data = (int*)element->data;
            int row = data[0];
            int column = data[1];
            int value = data[2];

         // creating temporary element for adding
            Element tempElement = createMatrixPointElement(row, column, 0);

         
            int fount = 0;
            for (int j = 0; j < resultSet->cardinality; j++) {
                Element resultElement = resultSet->elements[j];
                int* resultData = (int*)resultElement->data;

                if (resultData[0] == row && resultData[1] == column) {
                   // adding the value 
                    resultData[2] += value;
                    fount = 1;
                    break;
                }
            }
          // if nothing fount, insert new element
            if (!fount) {
                Element newElement = createMatrixPointElement(row, column, value);
                insertElement(resultSet, newElement);
            }
        }
    }
    return resultSet;
}