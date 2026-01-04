#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    // 类内只声明
    static int num_a;

    A() {
        ++num_a;
    }
    // TODO: 修改为虚析构函数，以支持多态删除
    virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

// 在类外定义并初始化静态字段
int A::num_a = 0;

struct B final : public A {
    // TODO: 正确初始化静态字段
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

// 在类外定义并初始化静态字段
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    // b 是 B 类型，而 B 继承自 A，所以构造 b 时会先构造 A
    // 因此此时 num_a 为 2 (一个是 a，一个是 b 里的基类部分)
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // 使用 dynamic_cast 进行动态类型转换
    B &bb = dynamic_cast<B&>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab; // 这里的 delete 会先调用 ~B() 再调用 ~A()，前提是 ~A 是虚函数
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}