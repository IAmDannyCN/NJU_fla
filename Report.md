**221502001 赵子轩**
## 实验完成度
- 完成了 PDA 和 TM 的解析器、模拟器
- 按手册要求完成了 TM 的 verbose 模式
- 完成了 PDA 的 verbose 模式，对非法输入的处理同 TM，对正常的模拟输出格式如下：
```
iad@LAPTOP-H96V848V:~/fla$ ./bin/fla -v ./pda/anbn.pda ab
Input: ab
==================== RUN ====================
State: q0
Input: [a]b
Stack: z
---------------------------------------------
State: q1
Input: a[b]
Stack: 1 z
---------------------------------------------
State: q2
Input: ab[]
Stack: z
---------------------------------------------
State: accept
Input: ab[]
Stack:
---------------------------------------------
true
==================== END ====================
```
- 完成了详细的 syntax error 报错，对于已知问题在 verbose 模式下会给出带行号的详细报错（只对第一个文法错误报错）：
```
syntax error on line 5: Parser error. Probably missing critical characters.
syntax error on line 5: Status `` is empty or contains illegal characters.
syntax error on line 8: Redefinition of input symbol `b`.
syntax error on line 10: Unrecognized identifier `R`.
syntax error on line 16: Redefinition of #q0.
syntax error on line 20: Undefined status `accept_`.
syntax error on line 25: Input lengths must be 1.
syntax error on line 28: Missing the definition for #F.
......
```
- 完成了手册要求的 `case.pda`、`case1.tm` 和 `case2.tm`
- **另编写了 `counter.tm`，通过进位计数器向第一条纸带写入 $10000$ 个 `X`，可以测试出对 STL 容器在扩容时错误处理迭代器导致的问题。**
- 另编写了 `nohalt.tm`，不断循环左移和右移，运行足够时间可以测试出在运行过程中潜在的内存泄漏。
---
## 实验思路
##### 解析器
**我自行实现了一个 `StirngView` 类，维护一个在给定的 `std::string()` 上滑动的可视窗口。** 其中实现了如下的关键函数（另有窗口左截断、查询长度、重载下标等常规函数不在此赘述）：
- `StringView(const string &str, size_t lpos=-1, size_t rpos=-1)`：构造函数
- `StringView sv_before(set<char> stop, set<char> ignore, bool cut=false)`：**查询从当前视窗左端点开始，直到第一次出现 `stop` 中字符为止的子串，然后将其两端在 `ignore` 中的字符删去，最后返回描述这一段子串的 `StringView`。** `cut` 参数代表查询后是否自动左截断至查询结果的右端点。

使用 `StringView::sv_before` 可以使用类似于 `LL(1)` 文法的方式对输入进行解析。例如对于 `#Q = {0,run,halt,accept}`，可以通过以下的调用序列完成解析：
```c
StringView sv(line);
assert(sv[0] == '#')

string identifier =  sv.sv_before({'='}, {' ', '\t'}, true).to_string(); // "Q"
string nothing    =  sv.sv_before({'{'}, {' ', '\t'}, true).to_string(); // ""
while(sv[-1] != '}') {
	string cu = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string(); // "0", "run", ...
}
```

在进行分析时，可以使用 `try`，在 `StringView` 解析失败（直到结尾仍未找到 `stop` 字符）时抛出异常，而后报错 `syntax error` 即可。
##### PDA模拟器
定义见 `./fla-project/pda.h`，按照规则进行模拟即可。
##### TM模拟器
定义见 `./fla-project/tm.h`，按照规则进行模拟即可。使用 `deque<char>` 表示纸带，初始时纸带宽度均为 $1$，未给出初值的格子全部为空。当一个刺头移动至纸带外时，向对应边缘 `push` 一个空格。
##### verbose模式
我的 `verbose` 模式牺牲了一定的运行效率：会将当前栈/纸带做一份完整拷贝，去除多余空格后计算排版距离、整理输出。具体实现见 `./fla-project/tm.c: TM::run`。

---
## 实验问题

> TM 模拟器稳定在 `deque<char>` 长度达到 2560 后触发 Segmentation Fault

我使用迭代器数组保存了每一个磁头的位置，在左移磁头时使用 `it--` 进行更新。但在 `push_front()` 后 `deque` 可能扩容导致迭代器失效，此时需要同步更新迭代器。**于是编写了 `./tm/counter.tm` 用于测试这种 bug。** 也可以使用 `./bin/fla ./tm/case1.tm a(repeat 50 times)b(repeat 50 times)` 触发。

---
## 感想与建议

1. 建议将文档写的更加完备：目前很多 corner case 下的行为（例如：TM输入均为合法字符但不在语言中、是否可以对 verbose 下的文法错误详细报错等）没有明确阐述，需要翻FAQ/提问。

