# Exercises

Some little exercises

## Exercise 1

> 给你C(Chips)枚相同的芯片，芯片用于测试的电压挡位从1到V(Volt)。
>
> 我们需要测得该芯片的最高工作电压挡位，记为B（Burned）满足：0<=B<=V，芯片的电压挡位超过B值就会被烧毁，低于或等于B值都不会被烧毁。
>
> 每次操作，你可以取一个完好的芯片，从电压挡位为X值开始测试（满足1<=X<=V)，如果芯片被烧毁了，你就不可以再次使用它，否则可以重复使用。
> 
> 请你计算并输出最坏情况下要测量出B值的最小操作次数
> 
> 输入格式：C，V
>
> 示例1：
> 
> 输入：1，2
> 
> 输出：2
>
> 解释：电压挡位1开始测试，会出现以下情况：
>
> 1）挡位1烧毁，得出B=0，操作次数为1
> 
> 2）挡位1正常，则需要继续测量挡位2
>
>> 2.1）挡位2烧毁，得出B=1，操作次数为2
>
>> 2.2）挡位2正常，得出B=1，操作次数为2
>
> 因此，在最坏的情况下我们需要2次操作以确定B是多少
>
> 示例2：
> 
> 输入：2，6
> 
> 输出：3

```cpp
#include <iostream>
#include <memory>

using namespace std;

int cal(int c, int v, shared_ptr<int> count)
{
	// 如果还有电压长度（v）和一个以上的芯片(c)，继续分
	if (c > 1 && v > 0)
	{
		// 在i处分割，右侧剩c个芯片，v-i个电压长度，左侧剩c-1个芯片，i-1个电压长度，从左开始找临界点
		for (int i = 1; i <= v; ++i)
		{
			if (cal(c, v - i, make_shared<int>(0)) <= cal(c - 1, i - 1, make_shared<int>(0)))
			{
				// 确定临界点为i还是i-1，在临界点上测试
				if (i > 1 && cal(c, v - (i - 1), make_shared<int>(0)) <= cal(c - 1, i - 1, make_shared<int>(0)))
				{
					// i-1处分割，计数加一
					++(*count);
					cal(c, v - (i - 1), count);
					break;
				}
				else
				{
					// i处分割，计数加一
					++(*count);
					cal(c - 1, i - 1, count);
					break;
				}
			}
		}
	}
	else
	{
		// 最后按顺序遍历或者芯片为0
		*count += v;
	}
	// 测试结束
	return *count;
}

int main(int argc, char *argv[])
{
	shared_ptr<int> count = make_shared<int>(0);
	cout << cal(2, 10, count) << endl;
	return 0;
}
```
