#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// NOTE: Do not include any additional headers here. This is an external facing header
//       so avoid any internal dependencies.
////////////////////////////////////////////////////////////////////////////////////////

// The Unity EventQueue is a standard way of passing information between plugins.
// The EventQueue is always serviced on the main simulation thread.
// Payloads should be very small.
//
// Plugins can be passed the EventQueue object on initialization.

// This defines the EventId for your event payload (struct)
// Use this for events that do not require a destructor.
#define REGISTER_EVENT_ID(HASHH, HASHL ,TYPE)                  \
namespace UnityEventQueue                                      \
{                                                              \
    template<>                                                 \
    inline const EventId GetEventId< TYPE > ()                 \
    {                                                          \
        return EventId(HASHH,HASHL) ;                          \
    }                                                          \
}

// If your event payload requires a destructor. Define a:
//
// void Destroy()
//
// Method on your payload and then use this. If your event
// can guarantee that someone is listening before you fire
// your first event this is all you have to do.
//
// However if you do not know if someone is listening when you
// fire your first event you will need to call RegisterCleanup
// somewhere to ensure your events get cleaned up properly.
#define REGISTER_EVENT_ID_WITH_CLEANUP(HASHH,HASHL,TYPE)                             \
namespace UnityEventQueue                                                            \
{                                                                                    \
	typedef StaticFunctionEventHandler< TYPE > DestructorMethod_##HASHH##HASHL;      \
                                                                                     \
	template<>                                                                       \
	inline void GenericDestructorMethodForType< TYPE > ( const TYPE & eventPayload ) \
	{ const_cast<TYPE&>(eventPayload).Destroy(); }                                   \
                                                                                     \
	template<>                                                                       \
	inline EventHandler * GetEventDestructor< TYPE >()                               \
	{                                                                                \
		static DestructorMethod_##HASHH##HASHL g_Destructor(&GenericDestructorMethodForType< TYPE >); \
		return &g_Destructor;                                                        \
	}                                                                                \
}                                                                                    \
REGISTER_EVENT_ID(HASHH,HASHL,TYPE)


namespace UnityEventQueue
{

	// EventId - GUID
	// To facilitate custom events the EventId object must be a full fledged GUID
	// to ensure cross plugin uniqueness constraints.
	//
	// Template specialization is used to produce a means of looking up an
	// EventId from it's payload type at compile time. The net result should compile
	// down to passing around the GUID.
	//
	// REGISTER_EVENT_ID should be placed in
	// the header file of any payload definition OUTSIDE of all namespaces (all EventIds live
	// in the UnityEventQueue namespace) The payload structure and the registration ID are all that
	// is required to expose the event to other systems.
	//
	// There is a tool built in to Unity to generate registration macros for EventIds.
	struct EventId
	{
	public:
		EventId(unsigned long long high, unsigned long long low)
			: mGUIDHigh(high)
			, mGUIDLow(low)
		{}

		EventId( const EventId & other )
		{
			mGUIDHigh = other.mGUIDHigh;
			mGUIDLow  = other.mGUIDLow;
		}

		EventId & operator=(const EventId & other )
		{
			mGUIDHigh = other.mGUIDHigh;
			mGUIDLow  = other.mGUIDLow;
			return *this;
		}

		bool Equals(const EventId & other)   const		{	return mGUIDHigh == other.mGUIDHigh && mGUIDLow == other.mGUIDLow;	}
		bool LessThan(const EventId & other) const		{	return mGUIDHigh < other.mGUIDHigh || (mGUIDHigh == other.mGUIDHigh && mGUIDLow < other.mGUIDLow);	}

		unsigned long long mGUIDHigh;
		unsigned long long mGUIDLow;
	};
	inline bool operator==(const EventId & left, const EventId & right ) {	return left.Equals(right);	    }
	inline bool operator!=(const EventId & left, const EventId & right ) {	return !left.Equals(right);	    }
	inline bool operator< (const EventId & left, const EventId & right ) {	return left.LessThan(right);    }
	inline bool operator> (const EventId & left, const EventId & right ) {	return right.LessThan(left);	}
	inline bool operator>=(const EventId & left, const EventId & right ) {	return !operator< (left,right);	}
	inline bool operator<=(const EventId & left, const EventId & right ) {	return !operator> (left,right);	}
	// Generic Version of GetEventId to allow for specialization
	//
	// If you get errors about return values related to this method
	// then you have forgotten to include REGISTER_EVENT_ID() for your
	// payload / event. This method should never be compiled, ever.
	template< typename T > const EventId GetEventId() {}
	class EventQueue;
	class EventHandler;

