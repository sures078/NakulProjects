#include "matvec.h"
#include <stdlib.h>

int optimized_matrix_trans_mult_vec(matrix_t mat, vector_t vec, vector_t res) {
  //error checking
  if(mat.rows != vec.len){
    printf("mat.rows (%ld) != vec.len (%ld)\n",mat.rows,vec.len);
    return 1;
  }

  //error checking
  if(mat.cols != res.len){
    printf("mat.cols (%ld) != res.len (%ld)\n",mat.cols,res.len);
    return 2;
  }

  int vals[mat.cols]; //will temporarily store all results

  for (int index = 0; index < mat.cols; index++) { //zeros out vals
    vals[index] = 0;
  }

  int i, j;

  for(i = 0; i < mat.rows; i++) { //ITERATING OUTER LOOP BY ROWS
    for(j = 0; j < mat.cols; j+=4) { //STEP SIZE INCREASED TO 4
      int vecNum = VGET(vec,i); //vec at index i, will be used to multiply by first 4 elements in mat

      //LOOP UNROLLING
      int matNum1 = MGET(mat,i,j);
      int product1 = matNum1 * vecNum;
      vals[j] += product1; //adds on product1 to current value at vals[j]

      int matNum2 = MGET(mat,i,j+1);
      int product2 = matNum2 * vecNum;
      vals[j+1] += product2; //adds on product2 to current value at vals[j+1]

      int matNum3 = MGET(mat,i,j+2);
      int product3 = matNum3 * vecNum;
      vals[j+2] += product3; //adds on product3 to current value at vals[j+2]

      int matNum4 = MGET(mat,i,j+3);
      int product4 = matNum4 * vecNum;
      vals[j+3] += product4; //adds on product4 to current value at vals[j+3]
    }
    for (; j < mat.cols; j++) { //cleans up any entries not multiplied
      int matNum = MGET(mat,i,j);
      int vecNum = VGET(vec,j);
      int product = matNum * vecNum;
      vals[j] += product;
    }
  }

  for (int k = 0; k < mat.cols; k++) { //sets res = vals
    VSET(res, k, vals[k]);
  }

return 0;

}
