class A
{
protected: void f() {}
};

class B : public A
{
public:
    using A::f;
};

int main (int argc, char* argv[])
{
    A a;
    B b;
    A* c = &b;

    a.f();
    b.f();
    c->f();

    return 0;
}
