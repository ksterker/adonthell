/*
   $Id: callback.h,v 1.3 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/**
 * @file   base/callback.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the callback template classes.
 * 
 * 
 */


#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_

#include <string.h>
#include <stddef.h>

namespace base
{

    /**
     * Base class for all our functors
     * 
     */
    class functor_base
    {
    protected:
        typedef void (functor_base::*PMemFunc)();
        typedef void (*PFunc)();
        enum {MEM_FUNC_SIZE = sizeof(PMemFunc)};
        union{
            PFunc func;
            char memFunc[MEM_FUNC_SIZE];
        };
        void *callee;
        functor_base();  
        functor_base(void *c, PFunc func, const void *memFunc, size_t sz);
        
    public:
        operator bool()const{return callee||func;}
        friend bool operator==(const functor_base &lhs,const functor_base &rhs);
        friend bool operator!=(const functor_base &lhs,const functor_base &rhs);
        friend bool operator<(const functor_base &lhs,const functor_base &rhs);
        PFunc	getFunc() const {return func;}
        void *getCallee() const {return callee;}
        const char *getMemFunc() const {return memFunc;}
    };


    /* *******************************************************
     * No argument, no return value.
     * *******************************************************
     */

    class functor_0:public functor_base
    {
    private:
        typedef void (*Thunk)(const functor_base &);
        Thunk thunk;
    protected:
      functor_0(Thunk t, void * c,PFunc f, const void * mf,size_t sz);
    public:
      functor_0();
      virtual ~functor_0();
        void operator()()const
        {
            thunk(*this);
        }
    };
    
    
    template <class Callee, class MemFunc>
    class membertranslator_0:public functor_0{
    public:
        membertranslator_0(Callee &c,const MemFunc &m):functor_0(thunk,&c,0,&m,sizeof(MemFunc)){}
        static void thunk(const functor_base &ftor)
        {
            Callee *callee = (Callee *)ftor.getCallee();
            MemFunc &memFunc = (*(MemFunc*)(void *)(ftor.getMemFunc()));
            (callee->*memFunc)();
        }
    };
    
    template <class Func>
    class functiontranslator_0:public functor_0{
    public:
        functiontranslator_0(Func f):functor_0(thunk,0,(functor_base::PFunc)f,0,0){}
        static void thunk(const functor_base &ftor)
        {
            (Func(ftor.getFunc()))();
        }
    };

    /**
     * Create a functor for a method
     * 
     */
    template <class Callee,class CallType,class TRT>
    inline functor_0 *
    make_functor(Callee &c,TRT (CallType::* f)())
    {
        functor_0 * ret = new functor_0();
        typedef TRT (CallType::*MemFunc)();
        *ret = membertranslator_0<Callee,MemFunc>(c,f);
        return ret;
    }

    /**
     * Create a functor for a method of a const object
     * 
     */
    template <class Callee,class CallType,class TRT>
    inline functor_0 *
    make_functor(const Callee &c, TRT (CallType::* f)()const)
    {
        functor_0 * ret = new functor_0();
        typedef TRT (CallType::*MemFunc)()const;
        *ret = membertranslator_0<const Callee,MemFunc>(c,f);
        return ret;
    }
    
    /**
     * Create a functor for a non-member function.
     * 
     */
    template <class TRT>
    inline functor_0 *
    make_functor(TRT (*f)())
    {
        functor_0 * ret = new functor_0();
        *ret = functiontranslator_0<TRT (*)()>(f);
        return ret;
    }
    


    /* *******************************************************
     * No argument, return value.
     * *******************************************************
     */
    
    template <class RT>
    class functor_0ret:public functor_base{
    public:
        functor_0ret(){}
        virtual ~functor_0ret() {}
	RT operator()()const
        {
            return thunk(*this);
        }
    protected:
	typedef RT (*Thunk)(const functor_base &);
	functor_0ret(Thunk t,void *c,PFunc f,const void *mf,size_t sz):
            functor_base(c,f,mf,sz),thunk(t){}
    private:
	Thunk thunk;
    };
    
    
    template <class RT, class Callee, class MemFunc>
    class membertranslator_0ret:public functor_0ret<RT>{
    public:
	membertranslator_0ret(Callee &c,const MemFunc &m):
            functor_0ret<RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
	static RT thunk(const functor_base &ftor)
        {
            Callee *callee = (Callee *)ftor.getCallee();
            MemFunc &memFunc = (*(MemFunc*)(void *)(ftor.getMemFunc()));
            return ((callee->*memFunc)());
        }
    };
    
    
    template <class RT,class Func>
    class functiontranslator_0ret:public functor_0ret<RT>{
    public:
	functiontranslator_0ret(Func f):functor_0ret<RT>(thunk,0,(functor_base::PFunc)f,0,0){}
	static RT thunk(const functor_base &ftor)
        {
            return (Func(ftor.getFunc()))();
        }
    };

    
    /**
     * Create a functor for a method
     * 
     */
    template <class RT,class Callee,class TRT,class CallType>
    inline functor_0ret<RT> *
    make_functor_ret(functor_0ret<RT>*,Callee &c,TRT (CallType::* f)())
    {
        functor_0ret<RT> * ret = new functor_0ret<RT>();
        typedef TRT (CallType::*MemFunc)();
        *ret = membertranslator_0ret<RT,Callee,MemFunc>(c,f);
        return ret;
    }

    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class RT,class Callee,class CallType>
    inline functor_0ret<RT> *
    make_functor_ret(Callee &c,RT (CallType::* f)())
    {
        return make_functor_ret((functor_0ret<RT> *) NULL, c, f);
    }
    
    
    
