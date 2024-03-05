#include "../async/async.h"
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <fstream>
using namespace std;

mutex mut; // 互斥锁

// 某可并发的耗时任务(单参无返回值函数)
void task(int n)
{
	mut.lock();
	cout << "\033[" << n % 8 + 30 << "m任务" << n << "开始执行\033[0m" << endl;
	mut.unlock();
	this_thread::sleep_for(2s);
	mut.lock();
	cout << "\033[" << n % 8 + 30 << "m任务" << n << "执行结束\033[0m" << endl;
	mut.unlock();
}

// 一般情况(多参有返回值函数)
// 用于读取文件的某一行
string getlinefrom(const string &path, int row)
{
	ifstream ifs;
	ifs.open(path, ios::in);
	if (!ifs.is_open())
	{
		mut.lock();
		cout << "文件" << path << "打开失败" << endl;
		mut.unlock();
		return "";
	}
	int rowcount = 1;
	string res = "";
	while (!ifs.eof())
	{
		getline(ifs, res);
		if (rowcount == row)
			break;
		rowcount++;
	}
	ifs.close();
	return res;
}

int main()
{
	// 创建线程池
	AsyncThreadPool pool{10};

	// 线程池的使用
	{
		// 创建可调用对象泛型包装器
		auto f = [](void *args) -> void *
		{
			task(*(int *)args);
			return nullptr;
		};

		// 将任务加至队列
		for (int i = 0; i < 30; i++)
		{
			int *args = new int;
			*args = i;
			pool.push(f, args);
		}
	}
	this_thread::sleep_for(10s);
	cout << "-------------------------------------" << endl;
	// 泛型包装器使用
	{
		// 对于多参有返回值函数的包装
		// 对参数列表的包装
		struct Args
		{
			string path;
			int row;
		} args;
		// 对可调用对象 getlinefrom 的包装
		auto f = [](void *arg) -> void *
		{
			Args args = *(Args *)arg;
			string *ret = new string;
			*ret = getlinefrom(args.path, args.row);
			return ret;
		};
		// 向线程池添加任务
		// 在异步任务执行后，异步线程会回收此处用于传递参数的堆指针
		pool.push(f, new Args{.path = "test.txt", .row = 2}, "r2");
		pool.push(f, new Args{.path = "test.txt", .row = 4}, "r4");
		pool.push(f, new Args{.path = "test.txt", .row = 5}, "r5");

		// 获取异步任务结果
		// 由于所获得的数据指针是堆指针，因此建议使用智能指针接受返回值以避免内存泄漏
		string r2 = *shared_ptr<string>{(string *)pool.getret("r2")};
		string r4 = *shared_ptr<string>{(string *)pool.getret("r4")};
		string r5 = *shared_ptr<string>{(string *)pool.getret("r5")};

		// 输出读取结果
		cout << "r2: " << r2 << endl;
		cout << "r4: " << r4 << endl;
		cout << "r5: " << r5 << endl;
	}

	// 异步任务线程对象析构时会等待异步任务队列执行完毕
	return 0;
}