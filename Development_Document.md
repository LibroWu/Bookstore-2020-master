# development_document_Libro

## 实现目标

#### 图书管理系统

#### 注意点：鲁棒性

## 文件存储

* 书目数据库
  * 用来存放书目的具体信息
  * 生成的索引在其他文件中储存
* 用户数据库
  * 存放用户信息
* 倒排文档1
  * 基于ISBN的倒排文档
* 倒排文档2
  * 基于书名的倒排文档
* 倒排文档3
  * 基于作者的倒排文档
* 倒排文档4
  * 基于关键字的倒排文档
* 文件raw_log
  * 未经加工的日志文件

* 文件log
  * 用于展示

## 块状链表类*Unrolled_Linked_List* 

#### 拓展功能

内存池

#### 接口

构造函数

* `Unrolled_Linked_List()`

析构函数

* `~Unrolled_Linked_List()`

Add函数

Find函数

Delete函数

## 日志记录类*Watcher*

#### 数据成员

费用记录

#### 接口

构造函数：

* `Watcher(std::string file_name)`

  与输出的文件关联

* `default`

析构函数：

* `~Watcher()`

  关闭输出文件

文件定向函数：

* `void setfile(std::string file_name)`

  设置关联的输出文件，定向前检查是否已经与文件关联，如是，则关闭已经打开的文件后重定向。

输出函数：

* `void print(std::string Message)`
  * 两份文件，一份美观，一份实用

#### 输出格式

要求：美观

## 命令行接受处理类 *Apollo*

构造函数

析构函数

listen函数

* 接收一行命令并处理，返回一个消息传递类*PaperCup*



## 消息传递类 *PaperCup*

##### 成员变量

命令类型 *int command_type*

参数列表 int argc

​				char argv\[6][60]

## root用户类 *Markus*

## clerk用户类 *Conner*

## customer用户类 *Kara*

## Base类 *Base*