	// Generic version retrieving a classes destructor.
	// Any class that does not specialize this will not define a destructor.
	template< typename T > EventHandler * GetEventDestructor() { return (EventHandler*)NULL; }

	// This is a static method that helps us call a meaningful method on the event
	// itself improving the cleanliness of the EQ design.
	template< typename T > void GenericDestructorMethodForType(const T & /*eventPayload*/ ) {}

	// ====================================================================
	// ADAPTER / DELEGATE - This is the interface that the EventQueue
	// uses to know about listeners for events.
	//
	// Use the StaticFunction of ClassBased EventHandlers to build
	// adapters to your systems and empower them to receive events.
	class EventHandler
	{
	public:
		EventHandler()	: m_Next(0)	{}
		virtual ~EventHandler()     {}
		// This actually kicks the event to the handler function or object.
		virtual void HandleEvent ( EventId & id, void * data ) = 0;
		// This is required when registering this handler
		virtual EventId HandlerEventId()                       = 0;
		// This is required when registering this handler
		virtual EventHandler * GetMyEventDestructor()          = 0;
		// Internal, do not use, required for walking and calling handlers
		EventHandler * GetNext()                 { return m_Next; }
	private:
		// Intrusive list holding a linked list of handlers for this EventId
		// Exists to avoid allocations - Do Not Touch.
		friend class EventHandlerList;
		EventHandler * m_Next;
	};

	// ====================================================================
	// CLASS DELEGATE - Classes can be the target of events.
	//
	// Event handlers are the endpoints of the system. To Unity all
	// event endpoints look like a single virtual function call.
	//
	// This adapter will call the HandleEvent( EVENTTYPE & ) method
	// on the object specified when an event is triggered.
	template< typename EVENTTYPE, typename OBJECTTYPE >
	class ClassBasedEventHandler : public EventHandler
	{
	public:
		ClassBasedEventHandler( OBJECTTYPE * handler = NULL ) : m_Handler(handler) {}

		// The actual adapter method, calls into the registered object.
		virtual void HandleEvent( EventId & id, void * data )
		{ (void)id; m_Handler->HandleEvent( *static_cast<EVENTTYPE*>(data) );	}

		// Boilerplate required when registering this handler.
		virtual EventId HandlerEventId()
		{ return GetEventId<EVENTTYPE>(); }

		// Required boilerplate. Used during registration of this object.
		virtual EventHandler * GetMyEventDestructor()
		{ return GetEventDestructor<EVENTTYPE>(); }

		ClassBasedEventHandler<EVENTTYPE,OBJECTTYPE> *
		SetObject( OBJECTTYPE * handler )
		{ m_Handler = handler; return this; }

		OBJECTTYPE * GetHandler()
		{ return m_Handler; }
	protected:
		OBJECTTYPE * m_Handler;
	};

	// ====================================================================
	// FUNCTION DELEGATE - Static functions can be event handlers.
	//
	// This object wraps a static function turning it into an event endpoint
	// much like a C# delegate does.
	// The wrapped function will be called when the event is triggered
	template< typename EVENTTYPE >
	class StaticFunctionEventHandler : public EventHandler
	{
	public:
		typedef void (*HandlerFunction)(const EVENTTYPE & payload);

		StaticFunctionEventHandler( HandlerFunction handlerCallback ) : m_Handler(handlerCallback)	{}

		virtual void HandleEvent( EventId & id, void * data )
		{ (void)id; m_Handler(*static_cast<EVENTTYPE*>(data)); }

		// Required boilerplate. Used during registration of this object.
		virtual EventId HandlerEventId()
		{ return GetEventId<EVENTTYPE>(); }

		// Required boilerplate. Used during registration of this object.
		virtual EventHandler * GetMyEventDestructor()
		{ return GetEventDestructor<EVENTTYPE>(); }

