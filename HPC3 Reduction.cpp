#include<iostream>
#include<omp.h>
#include<cstdlib>
#include<climits>
#include<vector>
// #include<ctime>
using namespace std;
void min_seq(const vector<int>&arr){
    int mini=INT_MAX;
    for(int i=0;i<arr.size();i++){
        if(arr[i]<mini){
            mini=arr[i];
        }
    }
    cout<<"sequential minimum: "<<mini<<endl;
}

void min_par(const vector<int>&arr){
    int minimum=INT_MAX;
    #pragma omp parallel for reduction(min:minimum)
    for(int i=0;i<arr.size();i++){
        if(arr[i]<minimum){
            minimum=arr[i];
        }
    }
    cout<<"parallel minimum: "<<minimum<<endl;
}

void max_seq(const vector<int>&arr){
    int max=INT_MIN;
    for(int i=0;i<arr.size();i++){
        if(arr[i]>max){
            max=arr[i];
        }
    }
    cout<<"sequential maximum: "<<max<<endl;
}

void max_par(const vector<int>&arr){
    int maximum=INT_MIN;
    #pragma omp parallel for reduction(max:maximum)
    for(int i=0;i<arr.size();i++){
        if(arr[i]>maximum){
            maximum=arr[i];
        }
    }
    cout<<"parallel maximum: "<<maximum<<endl;
}

void sum_seq(const vector<int>&arr){
    int sum=0;
    for(int i=0;i<arr.size();i++){
        sum+=arr[i];
    }
    cout<<"sum using sequential: "<<sum<<endl;
}

void sum_par(const vector<int>&arr){
    int sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<arr.size();i++){
        sum+=arr[i];
    }
    cout<<"sum using parallel: "<<sum<<endl;
}

void average_seq(const vector<int>&arr){
    int sum=0;
    int n=arr.size();
    for(int i=0;i<n;i++){
        sum+=arr[i];
    }
    cout<<"average using sequential: "<<(float)sum/n<<endl;
}

void average_par(const vector<int>&arr){
    int sum=0;
    int n=arr.size();
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        sum+=arr[i];
    }
    cout<<"average using parallel: "<<(float)sum/n<<endl;
}
int main(){
    int n;
    cout<<"enter number of array elements:";
    cin>>n;
    vector<int>arr(n,0);
    for(int i=0;i<n;i++){
        arr[i]=rand()%n;
    }
    cout<<" min: \n";
    double start=omp_get_wtime();
    min_seq(arr);
    double end=omp_get_wtime();
    double minimum_seq=end-start;
    cout<<"sequential min time: "<<minimum_seq<<endl;
    
    start=omp_get_wtime();
    min_par(arr);
    end=omp_get_wtime();
    double minimum_par=end-start;
    cout<<"parallel min time: "<<minimum_par<<endl;

    cout<<"min speedup factor: "<<minimum_seq/ minimum_par<<endl;

    start=omp_get_wtime();
    max_seq(arr);
    end=omp_get_wtime();
    double maximum_seq=end-start;
    cout<<"sequential max time: "<<maximum_seq<<endl;

    start=omp_get_wtime();
    max_par(arr);
    end=omp_get_wtime();
    double maximum_par=end-start;
    cout<<"parallel max time: "<<maximum_par<<endl;

    cout<<"maximum speedup factor: "<<maximum_seq/maximum_par<<endl;

    start=omp_get_wtime();
    sum_seq(arr);
    end=omp_get_wtime();
    double sumSeq=end-start;
    cout<<"sequential sum time: "<<sumSeq<<endl;

    start=omp_get_wtime();
    sum_par(arr);
    end=omp_get_wtime();
    double sumpar=end-start;
    cout<<"parallel sum time: "<<sumpar<<endl;

    cout<<"sum speedup factor: "<<sumSeq/sumpar<<endl;

    start=omp_get_wtime();
    average_seq(arr);
    end=omp_get_wtime();
    double avg_seq=end-start;
    cout<<"sequential average time: "<<avg_seq<<endl;

    start=omp_get_wtime();
    average_par(arr);
    end=omp_get_wtime();
    double avg_par=end-start;
    cout<<"parallel average time: "<<avg_par<<endl;

    cout<<"average speedup factor: "<<avg_seq/avg_par<<endl;
}