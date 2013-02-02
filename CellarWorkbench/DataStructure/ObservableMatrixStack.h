#ifndef CELLARWORKBENCH_OBSERVABLEMATRIXSTACK_H
#define CELLARWORKBENCH_OBSERVABLEMATRIXSTACK_H

#include "MatrixStack.h"
#include "DesignPattern/SpecificSubject.h"


namespace cellar
{
    template <typename M>
    class ObservableMatrixStack : public MatrixStack<M>,
                                  public SpecificSubject< ObservableMatrixStack<M> >
    {
    public:
        ObservableMatrixStack();
        explicit ObservableMatrixStack(const M& mat);

        // Stack operations
        virtual void pop();     // notify observers
        virtual void clear();   // notify observers

        // Matrix specific
        virtual void load(const M& mat);    // notify observers
        virtual void mult(const M& mat);    // notify observers
    };



    // IMPLEMENTATION //
    template <typename M>
    ObservableMatrixStack<M>::ObservableMatrixStack() :
        MatrixStack<M>()
    {
    }

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
