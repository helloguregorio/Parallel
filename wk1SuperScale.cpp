#include <Windows.h>
#include <iostream>
#include <ctime>
//平凡算法
int ordinarilySum(int *arr, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return sum;
}
//多链路
int doubleLinkSum(int *arr, int len)
{
    int sum = 0;
    int sum1 = 0;
    int sum2 = 0;
    int i = 0;
    for (; i < len; i += 2)
    {
        sum1 += arr[i];
        sum2 += arr[i + 1];
    }
    if (i == len - 2)
    {
        sum1 += arr[i + 1];
    }
    return sum1 + sum2;
}
//十链路
int TenLinkSumWithoutArr(int *arr, int len)
{
    int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0;
    int i = 0;
    for (; i < len; i += 10)
    {
        sum1 += arr[i];
        sum2 += arr[i + 1];
        sum3 += arr[i + 2];
        sum4 += arr[i + 3];
        sum5 += arr[i + 4];
        sum6 += arr[i + 5];
        sum7 += arr[i + 6];
        sum8 += arr[i + 7];
        sum9 += arr[i + 8];
        sum10 += arr[i + 9];
    }
    return sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8 + sum9 + sum10;
}
int TenLinkSum(int *arr, int len)
{
    int sumArr[10] = {0};
    int i = 0;
    for (; i < len; i += 10)
    {
        sumArr[0] += arr[i];
        sumArr[1] += arr[i + 1];
        sumArr[2] += arr[i + 2];
        sumArr[3] += arr[i + 3];
        sumArr[4] += arr[i + 4];
        sumArr[5] += arr[i + 5];
        sumArr[6] += arr[i + 6];
        sumArr[7] += arr[i + 7];
        sumArr[8] += arr[i + 8];
        sumArr[9] += arr[i + 9];
    }
    for (int j = 0; i < len; j++)
    {
        sumArr[j] += arr[i++];
    }
    return ordinarilySum(sumArr, 10);
}
//多链路
int multiLinkSum(int *arr, int len, int linkNum)
{
    int sum = 0;
    int *sumArr = new int[linkNum]{0};
    int i = 0;
    for (; i < len; i += linkNum)
    {
#pragma unroll linkNum
        for (int j = 0; j < linkNum; j++)
        {
            sumArr[j] += arr[i + j];
        }
    }
    for (int j = 0; i < len; j++)
    {
        sumArr[j] += arr[i++];
    }
    int ret = ordinarilySum(sumArr, linkNum);
    delete[] sumArr;
    return ret;
}
//递归
void assitantRecursion(int *arr, int len)
{
    if (len <= 1)
    {
        return;
    }
    else
    {
        for (int i = 0; i < len / 2; i++)
        {
            arr[i] += arr[len - i - 1];
        }
        len /= 2;
        assitantRecursion(arr, len);
    }
}
int recursionSum(int *arr, int len)
{
    int *tmp = new int[len];
    memcpy(tmp, arr, sizeof(int) * len);
    assitantRecursion(tmp, len);
    int ret = tmp[0];
    delete[] tmp;
    return ret;
}
//二重循环
int doubleLoopSum(int *arr, int len)
{
    int *tmp = new int[len];
    for (int i = len; len > 1; len /= 2)
    {
        for (int j = 0; j < i / 2; j++)
        {
            tmp[j] = arr[j * 2] + arr[j * 2 + 1];
        }
    }
    int ret = tmp[0];
    delete[] tmp;
    return ret;
}
// metatemplateprogramming

template <int len>
struct SUM
{
    static const int result(int *arr)
    {
        return arr[len - 1] + SUM<len - 1>::result(arr);
    }
};
template <>
struct SUM<1>
{
    static const int result(int *arr)
    {
        return arr[0];
    }
};

int *getRandomArr(int len)
{
    srand((int)time(0));
    int *arr = new int[len];
    for (int i = 0; i < len; i++)
    {
        arr[i] = rand() % 100;
    }
    return arr;
}

void performanceTest(int times, int (*function)(int *, int))
{
    long long start, finish;
    int freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    int step = 10;
    for (int i = 0; i <= times; i += step)
    {
        int repeat = i <= 100 ? 100 : 10;
        int *arr = getRandomArr(i);
        QueryPerformanceCounter((LARGE_INTEGER *)&start);
        for (int j = 0; j < repeat; j++)
        {
            int sum = (*function)(arr, i);
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&finish);
        double seconds = (finish - start) / repeat;
        std::cout << "scale: " << i << " "
                  << "times: " << repeat << " "
                  << "consume/s: " << seconds / freq << std::endl;
        delete[] arr;
        if (i == 100)
        {
            step = 100;
        }
        if (i == 100000)
        {
            step = 10000;
        }
    }
}
int main()
{
    //  constexpr int SCALE = 1000000;
    //  performanceTest(SCALE, ordinarilySum);
    //  performanceTest(SCALE, doubleLinkSum);
    //  performanceTest(SCALE, recursionSum);
    //  performanceTest(SCALE, doubleLoopSum);
    //  performanceTest(SCALE, TenLinkSumWithoutArr);

    int arr[100];
    for (int i = 0; i < 100; i++)
    {
        arr[i] = i;
    }
    long long start, finish;
    int freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    QueryPerformanceCounter((LARGE_INTEGER *)&start);
    int sum = SUM<100>::result(arr);
    QueryPerformanceCounter((LARGE_INTEGER *)&finish);
    std::cout << (finish - start) / (double)freq << std::endl;
    return 0;
}