	protected:
		HandlerFunction m_Handler;
	};

	// ============================================================
	// Some built in event types for adding removing event handlers
	// from secondary threads. Use these instead of calling AddHandler
	// from a secondary thread.
	struct AddEventHandler
	{
		AddEventHandler( EventHandler * handler ) : m_Handler(handler) {}
		EventHandler * m_Handler;
	};

	struct RemoveEventHandler
	{
		RemoveEventHandler( EventHandler * handler ) : m_Handler(handler) {}
		EventHandler * m_Handler;
	};

	// Debug printfs to the log have a maximum size when being sent as events.
	// NOTE: avoid sending these in master builds! Your plugin should respect that.
	// This does however allow plugins to insert text into the standard debug log.
	#define kMaxDebugLogLine 64
	struct DebugLogEvent
	{
		char m_Buffer[kMaxDebugLogLine];
	};

	// ============================================================
	// The Event Queue is a lock free multiple write single read
	// deferred event system. It uses GUIDs to map payloads to
	// event handler delegates. The system has some
	// templates to make registering for events fairly painless
	// but takes care to try to keep template cost very low.
	//
	// NOTE: payloads should be very very small and never allocate
	//       or free memory since they can and will be passed across
	//       dll boundaries.
	//
	// There is a hard limit of kMaxEventQueueEventSize bytes for any
	// payload being passed through this system but payloads that are
	// this big are probably being handled improperly.
	class IEventQueue
	{
	public:
		#define kMaxEventQueueEventSize 2048+sizeof(EventId)

		virtual ~IEventQueue() {}

		// The primary entry point for firing any
		// event through the system. The templated
		// version is simply syntatic sugar for extracting
		// the EventId from the event object.
		//
		// This can be called from any thread.
		template<typename T>
		void SendEvent(T & payload)
		{
			// Ensure we never fire an event that we can't handle size wise.
			Assert( sizeof(T) < (kMaxEventQueueEventSize-sizeof(EventId)) );

			// NOTE: Keep this small to avoid code bloat.
			//       every line of code in here should be scrutinized
			//       as this and GetEventId could easily be sources
			//       of bloat if allowed to grow.
			SendEventImpl(UnityEventQueue::GetEventId<T>(), (unsigned char *)(&payload), sizeof(T));
		}

		// Some events want to have a destructor.
		//
		// You do this by creating a public Destroy()
		// method on your Event payload. You then
		// register the event with REGISTER_EVENT_ID_WITH_CLEANUP
		//
		// If your event will have someone listening to it right away
		// then you do not need to do anything else.
		//
		// However, if there is a danger that your event will be fired
		// before someone starts listening for it then you must
		// call this method somewhere. This will manually register your
		// class for cleanup. There is no harm in calling this even
		// if it is not required. If in doubt and your event payload
		// requires some form of destruction, call this method!
		template<typename T>
		void RegisterCleanup(T & payload)
		{
			EventHandler * eh = UnityEventQueue::GetEventDestructor<T> ();
			if (eh != NULL)
				SetCleanupImpl(eh);
		}

		// These are not thread safe and must be done on the same thread
		// as the dispatch thread. Doing otherwise risks race conditions.
		// Fire the add handler / remove handler event if you need to
		// schedule this from a different thread.
		virtual void AddHandler(EventHandler * handler)    = 0;
		virtual void RemoveHandler(EventHandler * handler) = 0;
	protected:
		virtual void SendEventImpl(EventId id, unsigned char * data, int size) = 0;

		// This is an event destructor. You can register one of these
		// per event type. This allows events to manage resource if
		// they absolutely must. In general you should avoid handling
		// resources in events as events usually imply cross thread activity.
		virtual void SetCleanupImpl(EventHandler * handler) = 0;
	};
}

REGISTER_EVENT_ID(0x19D736400584B24BULL,0x98B9EFBE26D3F3C9ULL,AddEventHandler)
REGISTER_EVENT_ID(0x8D4A317C4F577F4AULL,0x851D6E457566A905ULL,RemoveEventHandler)
REGISTER_EVENT_ID(0x8F0D898251731644ULL,0x904C3F1D04BAB17CULL,DebugLogEvent)