    /**
     * Create a functor for a method of a const object
     * 
     */
    template <class RT,class Callee,class TRT,class CallType>
    inline functor_0ret<RT> *
    make_functor_ret(functor_0ret<RT>*,const Callee &c, TRT (CallType::* f)()const)
    {
        functor_0ret<RT> * ret = new functor_0ret<RT>();
        typedef TRT (CallType::*MemFunc)()const;
        *ret = membertranslator_0ret<RT,const Callee,MemFunc>(c,f);
        return ret;
    }

    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class RT,class Callee,class CallType>
    inline functor_0ret<RT> *
    make_functor_ret(const Callee &c,RT (CallType::* f)()const)
    {
        return make_functor_ret((functor_0ret<RT> *) NULL, c, f);
    }
    
    
    /**
     * Create a functor for a non-member function.
     * 
     */
    template <class RT,class TRT>
    inline functor_0ret<RT> *
    make_functor_ret(functor_0ret<RT>*,TRT (*f)())
    {
        functor_0ret<RT> * ret = new functor_0ret<RT>();
        *ret = functiontranslator_0ret<RT,TRT (*)()>(f);
        return ret;
    }

    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class RT>
    inline functor_0ret<RT> *
    make_functor_ret(RT (*f)())
    {
        return make_functor_ret((functor_0ret<RT> *) NULL, f);
    }
    

    /* *******************************************************
     * One argument, no return value.
     * *******************************************************
     */

    template <class P1> class functor_1 : public functor_base
    {
    public:
        functor_1() {}
        virtual ~functor_1() {}
        void operator () (P1 p1) const
        {
            thunk (*this, p1);
        }
    protected:
        typedef void (*Thunk) (const functor_base &, P1);
        functor_1 (Thunk t, void *c, PFunc f, const void *mf, size_t sz)
            : functor_base (c, f, mf, sz), thunk (t) { }
    private:
        Thunk thunk;
    };
    
    template <class P1, class Callee, class MemFunc>
    class membertranslator_1 : public functor_1<P1>
    {
    public:
        membertranslator_1 (Callee & c, const MemFunc & m)
            : functor_1<P1> (thunk, &c, 0, &m, sizeof (MemFunc)) { }
        static void thunk (const functor_base & ftor, P1 p1)
        {
            Callee *callee = (Callee *) ftor.getCallee ();
            MemFunc & memFunc (*(MemFunc *) (void *)(ftor.getMemFunc ()));
            (callee->*memFunc) (p1);
        }
    };
    
    template <class P1, class Func> 
    class functiontranslator_1 : public functor_1 <P1>
    {
    public:
        functiontranslator_1 (Func f) : functor_1 <P1> (thunk, 0, (functor_base::PFunc)f, 0, 0) { }
        static void thunk (const functor_base & ftor, P1 p1)
        {
            (Func (ftor.getFunc())) (p1);
        }
    };
    
    /**
     * Create a functor for a method
     * 
     */
    template <class P1, class Callee, class TRT, class CallType, class TP1>
    inline functor_1<P1> *
    make_functor (functor_1<P1> *, Callee & c, TRT (CallType::* f) (TP1))
    {
        functor_1<P1> * ret = new functor_1<P1>();
        typedef TRT (CallType::*MemFunc) (TP1);
        *ret = membertranslator_1 <P1, Callee, MemFunc> (c, f);
        return ret;
    }

    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1, class Callee, class TRT, class CallType>
    inline functor_1<P1> *
    make_functor (Callee & c, TRT (CallType::* f) (P1))
    {
        return make_functor((functor_1<P1> *) NULL, c, f);
    }
    
    /**
     * Create a functor for a method of a const object
     * 
     */
    template <class P1, class Callee, class TRT, class CallType, class TP1>
    inline functor_1<P1> *
    make_functor (functor_1<P1> *, const Callee & c, TRT (CallType::*const &f) (TP1) const)
    {
        functor_1<P1> * ret = new functor_1<P1>();
        typedef TRT (CallType::*MemFunc) (TP1) const;
        *ret = membertranslator_1 <P1, const Callee, MemFunc> (c, f);
        return ret;
    }
    
    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1, class Callee, class TRT, class CallType>
    inline functor_1<P1> *
    make_functor (const Callee & c, TRT (CallType::*const f) (P1)const)
    {
        return make_functor((functor_1<P1> *) NULL, c, f);
    }

