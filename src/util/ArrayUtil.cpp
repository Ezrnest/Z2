//
// Created by liyicheng on 2018/11/21.
//
namespace ancono {
/**
 * Helper function for storing edges in a vertex.
 */
template<typename T>
T *ensureCapacityAndAdd(T *arr, int &cap, int &n, const T &e) {
    if (n == cap) {
        cap = cap * 3 / 2;
        T *narr = new T[cap];
        for (int i = 0; i < n; i++) {
            narr[i] = arr[i];
        }
        delete[] arr;
        arr = narr;
    }
    arr[n] = e;
    n++;
    return arr;
}


int removeDuplicated(int *arr, int size) {
    int idx = 1;
    int prev = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] != prev) {
            arr[idx] = arr[i];
            prev = arr[i];
            idx++;
        }
    }
    return idx;
}

}
//int sum(int i,int j){
//    return i+j;
//}
//
//int main(){
//    int size;
//    cin >> size;
//    int** mat = readMatrix<int>(size,size);
//    printMatrix(mat,size,size);
//    deleteMatrix(mat,size);
//    mat = initMatrix(size,size,sum);
//    printMatrix(mat,size,size);
//    deleteMatrix(mat,size);
//}