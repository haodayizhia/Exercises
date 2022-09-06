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

```cpp
// 优化：中间开始回溯版本
#include <iostream>
#include <memory>
#include <ctime>

using namespace std;

int cal(int c, int v, int count)
{
	// 如果还有电压长度（v）和一个以上的芯片(c)，继续分
	if (c > 1 && v > 0)
	{
		int right_before;
		int left_before;
		// 从中间开始往前找分割点，在i处分割，右侧剩c个芯片，v-i个电压长度，左侧剩c-1个芯片，i-1个电压长度
		for (int i = (v + 1) / 2; i > 0; --i)
		{
			int right = cal(c, v - i, 0);
			int left = cal(c - 1, i - 1, 0);
			if (right >= left)
			{
				// 确定临界点为i还是i+1
				if ((i < (v + 1) / 2) & (right > left_before))
				{
					// i+1处分割，计数加一
					count = 1 + left_before;
					break;
				}
				// i处分割，计数加一
				count = 1 + right;
				break;
			}
			right_before = right;
			left_before = left;
		}
	}
	else
	{
		// 最后按顺序遍历或者芯片为0
		count += v;
	}
	// 测试结束
	return count;
}

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	int count = 0;
	cout << cal(2, 100, count) << endl;
	clock_t end = clock();
	cout << "cost time: " << double(end - begin) / CLOCKS_PER_SEC << endl;
	return 0;
}
```
```cpp
// 减少顶层分割次数，但增加了底层分割次数，劣化版本？
using namespace std;

#include <iostream>
#include <memory>
#include <ctime>
#include <vector>

using namespace std;

int cal(int c, int v, int count)
{
	// 如果还有电压长度（v）和一个以上的芯片(c)，继续分
	if (c > 1 && v > 0)
	{
		// 上一次分割点
		int bpoint = 0;
		// 左右两个边界，每个边界的第一个值代表左侧次数，第二个值代表右侧次数
		vector<int> left_bound(2, 0);
		vector<int> right_bound(2, 0);
		// 在v中寻找临界点
		for (int i = 1, j = v / 2 + 1;;)
		{
			// 在(j+i-1)/2+1处分割
			int point = (j + i - 1) / 2 + 1;
			// 无法再分割
			if (point == bpoint)
				break;
			int left = cal(c - 1, point - 1, 0);
			int right = cal(c, v - point, 0);
			if (left >= right)
			{
				// 在前面
				j = point;
				right_bound = {left, right};
			}
			if (left < right)
			{
				// 在后面
				i = point;
				left_bound = {left, right};
			}
			bpoint = point;
		}
		// 如果没有计算left_bound的值
		if (left_bound[0] == 0)
		{
			left_bound = {0, cal(c, v - 1, 0)};
		}
		// 确定临界点在左边界还是右边界
		if (left_bound[1] < right_bound[0])
			count = 1 + left_bound[1];
		else
			count = 1 + right_bound[0];
	}
	else
	{
		// 最后按顺序遍历或者芯片为0
		count += v;
	}
	// 测试结束
	return count;
}

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	int count = 0;
	cout << cal(2, 100, count) << endl;
	clock_t end = clock();
	cout << "cost time: " << double(end - begin) / CLOCKS_PER_SEC << endl;
	return 0;
}
```

```cpp
// 真·强化版本，深刻认识分割排列规律
#include <iostream>
#include <memory>
#include <ctime>
#include <vector>

using namespace std;

// c个电压，k次，返回最多能测多少个电压
int cal(int c, int k)
{
	int v = 0;
	if (c == 1)
		return k;
	if (k == 0)
		return 0;
	for (int i = k; i > 0; --i)
		v += 1 + cal(c - 1, i - 1);
	return v;
}

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	int c = 0, v = 0, k = 0;
	cin >> c >> v;
	// 寻找电压范围
	while (v > cal(c, k))
		++k;
	cout << k << endl;
	clock_t end = clock();
	cout << "cost time: " << double(end - begin) / CLOCKS_PER_SEC << endl;
	return 0;
}
```

```cpp
// final_version，简化上一个版本
```

## Exercise 2

> 游戏中的任务升级需要X经验，一共有Y个任务以及对应的Z的经验值，请问一共有多少种方法可以升级（每个任务能够完成多次）
>
> 输入：
>
> 6
>
> 1，3
>
> 输出：
>
> 3
>
> 解释
>
> 6=1+1+1+1+1+1
>
> 6=3+1+1+1
>
> 6=3+3
>
> 输入：
>
> 9
>
> 4，4
>
> 输出：
>
> 0

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    // 升级需要的总经验
    int total;
    // 升一级的经验值
    int z;
    // 情况数
    int count = 0;
    // 经验值集合
    vector<int> jy;
    cin >> total;
    while (cin >> z)
        jy.push_back(z);
    // 从小到大排列经验值
    sort(jy.begin(), jy.end());
    int size = jy.size();
    // first可能的经验值，second出现的次数
    map<int, int> value = {{total, 1}};
    for (int i = size; i > 0; --i)
    {
        // 到小的经验值之前
        if (i > 1)
        {
            auto temp_value = value;
            for (auto &j : temp_value)
                for (int k = 1; k <= j.first / jy[i - 1]; ++k)
                {
                    int t = j.first - k * jy[i - 1];
                    if (temp_value.find(t) == temp_value.end())
                        value.insert(pair<int, int>(t, 1));
                    else
                        ++value[t];
                }
        }
        // 最后一个经验值，直接整除
        else
        {
            for (auto &j : value)
                if (!(j.first - j.first / jy[i - 1]))
                    count += j.second;
        }
    }
    std::cout << count << endl;
    return 0;
}
```
