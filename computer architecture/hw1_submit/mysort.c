// void sort(int* out, int* in, int num){
//     int i, key, j;
//     for (i = 0; i < num; i++) {
//         key = in[i];
//         j = i - 1;

//         while (j >= 0 && out[j] > key) {
//             out[j + 1] = out[j];
//             j = j - 1;
//         }
//         out[j + 1] = key;
//     }
// }

  // 127  riscv64-unknown-elf-gcc -S mysort.c
  // 128  mv mysort.s sort.s 
  // 129  make


void insert(int* out, int num, int in){
  int j = num-1;
  while (j >= 0 && out[j] > in) {
    out[j + 1] = out[j];
    j = j - 1;
  }
  out[j + 1] = in;
}
void sort(int* out, int* in, int num){
  int i;
  for (i = 0; i < num; i++) {
    // key = in[i];
    insert(out, i, in[i]);
    }
}