    /**
     * Create a functor for a non-member function.
     * 
     */
    template <class P1, class TRT, class TP1>
    inline functor_1<P1> *
    make_functor (functor_1<P1> *, TRT (*f) (TP1))
    {
        functor_1<P1> * ret = new functor_1<P1>();
        *ret = functiontranslator_1 <P1, TRT (*)(TP1) > (f);
        return ret;
    }
    
    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1, class TRT>
    inline functor_1<P1> *
    make_functor (TRT (*f) (P1))
    {
        return make_functor((functor_1<P1> *) NULL, f);
    }

    /* *******************************************************
     * One argument, return value.
     * *******************************************************
     */
    
    /**
     * Functor class - takes one argument, returns a value.
     * 
     */
    template <class P1,class RT>
    class functor_1ret:public functor_base
    {
    public:
        
        functor_1ret(){}
        virtual ~functor_1ret() {}
        
        /** 
         * Runs the connected function.
         * 
         * @arg p1 argument passed to the function
         * @return return value of the function
         */
        RT operator()(P1 p1)const
        {
            return (thunk(*this,p1));
        }
        
        //for STL
        typedef P1 argument_type;
        typedef RT result_type;
        
    protected:
        typedef RT (*Thunk)(const functor_base &,P1);
        functor_1ret(Thunk t,void *c,PFunc f,const void *mf,size_t sz):
            functor_base(c,f,mf,sz),thunk(t){}
        
    private:
        Thunk thunk;
    };
    
    /**
     * Translator class for creating a functor_1ret that will be connected to a member function.
     * 
     */
    template <class P1,class RT,class Callee, class MemFunc>
    class membertranslator_1ret:public functor_1ret<P1,RT>
    {
    public:
        membertranslator_1ret(Callee &c,const MemFunc &m):
            functor_1ret<P1,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
        
        static RT thunk(const functor_base &ftor,P1 p1)
        {
            Callee *callee = (Callee *)ftor.getCallee();
            MemFunc &memFunc(*(MemFunc*)(void *)(ftor.getMemFunc()));
            return ((callee->*memFunc)(p1));
        }
    };
    
    /**
     * Translator class for creating a functor_1ret that will be connected to a nonmember function.
     * 
     */
    template <class P1,class RT,class Func>
    class functiontranslator_1ret:public functor_1ret<P1,RT>
    {
    public:
	functiontranslator_1ret(Func f):
            functor_1ret<P1,RT>(thunk,0,(functor_base::PFunc)f,0,0){}
        
	static RT thunk(const functor_base &ftor,P1 p1)
        {
            return (Func(ftor.getFunc()))(p1);
        }
    };
    

    /**
     * Create a functor for a method
     * 
     */
    template <class P1,class RT, class Callee,class TRT,class CallType,class TP1>
    inline functor_1ret<P1,RT> *
    make_functor_ret(functor_1ret<P1,RT>*, Callee &c, TRT (CallType::*  f)(TP1))
    {
        functor_1ret<P1,RT> * ret = new functor_1ret<P1,RT>();
        typedef TRT (CallType::*MemFunc)(TP1);
        *ret = membertranslator_1ret<P1,RT,Callee,MemFunc>(c,f);
        return ret;
    }
    
    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1,class RT, class Callee, class CallType>
    inline functor_1ret<P1,RT> *
    make_functor_ret(Callee &c, RT (CallType::*  f)(P1))
    {
        return make_functor_ret((functor_1ret<P1,RT> *) NULL, c, f);
    }
    
    /**
     * Create a functor for a method of a const object
     * 
     */
    template <class P1,class RT, class Callee,class TRT,class CallType,class TP1>
    inline functor_1ret<P1,RT> *
    make_functor_ret(functor_1ret<P1,RT>*, const Callee &c,TRT (CallType::*  f)(TP1)const)
    {
        functor_1ret<P1,RT> * ret = new functor_1ret<P1,RT>();
        typedef TRT (CallType::*MemFunc)(TP1)const;
        *ret = membertranslator_1ret<P1,RT,const Callee,MemFunc>(c,f);
        return ret;
    }
    
    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1,class RT, class Callee, class CallType>
    inline functor_1ret<P1,RT> *
    make_functor_ret(const Callee &c, RT (CallType::*  f)(P1)const)
    {
        return make_functor_ret((functor_1ret<P1,RT> *) NULL, c, f);
    }

    /**
     * Create a functor for a non-member function.
     * 
     */
    template <class P1,class RT,class TRT,class TP1>
    inline functor_1ret<P1,RT> *
    make_functor_ret(functor_1ret<P1,RT>*,TRT (*f)(TP1))
    {
        functor_1ret<P1,RT> * ret = new functor_1ret<P1,RT>;
        *ret = functiontranslator_1ret<P1,RT,TRT (*)(TP1)>(f);
        return ret;
    }
    
    /**
     * Shorter version - function arguments must exactly match their declaration
     * to use this version
     * 
     */
    template <class P1,class RT>
    inline functor_1ret<P1,RT> *
    make_functor_ret(RT (*f)(P1))
    {
        return make_functor_ret((functor_1ret<P1,RT> *) NULL, f);
    }
}

#endif
