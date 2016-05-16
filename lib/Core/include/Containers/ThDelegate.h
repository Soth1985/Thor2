#pragma once

#include <Thor/Framework/Common.h>
#include <Thor/Framework/RefPtr.h>
#include <functional>
#include <type_traits>
#include <vector>


namespace Thor
{

THOR_FORWARD_CLASS(ThDelegateConnection);

class ThDelegateConnection:public ThRefCounted
{
public:
	typedef ThRefPtr<ThDelegateConnection>		pointer_t;
	typedef ThWeakPtr<ThDelegateConnection>		weak_pointer_t;

    virtual ~ThDelegateConnection()
    {
        //
    }

	virtual void Disconnect()=0;
};

namespace Private
{
	template <class FuncT>
	class ThDelegateBase;

	template <class FuncT>
	class ThDelegateConnectionImpl: public ThDelegateConnection
	{
	public:		
		typedef ThDelegateBase<FuncT>				delegate_t;
		typedef typename delegate_t::Connection		connection_t;

		virtual void Disconnect()
		{
			if(m_Del)
			{
				m_Del->Disconnect( m_Con );
				m_Del=0;
			}
		}

		ThDelegateConnectionImpl(delegate_t* del, connection_t* con)
			:
		m_Del(del),
		m_Con(con)
		{
			//
		}	

		~ThDelegateConnectionImpl()
		{
			Disconnect();
		}
	private:
		delegate_t*		m_Del;
		connection_t*	m_Con;
	};

	template <class FuncT>
	class ThDelegateBase
	{
	public:
		//typedef typename std::tr1::result_of<FuncT>::type		result_t;
		typedef std::tr1::function< FuncT >								function_t;
		typedef typename ThDelegateConnection::pointer_t				connection_t;
		typedef typename ThDelegateConnection::weak_pointer_t			weak_connection_t;
		typedef ThDelegateConnectionImpl<FuncT>							connection_value_type_t;

		ThDelegateBase(const ThDelegateBase& copy)
			:
		m_Connections(copy.m_Connections)
		{
			//
		}

		ThDelegateBase& operator=(const ThDelegateBase& rhs)
		{
			m_Connections=rhs.m_Connections;
			return *this;
		}

		~ThDelegateBase()
		{
			//remove all connections
			while( m_Connections.size() )
			{
				connection_t con = m_Connections.back().m_Conn.Lock();
				//if connection exists->disconnect, if not just remove from the function list
				if(con)
					con->Disconnect();
				else
				{
					assert(0 && "This should not happen");
					m_Connections.pop_back();
				}
			}
		}

		ThDelegateBase()
		{
			//THOR_STATIC_ASSERT(std::tr1::is_void<result_t>::value, "Return type should be void");
		}

        template <class FunctorT>
        connection_t ConnectFunctor(const FunctorT& func)
        {
            return Connect( std::tr1::ref(func) );
        }

		connection_t Connect(const function_t& func)
		{
			mutex_t::scoped_lock lock(m_Mutex);
			//insert function
			m_Connections.push_back( Connection(func) );
			//create connection
			connection_t result( new connection_value_type_t( this, &m_Connections.back() ) );
			//store a weak reference to the connection
			m_Connections.back().m_Conn=result;
			return result;
		}
		
	protected:

		struct Connection
		{			
			function_t			m_Func;
			weak_connection_t	m_Conn;

			Connection(const function_t& func)
				:
			m_Func(func)
			{
				//
			}
		};

		bool Disconnect(Connection* con)
		{
			mutex_t::scoped_lock lock(m_Mutex);

			if(!con)
				return false;
			//find connection
			connection_list_t::iterator pos = m_Connections.end();
			for( connection_list_t::iterator i = m_Connections.begin(); i != m_Connections.end(); ++i )
			{
				//check for equality using connection`s weak counters
				if( GetCounter(i->m_Conn)==GetCounter(con->m_Conn) )
				{
					pos=i;
					break;
				}
			}
			//erase connection if it exists
			if( pos != m_Connections.end() )
			{
				m_Connections.erase(pos);
				return true;
			}
			//connection not found
			return false;
		}	

		friend class ThDelegateConnectionImpl<FuncT>;

		typedef std::vector< Connection >	connection_list_t;
		typedef tbb::spin_mutex				mutex_t;

		mutex_t				m_Mutex;
		connection_list_t	m_Connections;
	};
}//Private

struct NullArg{};

template <class Arg0=NullArg, class Arg1=NullArg, class Arg2=NullArg, class Arg3=NullArg, class Arg4=NullArg, class Arg5=NullArg, class Arg6=NullArg, class Arg7=NullArg, class Arg8=NullArg, class Arg9=NullArg, class DummyArg=NullArg>
class ThDelegate;

template<>
class ThDelegate<>:public Private::ThDelegateBase<void()>
{
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj) );
    }

	void operator()()
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func();
		}
	}
};

template<class Arg0>
class ThDelegate<Arg0>:public Private::ThDelegateBase<void(Arg0)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1) );
    }

	void operator()(Arg0 a0)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0);
		}
	}
};

template<class Arg0, class Arg1>
class ThDelegate<Arg0, Arg1>:public Private::ThDelegateBase<void(Arg0, Arg1)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2) );
    }

	void operator()(Arg0 a0, Arg1 a1)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1);
		}
	}
};

template<class Arg0, class Arg1, class Arg2>
class ThDelegate<Arg0, Arg1, Arg2>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3>
class ThDelegate<Arg0, Arg1, Arg2, Arg3>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5, _6) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4, a5);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5, _6, _7) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4, a5, a6);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5, _6, _7, _8) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4, a5, a6, a7);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5, _6, _7, _8, _9) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4, a5, a6, a7, a8);
		}
	}
};

template<class Arg0, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9>
class ThDelegate<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9>:public Private::ThDelegateBase<void(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
{	
public:

    template <class MethodPtrT, class ObjectPtrT>
    connection_t ConnectMethod(MethodPtrT method, ObjectPtrT obj)
    {
        using namespace std::tr1::placeholders;	
		return Connect( std::tr1::bind(method, obj, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) );
    }

	void operator()(Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9)
	{
		for( ThSize i = 0; i < m_Connections.size(); ++i )
		{
			m_Connections[i].m_Func(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}
	}
};

}//Thor