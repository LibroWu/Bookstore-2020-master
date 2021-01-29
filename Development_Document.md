# development_document_Libro

## 实现目标

#### 图书管理系统

## 块状链表类*Unrolled_Linked_List* 

### namespace ULL

##### 参数 

* 每块数据长度 MAXN
  * 需要为偶数
* 半长 HALF_MAXN
  * 用于列块
* 合并阈值 MERGE_THRESHOLD

### 模板类 Unrolled_Linked_List

#### 注释

模板参数 int Key_Len，在实例化时确定索引关键字长度

双关键字排序，第一级关键字是key，第二级关键字是存储位置int

#### 成员

```
int block_num,init; //block_num是当前的块数，init的起始指针位置
std::fstream file,memfile;
std::string file_name,memfile_name;  //用于存放关联的数据文件名
```

##### 单索引结构体 Node

*  private
*  char key[Key_len+1]; //索引
*  int pos;   //实际数据在顺序存储文件的起始位置

##### 块类 block

* private
* 数据成员

```
private:
	int next, num;
public:
	Node data[MAXN];
```

* 构造函数

```
	block() : next(0), num(0) {}
```

#### private函数

##### *字符串相关操作*

##### cmp

```
bool cmp(const char a[], const char b[]);
```

若a<b返回true，否则返回false

##### is_same

```
bool is_same(const char a[], const char b[])
```

若a==b返回true，否则返回false

##### *块相关操作*

##### merge_blocks

```
void merge_blocks(const int &cur, int &nxt);
```

将起始位置为nxt的块合并到起始位置cur的块中，回收nxt块的空间到内存池中

##### *内存池相关操作*

##### memfree

```
void mem_free(int &cur);
```

将起始位置为cur的块加入到内存池中

##### mem_allocate

```
void mem_allocate(int &free_num, int &address);
```

从内存池中取出第free_num个地址，将其赋值给address

#### public函数接口

##### 构造函数

```
Unrolled_Linked_List(std::string file_name, std::string memory_pool);
```

初始化file_name,memfile_name及其他参数

##### initialize

```
void initialize(bool op = false);
```

若op==true则是程序第一次打开的初始化（往数据文件中写入块数和起始指针位置），否则是数据文件已创立的初始化（从数据文件中读入块数和起始指针位置）。

##### insert

```
void insert(const char *target, const int &pos);
```

插入一个索引

##### find

```
std::vector<int> &find(const char target[]);
```

找到所有key==target的索引对应的pos，添加到`vector<int>`中

##### Delete

```
void Delete(const char target[], const int &pos);
```

删除一个索引

##### 析构函数

```
~Unrolled_Linked_List();
```

若文件未关闭，关闭之。

## Apollo.h && Apollo.cpp 

##### #命令行读入及字符串处理

### 命令行接受处理类 *Apollo*

#### 函数接口

##### listen函数

* 接收一行命令并处理，返回一个消息传递类*PaperCup*的引用

  ```
  PaperCup& listen();
  ```

* 根据文档要求初步处理接收到的命令。

### 消息传递类 *PaperCup*

##### 成员变量

命令类型 *int command_type*

参数列表 std::stringstream tokens;

command_type与对应command的对照表

```
//command_type
//0 for su, tokens has one or two tokens (user_id and passwd(optional))
//1 for logout, tokens is empty
//2 for useradd, tokens has four tokens (user_id,passwd,level and name)
//3 for register, tokens has three tokens (user_id,passwd and name)
//4 for delete, tokens has one token (user_id)
//5 for passwd, tokens has two or three tokens (user_id,new_pw,old_pw(optional))
//6 for select, tokens has one token ISBN
//7 for modify, tokens has even tokens. eg: "ISBN" will be followed by [ISBN]
//8 for import, tokens has two tokens. (quantity and cost_price)
//9 for show, tokens has zero or two tokens, the same as modify.
//10 for show finance, tokens has zero or one token (time)
//11 for buy, tokens has two tokens (ISBN and quantity)
//12 for report finance
//13 for report employee
//14 for log
//15 for report myself
//-1 for invalid command
//-2 for quit
//besides,each tokens will end up with an eol
//eol="#@$%"
```

### 辅助函数

#### GetLine

```
void GetLine(std::string &s, const char &mark);
```

从stdin中读入以mark为结尾的字符串存入s中，mark将被从缓存区删除

#### Divide

```
bool Divide(const std::string& input,std::string& first,std::string& second);
```

将命令行中的开关拆分，如将`-ISBN=ISBN1`拆分成开关类型`ISBN`和参数`ISBN1`分布存入first和second中

#### DivideKey

```
bool DivideKey(const std::string& input,std::stringstream & output);
```

将包含多个keyword的字符串（关键词之间用`|`隔开）分割，依次存入output中，以一个` `（英文空格）隔开

#### Get_Hash

```
void Get_Hash(const std::string& input,std::string& output);
```

