#ifndef CELLARWORKBENCH_MATRIXSTACK_H
#define CELLARWORKBENCH_MATRIXSTACK_H

#include <stack>


namespace cellar
{

/// Matrix stack working the same way as the old Open GL matrix stack
/// Templated to work with any type of matrix
template <typename M>
class MatrixStack
{
public:
    /// Constructor
    /// \param[in] mat The initial matrix of the stack
    explicit MatrixStack(const M& mat = M());

    /// Destructor
    virtual ~MatrixStack();

    /// Pushes a copy of the top element onto the stack
    virtual void push();

    /// Pops the top element out of the stack without returning it
    virtual void pop();

    /// Clears the stack and push a single identity matrix
    virtual void clear();

    /// Returns the element on the top of the stack
    /// \return Const reference of the element on the top
    virtual const M& top() const;

    /// Replaces the top element with the given matrix
    /// \param[in] mat The matrix to put on the stack
    virtual void load(const M& mat);

    /// Replaces the top element with its multiplication with the given matrix
    /// \param[in] mat The matrix to multiply with the top element
    virtual void mult(const M& mat);

    /// Verifies the emptyness of the stack
    /// \return True if the stack does not contain any element
    bool isEmpty() const;

private:
    std::stack<M> _stack;
};



// IMPLEMENTATION //
template <typename M>
MatrixStack<M>::MatrixStack(const M& mat) :
    _stack()
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
void MatrixStack<M>::clear()
{
    while(!_stack.empty())
        _stack.pop();

    _stack.push( M() );
}

template <typename M>
const M& MatrixStack<M>::top() const
{
    return _stack.top();
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

template <typename M>
inline bool MatrixStack<M>::isEmpty() const
{
    return _stack.empty();
}

}

#endif // CELLARWORKBENCH_MATRIXSTACK_H
