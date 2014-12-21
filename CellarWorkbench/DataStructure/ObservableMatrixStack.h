#ifndef CELLARWORKBENCH_OBSERVABLEMATRIXSTACK_H
#define CELLARWORKBENCH_OBSERVABLEMATRIXSTACK_H

#include "MatrixStack.h"
#include "../DesignPattern/SpecificSubject.h"


namespace cellar
{

/// \see MatrixStack
/// This implementation of MatrixStack pushes update notifications to observers
/// \note ObservableMatrixStack::push does not notifies the observers
template <typename M>
class ObservableMatrixStack :
        public MatrixStack<M>,
        public SpecificSubject< ObservableMatrixStack<M> >
{
public:
    /// Constructor
    /// \param[in] mat The initial matrix of the stack
    explicit ObservableMatrixStack(const M& mat = M());

    /// \see MatrixStack::pop
    /// \note Notifies observers
    virtual void pop();

    /// \see MatrixStack::clear
    /// \note Notifies observers
    virtual void clear();

    /// \see MatrixStack::load
    /// \note Notifies observers
    virtual void load(const M& mat);

    /// \see MatrixStack::mult
    /// \note Notifies observers
    virtual void mult(const M& mat);
};



// IMPLEMENTATION //
template <typename M>
ObservableMatrixStack<M>::ObservableMatrixStack(const M& mat) :
    MatrixStack<M>( mat )
{
}

template <typename M>
void ObservableMatrixStack<M>::pop()
{
    MatrixStack<M>::pop();
    this->setIsChanged( true );
    this->notifyObservers( *this );
}

template <typename M>
void ObservableMatrixStack<M>::clear()
{
    MatrixStack<M>::clear();
    this->setIsChanged( true );
    this->notifyObservers( *this );
}

template <typename M>
void ObservableMatrixStack<M>::load(const M& mat)
{
    MatrixStack<M>::load( mat );
    this->setIsChanged( true );
    this->notifyObservers( *this );
}

template <typename M>
void ObservableMatrixStack<M>::mult(const M& mat)
{
    MatrixStack<M>::mult( mat );
    this->setIsChanged( true );
    this->notifyObservers( *this );
}
}

#endif // CELLARWORKBENCH_OBSERVABLEMATRIXSTACK_H
