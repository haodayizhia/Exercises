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
