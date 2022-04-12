/*
 * @Author: your name
 * @Date: 2022-03-24 16:04:44
 * @LastEditTime: 2022-03-26 15:22:06
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /datastructure/basic_datastructre/static-Hash-Table/Rabin-Karp-algorithm.cpp
 */
#include <string>
#include <iostream>
#include <cmath>
int hash(const string & s, int l)
{
    int result = s[0] - 48;
    for (size_t i = 1; i < l; i++)
    {
        result += 10 * result + (s[i] - 48);
    }
    return result % 13
}
int function(const std::string & s, const std::string p)
{
    int dp = hash(p, p.size());
    int f = std::pow(10, p.size() - 1) % 13;
    int r = hash(s, p.size() - 1);
    int result = 0;//记录匹配次数
    int i = 0;//计数
    do
    {
        if (r == dp)
        {
            size_t j = 0;
            for (; j < p.size(); i++)
            {
                if (s[i + j] != p[j])
                {
                    break;
                }
            }
            if (j== p.size())
            {
                result++;
            }
        }
        r = (10*(r - f * (s[i] - 48)) + s[i + p.size()]) % 13;
    } while (i < s.size - p.size() + 1);
    return result;
}
int main(int argc, char** argv)
{

    return 0;
}