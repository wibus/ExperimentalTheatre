#ifndef CELLARWORKBENCH_MATRIXSTACK_H
#define CELLARWORKBENCH_MATRIXSTACK_H

#include <stack>


namespace cellar
{
    template <typename M>
    class MatrixStack
    {
    public:
        MatrixStack();
        explicit MatrixStack(const M& mat);
        virtual  ~MatrixStack();

        // Stack operations
        virtual void     push();
        virtual void     pop();
        virtual const M& top() const;
        virtual void     clear();
        bool             isEmpty() const;

        // Matrix specific
        virtual void load(const M& mat); // Replace top matrix
        virtual void mult(const M& mat); // Multiply to top matrix and replace it

    private:
        std::stack<M> _stack;
    };



    // IMPLEMENTATION //
    template <typename M>
    MatrixStack<M>::MatrixStack() :
        _stack()
    {
        _stack.push( M(1.0) );
    }

    template <typename M>
    MatrixStack<M>::MatrixStack(const M& mat)
    {
        _stack.push( mat );
    }

    template <typename M>
    MatrixStack<M>::~MatrixStack()
    {
    }


    // Stack operations
    template <typename M>
    void MatrixStack<M>::push()
    {
        _stack.push( _stack.top() );
    }

    template <typename M>
    void MatrixStack<M>::pop()
    {
        _stack.pop();
    }

    template <typename M>
    const M& MatrixStack<M>::top() const
    {
        return _stack.top();
    }

    template <typename M>
    void MatrixStack<M>::clear()
    {
        while(!_stack.empty())
            _stack.pop();

        _stack.push( M(1.0) );
    }

    template <typename M>
    inline bool MatrixStack<M>::isEmpty() const
    {
        return _stack.empty();
    }

    // Matrix specific
    template <typename M>
    void MatrixStack<M>::load(const M& mat)
    {
        _stack.top() = mat;
    }

    template <typename M>
    void MatrixStack<M>::mult(const M& mat)
    {
        _stack.top() *= mat;
    }
}

#endif // CELLARWORKBENCH_MATRIXSTACK_H
