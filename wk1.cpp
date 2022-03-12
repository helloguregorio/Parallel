#include <Windows.h>
#include <iostream>
int **getMatrix(int n)
{
    int **matrix = new int *[n];
    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[n];
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = i + j;
        }
    }
    return matrix;
}

void delMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int *getVec(int n)
{
    int *vec = new int[n];
    for (int i = 0; i < n; i++)
    {
        vec[i] = i;
    }
    return vec;
}
//平凡算法
int *ordinarilyDotProduct(int n, int **matrix, int *vec)
{
    int *sum = new int[n]{};
    for (int colPivot = 0; colPivot < n; colPivot++)
    {
        for (int rowPivot = 0; rowPivot < n; rowPivot++)
        {
            sum[colPivot] += matrix[rowPivot][colPivot] * vec[rowPivot];
        }
    }
    return sum;
}
//行主算法
int *optimizedDotProduct(int n, int **matrix, int *vec)
{
    int *sum = new int[n]{};
    for (int rowPivot = 0; rowPivot < n; rowPivot++)
    {
        for (int colPivot = 0; colPivot < n; colPivot++)
        {
            sum[colPivot] += matrix[rowPivot][colPivot] * vec[rowPivot];
        }
    }
    return sum;
}

/*
记得动态删除获得的资源
*/
bool judge(int n)
{
    int **matrix = getMatrix(n);
    int *vec = getVec(n);
    int *sum1 = ordinarilyDotProduct(n, matrix, vec);
    int *sum2 = optimizedDotProduct(n, matrix, vec);
    for (int i = 0; i < n; i++)
    {
        if (sum1[i] != sum2[i])
        {
            return false;
        }
    }
    delete[] sum1, sum2, vec;
    delMatrix(matrix, n);
    return true;
}

void performanceTest(int times, int *(*function)(int, int **, int *))
{
    //定义步长、频率、开始、截至等等
    int step = 10, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    // seconds是求平均的时长
    double seconds = 0;
    long long start, finish;
    //这里i是当次实验规模
    for (int i = 0; i <= times; i += step)
    {
        // repeat是重复次数
        int repeat = i <= 100 ? 100 : 10;
        int **matrix = getMatrix(i);
        int *vec = getVec(i);
        QueryPerformanceCounter((LARGE_INTEGER *)&start);
        for (int j = 0; j < repeat; j++)
        {
            int *sum = (*function)(i, matrix, vec);
            delete[] sum;
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&finish);
        delMatrix(matrix, i);
        delete[] vec;
        seconds = (finish - start) / repeat;
        std::cout << i << " " << seconds / freq << std::endl;
        // std::cout << "scale: " << i << " "
        //           << "times: " << repeat << " "
        //           << "consume/s: " << seconds / freq << std::endl;
        if (i == 100)
        {
            step = 100;
        }
    }
}
//接下来想办法解决模板元问题
//这里DIM是维度，COL是当前所处列。下面的两个是子函数，用来计算matrix某一列与vector的DotProduct。
template <int DIM, int COL>
struct DotProduct
{
    const static int result(int *vec, int **matrix)
    {
        return vec[DIM - 1] * matrix[DIM - 1][COL - 1] + DotProduct<DIM - 1, COL>::result(vec, matrix);
    }
};
template <int COL>
struct DotProduct<1, COL>
{
    const static int result(int *vec, int **matrix)
    {
        return vec[0] * matrix[0][COL - 1];
    }
};
//接下来使用DotProduct
template <int DIM, int COL>
struct getSum
{
    const static void caculate(int *sum, int *vec, int **matrix)
    {
        sum[COL - 1] = DotProduct<DIM, COL>::result(vec, matrix);
        getSum<DIM, COL - 1>::caculate(sum, vec, matrix);
    }
};
template <int DIM>
struct getSum<DIM, 1>
{
    const static void caculate(int *sum, int *vec, int **matrix)
    {
        sum[0] = DotProduct<DIM, 1>::result(vec, matrix);
    }
};

int main()
{
    // const int TIMES = 10000;
    // performanceTest(TIMES, optimizedDotProduct);
    // performanceTest(TIMES, ordinarilyDotProduct);

    //计算用
    //接下来定义时间所用变量
    // constexpr int SCALE=200;
    // long long start, finish;
    // int freq;
    // QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    // int **matrix = getMatrix(SCALE);
    // int *vec = getVec(SCALE);
    // int *sum = new int[SCALE]{};
    // QueryPerformanceCounter((LARGE_INTEGER *)&start);
    // for (int i = 0; i < 10; i++)
    // {
    //     getSum<SCALE, SCALE>::caculate(sum, vec, matrix);
    // }
    // QueryPerformanceCounter((LARGE_INTEGER *)&finish);
    // double time = (finish - start) / 10.0;
    // std::cout << time / freq << std::endl;
    // delete[] sum;
    // delete[] vec;
    // delMatrix(matrix, SCALE);
    return 0;
}