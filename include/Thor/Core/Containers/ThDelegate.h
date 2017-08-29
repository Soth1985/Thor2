#pragma once

#include <memory>
#include <functional>
#include <list>

namespace Thor
{
	template <class ... ArgsT>
	class ThDelegate;

	namespace Private
	{
		class ThDelegateConnectionBase
		{
		public:
			typedef std::shared_ptr<ThDelegateConnectionBase>	Pointer;
			typedef std::weak_ptr<ThDelegateConnectionBase>		WeakPointer;

			virtual ~ThDelegateConnectionBase()
			{
				//
			}

			virtual void Disconnect() = 0;
			virtual void Reset() = 0;
		};

		template <class ... ArgsT>
		class ThDelegateConnectionImpl : public ThDelegateConnectionBase
		{
		public:
			typedef ThDelegate<ArgsT...> Delegate;
			typedef typename Delegate::SlotIterator SlotIterator;

			virtual void Disconnect()override
			{
				if (m_Del)
				{
					m_Del->Disconnect(m_Slot);
					m_Del = nullptr;
				}
			}

			ThDelegateConnectionImpl(Delegate* del, SlotIterator slot)
				:
			m_Del(del),
			m_Slot(slot)
			{
				//
			}

			~ThDelegateConnectionImpl()
			{
				Disconnect();
			}

			virtual void Reset()override
			{
				m_Del = nullptr;
			}
		private:
			Delegate* m_Del;
			SlotIterator m_Slot;
		};		
	}//Private

	class ThDelegateConnection
	{
	public:
		typedef typename Private::ThDelegateConnectionBase::Pointer Connection;

		ThDelegateConnection()
		{

		}

		~ThDelegateConnection()
		{
			Disconnect();
		}

		ThDelegateConnection(const Connection& connection)
			:
		m_Connection(connection)
		{

		}

		ThDelegateConnection(const ThDelegateConnection& copy)
		{
			//no-op, dont point to other object method
		}

		ThDelegateConnection& operator=(const ThDelegateConnection& copy)
		{
			m_Connection = copy.m_Connection;
			return *this;
		}

		ThDelegateConnection& operator=(ThDelegateConnection&& move)
		{
			Disconnect();
			m_Connection = move.m_Connection;
			move.m_Connection = nullptr;
			return *this;
		}

		ThDelegateConnection(const ThDelegateConnection&& move)
		{
			//no-op, dont point to other object method
		}

		void Disconnect()
		{
			if (m_Connection)
			{
				m_Connection->Disconnect();
				m_Connection.reset();
			}
		}

	private:
		Connection m_Connection;
	};

	template <class ... ArgsT>
	class ThDelegate
	{
	public:

		typedef std::function<void(ArgsT...)> Function;
		typedef typename Private::ThDelegateConnectionBase::Pointer Connection;
		typedef typename Private::ThDelegateConnectionBase::WeakPointer WeakConnection;
		typedef Private::ThDelegateConnectionImpl<ArgsT...> ConnectionImpl;

		ThDelegate()
		{

		}

		ThDelegate(const ThDelegate& copy)
		{
			//noop
		}

		ThDelegate& operator=(const ThDelegate& rhs)
		{
			//noop
			return *this;
		}

		ThDelegate(const ThDelegate&& move)
		{
			//noop
		}

		ThDelegate& operator=(const ThDelegate&& rhs)
		{
			//noop
			return *this;
		}

		~ThDelegate()
		{
			for (auto i = m_Slots.begin(); i != m_Slots.end(); ++i)
			{
				Connection con = i->m_Conn.lock();

				if (con)
					con->Reset();
			}
		}

		ThDelegateConnection Connect(const Function& func)
		{
			//insert function
			m_Slots.push_back(Slot(func));
			SlotIterator last = m_Slots.end();
			--last;
			//create connection
			auto impl = std::make_shared<ConnectionImpl>(this, last);
			Connection result = impl;
			//store a weak reference to the connection
			m_Slots.back().m_Conn = result;
			return ThDelegateConnection(result);
		}

		void Invoke(ArgsT... args)
		{
			SlotIterator slot = m_Slots.begin();
			while (slot != m_Slots.end())
			{
				Connection con = slot->m_Conn.lock();

				if (con)
				{
					slot->m_Func(args...);
					++slot;
				}
				else
				{
					slot = m_Slots.erase(slot);
				}
			}
		}

	protected:

		struct Slot
		{
			Function	m_Func;
			WeakConnection m_Conn;

			Slot(const Function& func)
				:
			m_Func(func)
			{
				//
			}
		};

		friend class ConnectionImpl;

		typedef std::list< Slot > SlotList;
		typedef typename SlotList::iterator SlotIterator;

		void Disconnect(SlotIterator slot)
		{
			slot->m_Conn.reset();
			slot->m_Func = nullptr;			
		}	

		SlotList m_Slots;
	};
}//Thor