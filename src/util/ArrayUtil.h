//
// Created by liyicheng on 2019/1/3.
//

#ifndef HOMEWORK_ARRAYUTIL_H
#define HOMEWORK_ARRAYUTIL_H
namespace ancono {

template<typename T>
T *copyArr(T *original, int length) {
    T *arr = new T[length];
    for (int i = 0; i < length; i++) {
        arr[i] = original[i];
    }
    return arr;
}


template<typename T>
T **initMatrix(int length1, int length2) {
    T **arr = new T *[length1];
    for (int i = 0; i < length1; i++) {
        arr[i] = new T[length2];
    }
    return arr;
}

template<typename T>
T **initMatrix(int length1, int length2, T init) {
    T **arr = new T *[length1];
    for (int i = 0; i < length1; i++) {
        arr[i] = new T[length2];
        for (int j = 0; j < length2; j++) {
            arr[i][j] = init;
        }
    }
    return arr;
}

template<typename T>
T **initMatrix(int row, int col, T (*supplier)(int, int)) {
    T **arr = new T *[row];
    for (int i = 0; i < row; i++) {
        arr[i] = new T[col];
        for (int j = 0; j < col; j++) {
            arr[i][j] = supplier(i, j);
        }
    }
    return arr;
}


template<typename T>
void deleteMatrix(T **arr, int length) {
    for (int i = 0; i < length; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

template<typename T>
bool equalMatrix(int length1, int length2, T **a, T **b) {
    for (int i = 0; i < length1; i++) {
        for (int j = 0; j < length2; j++) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
T **copyMatrix(T **mat, int row, int col) {
    T **re = new T *[row];
    for (int i = 0; i < row; i++) {
        re[i] = new T[col];
        for (int j = 0; j < col; j++) {
            re[i][j] = mat[i][j];
        }
    }
    return re;
}

template<typename T>
class CyclicBuffer {
private:
    T *data;
    unsigned int maxCapacity;
    unsigned int size;
    int oldestPos;
public:
    explicit CyclicBuffer(unsigned int capacity) {
        data = new T[capacity];
        maxCapacity = capacity;
        size = 0;
        oldestPos = 0;
    }

    CyclicBuffer(const CyclicBuffer &obj) {
        maxCapacity = obj.maxCapacity;
        size = obj.size;
        oldestPos = obj.oldestPos;
        data = copyArr(obj.data, maxCapacity);
    }

    ~CyclicBuffer() {
        delete[] data;
    }

    bool add(T &e) {
        for (int i = 0; i < size; i++) {
            if (data[i] == e) {
                return true;
            }
        }
        if (size == maxCapacity) {
            data[oldestPos++] = e;
            oldestPos %= maxCapacity;
        } else {
            data[size++] = e;
        }
        return false;
    }
};


/**
 * Call quickSort(arr,size) first.
 */
int removeDuplicated(int arr[], int size);

/**
 * Returns the index of the max element that is smaller or equal to target.
 * If no such element exists, -1 will be returned.
 */
template<typename T>
int binarySearchFloor(T *arr, int lo, int hi, const T &target) {
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (target < arr[mid]) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo - 1;
}

/**
 * Returns the index of the min element that is bigger or equal to target.
 * If no such element exists, hi+1 will be returned.
 */
template<typename T>
int binarySearchCeiling(T *arr, int lo, int hi, const T &target) {
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo; // lo == hi
}

}
#endif //HOMEWORK_ARRAYUTIL_H
