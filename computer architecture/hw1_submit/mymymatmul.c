
  // 127  riscv64-unknown-elf-gcc -S mysort.c
  // 128  mv mysort.s sort.s 
  // 129  make
void matmul_idx(int* out, int* a, int* b, int dim, int x, int y){
    out[x*dim+y] = 0;
    for(int k=0;k<dim;k++){
        out[x*dim+y] += a[x*dim+k]*b[y+dim*k];
    }
}
void matmul(int* out, int* a, int* b, int dim){
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            matmul_idx(out,a,b,dim,i,j);
        }
    }
}

// void main()
// {
//    int matrix[2][2]={1,2,3,4};     //The first square matrix
//    int matrix2[2][2]={5,6,7,8};    //The second square matrix
//    int new[2][2]={0};              //The result of the multiplication
//    int i,j,k;
//    for(i=0;i<2;i++){
//     for(j=0;j<2;j++){
//       for(k=0;k<2;k++)
//         new[i][j]+=matrix[i][k]*matrix2[k][j]; 
//      } 
//   }
// }
