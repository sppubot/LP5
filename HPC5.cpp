#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void sequential_lr(const vector<double> &x, const vector<double> &y, double &m, double &c, double &time)
{
    int n = x.size();
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;

    double start = omp_get_wtime();

    for (int i = 0; i < n; ++i)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }

    m = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    c = (sum_y - m * sum_x) / n;

    double end = omp_get_wtime();
    time = end - start;
}

void parallel_lr(const vector<double> &x, const vector<double> &y, double &m, double &c, double &time)
{
    int n = x.size();
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+ : sum_x, sum_y, sum_xy, sum_x2)
    for (int i = 0; i < n; ++i)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }

    m = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    c = (sum_y - m * sum_x) / n;

    double end = omp_get_wtime();
    time = end - start;
}

// int main()
// {
//     omp_set_num_threads(4);  // Set number of threads

//     int n;
//     cout << "Enter the number of data points: ";
//     cin >> n;

//     vector<double> x(n), y(n);

//     cout << "Enter values for x:\n";
//     for (int i = 0; i < n; ++i)
//     {
//         cout << "x[" << i << "]: ";
//         cin >> x[i];
//     }

//     cout << "Enter values for y:\n";
//     for (int i = 0; i < n; ++i)
//     {
//         cout << "y[" << i << "]: ";
//         cin >> y[i];
//     }

//     double m_seq, c_seq, time_seq;
//     double m_par, c_par, time_par;

//     sequential_lr(x, y, m_seq, c_seq, time_seq);
//     parallel_lr(x, y, m_par, c_par, time_par);

//     cout << "\nSequential Execution:\n";
//     cout << "m: " << m_seq << ", c: " << c_seq << ", Time: " << time_seq << "s\n";
//     cout << "Line Equation (Sequential): y = " << m_seq << "x + " << c_seq << endl;

//     cout << "\nParallel Execution:\n";
//     cout << "m: " << m_par << ", c: " << c_par << ", Time: " << time_par << "s\n";
//     cout << "Line Equation (Parallel): y = " << m_par << "x + " << c_par << endl;

//     double speedup = time_seq / time_par;
//     cout << "\nSpeedup: " << speedup << endl;

//     return 0;
// }

int main(){
    omp_set_num_threads(2);//important
    int n=100000;
    vector<double>x(n),y(n);
    for(int i=0;i<n;i++){
        x[i]=rand()%10000;
        y[i]=3*x[i]+7+rand()%50;
    }
    double m_seq,c_seq,time_seq;
    double m_par,c_par,time_par;

    sequential_lr(x,y,m_seq,c_seq,time_seq);
    parallel_lr(x,y,m_par,c_par,time_par);

    cout<<"m seq: "<<m_seq<<"c seq: "<<c_seq<<"time seq: "<<time_seq<<endl;
    cout<<"m par: "<<m_par<<"c par: "<<c_par<<"time par: "<<time_par<<endl;
    double speedup=time_seq/time_par;
    cout<<"speedup: "<<speedup;
}