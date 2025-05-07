#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// Bubble Sort Sequential
void bubbleSortSeq(int arr[], int n) {
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Bubble Sort Parallel
void bubbleSortPar(int arr[], int n) {
    for(int i=0; i<n; i++) {
        #pragma omp parallel for
        for(int j=i%2; j<n-1; j+=2) {
            if(arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Merge function
void merge(int arr[], int l, int m, int r) {
    int n1=m-l+1, n2=r-m;
    int L[n1], R[n2];

    for(int i=0;i<n1;i++) L[i]=arr[l+i];
    for(int i=0;i<n2;i++) R[i]=arr[m+1+i];

    int i=0, j=0, k=l;
    while(i<n1 && j<n2)
        arr[k++] = (L[i]<R[j]) ? L[i++] : R[j++];

    while(i<n1) arr[k++] = L[i++];
    while(j<n2) arr[k++] = R[j++];
}

// Merge Sort Sequential
void mergeSortSeq(int arr[], int l, int r) {
    if(l<r) {
        int m=l+(r-l)/2;
        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// Merge Sort Parallel
// void mergeSortPar(int arr[], int l, int r) {
//     if(l<r) {
//         int m=(l+r)/2;
//         #pragma omp parallel sections
//         {
//             #pragma omp section
//             mergeSortPar(arr, l, m);

//             #pragma omp section
//             mergeSortPar(arr, m+1, r);
//         }
//         merge(arr, l, m, r);
//     }
// }


void mergeSortPar(int arr[], int l, int r) {
    int threshold = 1000; // tune this number
    if(l<r) {
        int m=l+(r-l)/2;
        if ((r - l) <= threshold) {
            mergeSortSeq(arr, l, r);  // fallback to sequential
        } 
        else {
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortPar(arr, l, m);

            #pragma omp section
            mergeSortPar(arr, m+1, r);
        }
        merge(arr, l, m, r);
    }
}
}
int main() {
    int n=10000;
    int arr1[n], arr2[n], arr3[n], arr4[n];
    
    // srand(time(0));
    for(int i=0;i<n;i++) {
        arr1[i] = arr2[i] = arr3[i] = arr4[i] = rand()%10000;
    }

    double start, end;

    // Sequential Bubble Sort
    start = omp_get_wtime();
    bubbleSortSeq(arr1, n);
    end = omp_get_wtime();
    double bs=end-start;
    cout << "Sequential Bubble Sort Time: " << bs << " sec" << endl;

    // Parallel Bubble Sort
    start = omp_get_wtime();
    bubbleSortPar(arr2, n);
    end = omp_get_wtime();
    double bp=end-start;
    cout << "Parallel Bubble Sort Time: " << bp << " sec" << endl;
    cout<<"bubble sort speedup: "<<bs/bp<<endl;

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergeSortSeq(arr3, 0, n-1);
    end = omp_get_wtime();
    double ms=end-start;
    cout << "Sequential Merge Sort Time: " << ms << " sec" << endl;

    // Parallel Merge Sort
    start = omp_get_wtime();
    mergeSortPar(arr4, 0, n-1);
    end = omp_get_wtime();
    double mp=end-start;
    cout << "Parallel Merge Sort Time: " << mp << " sec" << endl;
    cout<<"merge sort speedup: "<<ms/mp<<endl;

    return 0;
}