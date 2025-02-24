#include <iostream>
#include <memory> // std::unique_ptr
#include <string> 
#include <vector>

class shape
{
    public:
    virtual std::string getname () = 0;
};

class triangle : public shape
{
    public:
    std::string getname() override { return "triangle";}
};

class square : public shape
{
    public:
    std::string getname() override { return "square";}
};

class circle : public shape
{
    public:
    std::string getname() override { return "circle";}
};

enum Type{ TRIANGLE, SQUARE, CIRCLE};

//responsible for creating and returning shape objects on demand
class factory
{
    public:
    shape* create(Type t) //factory method
    {
        if(t==TRIANGLE)
            return new triangle;
        if(t==SQUARE)
            return new square;
        if(t==CIRCLE)
            return new circle;
    }
};

int main()
{
    std::vector <shape*> shapes;
    //our factory is responsible for creating shape objects on demand
    factory f; 
    //lets create 3 shapes objects and assign their memeory address to our shapes vector  
    shapes.push_back(f.create(TRIANGLE));
    shapes.push_back(f.create(SQUARE));
    shapes.push_back(f.create(CIRCLE));

    //
    for(shape*& s: shapes)
        std::cout<<s->getname()<<std::endl;

    for(shape*& s: shapes)
        delete s;

    shapes.clear();
    return 0;
}