将input转化成一个长度为7的string的散列函数，实现是双hash成两个int，利用union的性质转化成char数组添加到output中（注意每字节不为0）

## user.h && user.cpp

### 注释

### 数据存储类

##### class user

```
static int user_num;
char id[len_id+1], passwd[len_pw+1];
int level;
char name[len_name+1];
```

构造函数

```
user(const char *user_id, const char *pswd, const char *user_name, int user_level);
```

##### class book

```
static int book_num;
int quantity;
char ISBN[len_ISBN+1], name[len_others+1], author[len_others], keywords[len_others+1];
double price;
```

构造函数及拷贝构造

```
book() = default;  //缺省构造
explicit book(const char* isbn,const char * name_="",const char * author_="",const char* keywords_="",const double& price=0,const int quantity=0); //构造函数
book(const book& other);  //拷贝构造
```

重载小于号（用于sort）

```
bool operator<(const book& other);
```

### Base类 *Base*

##### 注释

权限等级为0的登录状态，能执行最低权限0的操作。

##### 数据成员

* private
  * `static const int level = 0;`

* protected
  * `PaperCup *receive;`

##### public函数接口

* `void ferry()`
  * 登录状态

* `void su(std::stringstream &tokens, int level_cur)`
  * 登录到其他账户

* `void register_(std::stringstream &tokens)`
  * 注册一个权限1的账户

### customer用户类 *Kara*

##### 注释

public继承自Base，权限等级为1的登录状态，能执行最低权限1的操作。

##### 数据成员

private

* `static const int level = 1`

protected

* `std::string user_id, passwd, name;`

##### public函数接口

* 缺省构造
  * `Kara() = default;`
* 构造函数
  * `Kara(const std::string &_user_id, const std::string &_passwd, const std::string &_name);`
  * 登录状态

* `void change_passwd(std::stringstream &tokens, int cur_level);`
  * 修改密码操作
* `void show(std::stringstream &tokens);`
  * 显示符合要求的书目信息
* `void buy(std::stringstream &tokens);`
  * 购买书

### clerk用户类 *Conner*

##### 注释

public继承自Kara，权限等级为3的登录状态，能执行最低权限3的操作。

##### 数据成员

private

* `static const int level = 3`

protected

* `int offset`由select操作选中的书本对应的存储位置

##### public函数接口

* `Conner();`
  * 初始化offset
* ` Conner(const std::string &_user_id, const std::string &_passwd, const std::string &_name);`
  * 登录状态
* `void useradd(std::stringstream &tokens, int cur_level);`
  * 添加一个用户
* `void select(std::stringstream &tokens);`
  * 根据ISBN选中或创建
* `void modify(std::stringstream &tokens);`
  * 修改选中书的信息
* `void import(std::stringstream &tokens);`
  * 进货

### root用户类 *Markus*

##### 注释

public继承自Conner，权限等级为7的登录状态，能执行最低权限7的操作。

##### 数据成员

private

* `static const int level = 7`

##### public函数接口

* `Markus(const std::string &_user_id, const std::string &_passwd, const std::string &_name);`
  * 登录状态
* `void Delete(std::stringstream &tokens);`
  * 删除用户
* `void show_finance(std::stringstream &tokens);`
  * 展示财务信息

## Watcher.h&& Watcher.cpp

### 注释

每条记录以record格式存储

```
struct record {
    double sum_cost, sum_profit;  //支出与收入前缀和
    int offset;
    char user_name[31];
};
```

### 日志记录类*Watcher*

#### 数据成员

* private
  * `std::string file_name;`

#### public接口

构造函数：

* `Watcher(const std::string& file_name)`

  与输出的文件关联

* `void init()`

  * 初始化

* `void add_record(const std::string &user_id, int offset, double change);`

  * 添加记录

* `void get_finance(double &cost, double &profit, const int &times);`

  * 展示财务信息

#### 输出格式

要求：美观

## error.h && error.cpp

继承自std::exception的异常类

## 文件存储

* 书目数据库
  * 用来存放书目的具体信息
  * 生成的索引在其他文件中储存
* 用户数据库
  * 存放用户信息
* 倒排文档1
  * 基于ISBN的倒排文档
  * 存储文件名及管理文件名分别为`"ISBN.file","mem_ISBN.file"`
* 倒排文档2
  * 基于书名的倒排文档
  * 存储文件名及管理文件名分别为`"name.file","mem_name.file"`
* 倒排文档3
  * 基于作者的倒排文档
  * 存储文件名及管理文件名分别为`"author.file","mem_author.file"`
* 倒排文档4
  * 基于关键字的倒排文档
  * 存储文件名及管理文件名分别为`"keywords.file","mem_keywords.file"`
* 倒排文档5
  * 基于用户id的倒排文档
  * 存储文件名及管理文件名分别为`"ID.file","mem_ID.file"`
* 文件raw_log
  * 未经加工的日志文件
  * 文件名`"Nights_watch.file"`