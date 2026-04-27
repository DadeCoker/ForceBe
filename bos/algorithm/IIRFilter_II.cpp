#include <iostream>
#include "IIRFilter_II.h"


IIR_II::IIR_II()
{
    //ctor
    m_pNum = NULL;
    m_pDen = NULL;
    m_pW = NULL;
    m_num_order = -1;
    m_den_order = -1;
    m_N = 0;
};
 
/** \brief 将滤波器的内部状态清零，滤波器的系数保留
 * \return
 */
void IIR_II::reset()
{
    for(int i = 0; i < m_N; i++)
    {
        m_pW[i] = 0.0;
    }
}
/** \brief
 *
 * \param num 分子多项式的系数，升序排列,num[0] 为常数项
 * \param m 分子多项式的阶数
 * \param den 分母多项式的系数，升序排列,den[0] 为常数项
 * \param m 分母多项式的阶数
 * \return
 */
void IIR_II::setPara(double num[], int num_order, double den[], int den_order)
{
    delete[] m_pNum;
    delete[] m_pDen;
    delete[] m_pW;
    m_num_order = num_order;
    m_den_order = den_order;
    m_N = max(num_order, den_order) + 1;
    m_pNum = new double[m_N];
    m_pDen = new double[m_N];
    m_pW = new double[m_N];
    for(int i = 0; i < m_N; i++)
    {
        m_pNum[i] = 0.0;
        m_pDen[i] = 0.0;
        m_pW[i] = 0.0;
    }
    for(int i = 0; i <= num_order; i++)
    {
         m_pNum[i] = num[i];
    }
    for(int i = 0; i <= den_order; i++)
    {
        m_pDen[i] = den[i];
    }
}
/** \brief 计算 IIR 滤波器的时域响应，不影响滤波器的内部状态
 * \param data_in 为滤波器的输入，0 时刻之前的输入默认为 0，data_in[M] 及之后的输入默认为data_in[M-1]
 * \param data_out 滤波器的输出
 * \param M 输入数据的长度
 * \param N 输出数据的长度
 * \return
 */
void IIR_II::resp(double data_in[], int M, double data_out[], int N)
{
    int i, k, il;
    for(k = 0; k < N; k++)
    {
        data_out[k] = 0.0;
        for(i = 0; i <= m_num_order; i++)
        {
            if( k - i >= 0)
            {
                il = ((k - i) < M) ? (k - i) : (M - 1);
                data_out[k] = data_out[k] + m_pNum[i] * data_in[il];
            }
        }
        for(i = 1; i <= m_den_order; i++)
        {
            if( k - i >= 0)
            {
                data_out[k] = data_out[k] - m_pDen[i] * data_out[k - i];
            }
        }
    }
}
/** \brief 滤波函数，采用直接II型结构
 *
 * \param data 输入数据
 * \return 滤波后的结果
 */
double IIR_II::filter(double data)
{
    m_pW[0] = data;
    for(int i = 1; i <= m_den_order; i++) // 先更新 w[n] 的状态
    {
        m_pW[0] = m_pW[0] - m_pDen[i] * m_pW[i];
    }
    data = 0.0;
    for(int i = 0; i <= m_num_order; i++)
    {
        data = data + m_pNum[i] * m_pW[i];
    }
    for(int i = m_N - 1; i >= 1; i--)
    {
        m_pW[i] = m_pW[i-1];
    }
    return data;
}
/** \brief 滤波函数，采用直接II型结构
 *
 * \param data[] 传入输入数据，返回时给出滤波后的结果
 * \param len data[] 数组的长度
 * \return
 */
void IIR_II::filter(double data[], int len)
{
    int i, k;
    for(k = 0; k < len; k++)
    {
        m_pW[0] = data[k];
        for(i = 1; i <= m_den_order; i++) // 先更新 w[n] 的状态
        {
            m_pW[0] = m_pW[0] - m_pDen[i] * m_pW[i];
        }
        data[k] = 0.0;
        for(i = 0; i <= m_num_order; i++)
        {
            data[k] = data[k] + m_pNum[i] * m_pW[i];
        }
 
        for(i = m_N - 1; i >= 1; i--)
        {
            m_pW[i] = m_pW[i-1];
        }
    }
}
/** \brief 滤波函数，采用直接II型结构
 *
 * \param data_in[] 输入数据
 * \param data_out[] 保存滤波后的数据
 * \param len 数组的长度
 * \return
 */
void IIR_II::filter(double data_in[], double data_out[], int len)
{
    int i, k;
    for(k = 0; k < len; k++)
    {
        m_pW[0] = data_in[k];
        for(i = 1; i <= m_den_order; i++) // 先更新 w[n] 的状态
        {
            m_pW[0] = m_pW[0] - m_pDen[i] * m_pW[i];
        }
        data_out[k] = 0.0;
        for(i = 0; i <= m_num_order; i++)
        {
            data_out[k] = data_out[k] + m_pNum[i] * m_pW[i];
        }
 
        for(i = m_N - 1; i >= 1; i--)
        {
            m_pW[i] = m_pW[i-1];
        }
    }
}

/**
*  function test
*/
void main_1(void)
{
    double b[5] = {0.001836, 0.007344, 0.011016, 0.007344, 0.001836};
    double a[5] = {1.0, -3.0544, 3.8291, -2.2925, 0.55075};
    double x[2] = {1.0, 1.0};
    double y[100];
 
    IIR_II filter;
    filter.setPara(b, 4, a, 4);
    filter.resp(x, 2, y, 100);
 
    for(int i= 0; i< 100; i++)
    {
        cout << y[i] << endl;
    }
}

void main_2(void)
{
    double b[5] = {0.001836, 0.007344, 0.011016, 0.007344, 0.001836};
    double a[5] = {1.0, -3.0544, 3.8291, -2.2925, 0.55075};
    double x[100], y[100];
    int len = 100;
    IIR_I filter;
    //IIR_II filter;
    filter.setPara(b, 4, a, 4);
 
    for (int i = 0; i < len; i++)
    {
        x[i] = 0.0;
        y[i] = 0.0;
    }
    x[0] = 1.0;
    filter.filter(x, y, 100);
    filter.reset();
    filter.filter(x, 100);
 
    for (int i = 0; i < len; i++)
    {
        cout << x[i] <<", " << y[i]<<  endl;
    }
}